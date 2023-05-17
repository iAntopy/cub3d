/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 10:21:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 18:41:58 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int next_obj(t_oinst **obj_p)
{
	*obj_p = (*obj_p)->next;
	return (1);
}

static int	prtl_proj_init_single_vect(t_pdata *pd, t_rdata *rd, t_oinst *obj, t_oinst *link)
{
	pd->odist = obj->dist * (*pd->fwd_len);
	pd->dist = pd->odist;
	pd->px = *rd->px + pd->odist * (*rd->rx) + (link->px - obj->px);
	pd->py = *rd->py + pd->odist * (*rd->ry) + (link->py - obj->py);
	pd->cx = (int)(pd->px * rd->inv_cw);
	pd->cy = (int)(pd->py * rd->inv_cw);
	pd->b = pd->py - (rd->a * pd->px);
	return (1);
}

static int	prtl_proj_probe(t_pdata *pd, float *axs, float *isct, float *dists)
{
	const t_rdata	*rd = pd->rdata;
	float	ratio;
	int		correction;

	if (dists[0] < dists[1])
	{
		pd->dist += dists[0];
		pd->hitx = isct[0];
		pd->hity = axs[1];
		pd->side = 1 + (rd->dy << 1);
		ratio = isct[0] - axs[0];
		correction = (*rd->rx < 0) != (*rd->ry < 0);
	}
	else
	{
		pd->dist += dists[1];
		pd->hitx = axs[0];
		pd->hity = isct[1];
		pd->side = rd->dx << 1;
		ratio = isct[1] - axs[1];
		correction = (*rd->rx < 0) == (*rd->ry < 0);
	}
	if ((pd->side == W_SIDE) || (pd->side == S_SIDE))
		ratio = -ratio;
	pd->tex_ratio = correction + (ratio * rd->inv_cw);
	pd->tex_height = (*rd->near_proj_factor) / pd->dist;
	return (0);
}

static inline uint32_t	*init_proj_wcol(t_cub *cub, t_pdata *pd, t_rcol *rc, int *tshape, int max_h)//, int *start_x)
{
	mlx_texture_t	*tex;
	int				tex_start_x;

	tex = cub->map.mx[pd->cy][pd->cx]->xwalls[pd->side];
//	tex = cub->tex.walls[rd->side];
	tshape[0] = tex->width;
	tshape[1] = tex->height;
	rc->half_texh = (tex->height >> 1);
	tex_start_x = (int)(pd->tex_ratio * tex->width);
//	printf("tex_start_x : %d\n", tex_start_x);
//	printf("tex_start_x : %d, tex ratio : %f\n", tex_start_x, pd->tex_ratio);
	if (max_h > SCN_HEIGHT)
		max_h = SCN_HEIGHT;
	rc->scn_height = ft_clamp(pd->tex_height, 0, max_h);
	rc->half_height = (rc->scn_height >> 1);
	rc->ratio = (float)tex->height / (float)pd->tex_height;
//	printf("tex->height: %d, pd->tex_height: %f, ratio: %f\n",
//		tex->height, pd->tex_height, rc->ratio);
	rc->scn_start_y = ((SCN_HEIGHT - rc->scn_height) >> 1);
	rc->scn_end_y = ((SCN_HEIGHT + rc->scn_height) >> 1);
	return ((uint32_t *)tex->pixels + tex_start_x);// + ((int)(rc->scn_start_y * rc->ratio) * tex->width));
}

// Pass the t_pdata * offset in array to the starting ray to cast from 0 <= i < SCN_WIDTH.
// n is th nb of rays to cast from start (end - start).
static int	prtl_proj_vectors(t_pdata *pd, t_map *map, t_oinst *obj, int *pframe)//, int n)///int start, int end)
{
	t_rdata	*rd;
	float	*axs;
	float	isct[2];
	float	dists[2];
	int		width;
	
//	printf("Portal Projecting \n");
	pd += pframe[0];
	rd = pd->rdata;// - 1;
	width = pframe[2] - pframe[0];
	dists[0] = 0;
	dists[1] = 0;
	isct[0] = 0;
	isct[1] = 0;
	axs = NULL;
	while (width--)
	{
		prtl_proj_init_single_vect(pd, pd->rdata, obj, obj->relative);
		while (1)//!(is_wall(map, pd->cx, pd->cy) && prtl_proj_probe(pd, axs, isct, dists) == 0))
		{
			axs = map->grid_coords[pd->cy + rd->dy] + ((pd->cx + rd->dx) << 1);
//			printf("axs : %p\n", axs);
			isct[1] = rd->a * axs[0] + pd->b;
			isct[0] = (axs[1] - pd->b) * rd->inv_a;
			dists[0] = (isct[0] - pd->px) * (*rd->p_dirx)
				+ (axs[1] - pd->py) * (*rd->p_diry);
			dists[1] = (axs[0] - pd->px) * (*rd->p_dirx)
				+ (isct[1] - pd->py) * (*rd->p_diry);
			if (dists[0] < dists[1])
				pd->cy += rd->cincr_y;
			else
				pd->cx += rd->cincr_x;
			if (is_wall(map, pd->cx, pd->cy) && prtl_proj_probe(pd, axs, isct, dists) == 0)
				break ;
		}
		++pd;
		++rd;
	}
	return (0);
}

void	__render_proj_walls(t_cub *cub)
{
	const int		*pframe = cub->renderer.pframe;
	const int		proj_height = pframe[3] - pframe[1];

	int			i;
	int			j;
	t_pdata		*pd;
	uint32_t	*pbuff;
	float		*dpbuff;
	uint32_t	*pb;
	t_rcol		rc;
	int			tex_shape[2];
	float		tex_y;
	uint32_t	*tex_buff;
	char		*isproj;
	
	pbuff = (uint32_t *)cub->renderer.objs_layer->pixels;
	i = pframe[0] - 1;
	while (++i < pframe[2])
	{
		pd = cub->hero.rcast.prtl_proj + i;
//		++pbuff;
		pb = pbuff + i;// - SCN_WIDTH;
		
//		dpbuff = cub->renderer.dpbuff + i - SCN_WIDTH;
//		pb = pbuff + i;
//		isproj = cub->renderer.isproj + i - SCN_WIDTH;// + i * SCN_HEIGHT - 1;
//		dpbuff = cub->renderer.dpbuff + (i - 1) - SCN_WIDTH;// + i * SCN_HEIGHT - 1;
		
//		dbuff = cub->renderer.dbuff + scn_offx + (loffs[1] - 1) * SCN_WIDTH;// * SCN_HEIGHT - 1;
//		isproj = cub->renderer.isproj + scn_offx + (loffs[1] - 1) * SCN_WIDTH;// - 1;
		
//		tex_buff = init_proj_wcol(cub, pd, &rc, tex_shape);//, pframe[2] - pframe[0]);
		tex_buff = init_proj_wcol(cub, pd, &rc, tex_shape, proj_height);// + ((pframe[1] - 1) * SCN_WIDTH);
	
		j = rc.scn_start_y - 1;//pframe[1] - 1;
		isproj = cub->renderer.isproj + i + cub->buff_offys[j + 1] - SCN_WIDTH;
		dpbuff = cub->renderer.dpbuff + i + cub->buff_offys[j + 1] - SCN_WIDTH;
//		printf("dpbuff init offset : x (i = %d) %d, y (j = %d) %d\n", i, i, j, cub->buff_offys[j]);
//		pb = pbuff + i + cub->buff_offys[j] - SCN_WIDTH;
		while (++j < rc.scn_end_y)//pframe[3])
		{
			isproj += SCN_WIDTH;
			dpbuff += SCN_WIDTH;

			if (!*isproj || (*dpbuff && *dpbuff < pd->dist))
				continue ;
			tex_y = (j - cub->scn_midy) * rc.ratio + rc.half_texh;//(int)(j_to_midy[j] * divergent_lens_ratio[j] * rc.ratio) + rc.half_texh;
//			printf("(j - cub->scn_midy) (%d) * ratio (%f) + half_tex_h (%d) =  tex_y : %f, tex_shape w: %d \n",
//				(j - cub->scn_midy), rc.ratio, rc.half_texh, tex_y, tex_shape[0]);
		
//			pb[j * SCN_WIDTH] = tex_col;
//			pb[j_times_sw[j]] = //tex_buff[tex_y * tex_shape[0]];
//			pb[cub->buff_offys[j]] = tex_col;
//			pbuff[i + j * SCN_WIDTH] = tex_col;
			pb[cub->buff_offys[j]] = tex_buff[(int)tex_y * tex_shape[0]] & TRANSPARENCY;//tex_col;
//			pbuff[i + j * SCN_WIDTH] = tex_buff[(int)tex_y * tex_shape[0]] & TRANSPARENCY;
//			*pb = tex_buff[(int)tex_y * tex_shape[0]];//tex_col;
//			cub_put_pixel(cub->renderer.objs_layer, scn_offx, loffs_y[j],
//				tex_col);
			*dpbuff = pd->dist;
		}
	}
}

void	__render_proj_obj(t_cub *cub, int dist, mlx_texture_t *tex, t_pdata *pd, int *dims,
		int *loffs, int *toffs, float *tex_incrs)//, int *end)
{
	int		toffs_y[SCN_HEIGHT];
	int		loffs_y[SCN_HEIGHT];
	float		*dbuff;
	char		*isproj;
	int			i;
	int			j;
	int			scn_offx;
	int			tex_offx;
	uint32_t	*pxls;
	uint32_t	tex_col;

	j = -1;
	while (++j < dims[1])
	{
		toffs_y[j] = (int)((j + toffs[1]) * tex_incrs[1]) * tex->width;
		loffs_y[j] = j + loffs[1];
	}
//	dbuff = cub->renderer.dbuff;
	pxls = (uint32_t *)tex->pixels;
	i = -1;
	while (++i < dims[0])
	{
		if (pd[i + loffs[0]].dist < dist)
			continue ;
		scn_offx = i + loffs[0];
		dbuff = cub->renderer.dpbuff + scn_offx + (loffs[1] - 1) * SCN_WIDTH;
		isproj = cub->renderer.isproj + scn_offx + (loffs[1] - 1) * SCN_WIDTH;
		tex_offx = (int)((i + toffs[0]) * tex_incrs[0]);

		j = -1;
		while (++j < dims[1])
		{
//			printf("RENDER PROJ OBJ !\n");
			dbuff += SCN_WIDTH;
			isproj += SCN_WIDTH;
//			scn_offs[1] = j + loffs[1];
			tex_col = pxls[tex_offx + toffs_y[j]];//(int)(toffs_y[j] * tex_incrs[1]) * tex->width];
//			++dbuff;
			if (!tex_col || !*isproj || (*dbuff && dist > *dbuff))
				continue ;
			cub_put_pixel(cub->renderer.objs_layer, scn_offx, loffs_y[j], tex_col);
			*dbuff = dist;
	//		dbuff[i + j * SCN_WIDTH] = dist;
		}
	}
}

void	__render_proj_objects(t_cub *cub)//, t_oinst *prtl, t_pdata *pdata, int *pframe)
{
	const int	*pframe = cub->renderer.pframe;
	t_oinst		*obj;
	t_oinst		*prtl;
	t_oinst		*link;
	float		ppos[2];
	float		ov[2];
	int			drawx;
	float		ratio;
	float		odist;
	mlx_texture_t	*tex;

	int		loffs[4];
	int		toffs[2];
	float		tincrs[2];
	int		dims[2];

	obj = cub->objs.instances;
	prtl = (t_oinst *)cub->renderer.portal;
	link = (t_oinst *)cub->renderer.portal->relative;
	ppos[0] = cub->hero.px + (link->px - prtl->px);
	ppos[1] = cub->hero.py + (link->py - prtl->py);
//	printf("\nlink id %d %p, (%.2f, %.2f)\n", link->_id, link, link->px, link->py);
	while (obj)
	{
		if ((!obj->type->is_drawable || !obj->type->gset) && next_obj(&obj))
//		{
//			obj = obj->next;
			continue ;
//		}
//		printf("\n\n Rendering obj %p\n", obj);
//		ov[0] = obj->px - link->px;
//		ov[1] = obj->py - link->py;
		ov[0] = obj->px - ppos[0];
		ov[1] = obj->py - ppos[1];
//		printf("obj id %d (%.2f, %.2f) to portal id %d (%.2f, %.2f)\n", obj->_id, obj->px, obj->py, link->_id, link->px, link->py);
//		obj->ox = obj->px - cub->hero.px;
//		obj->oy = obj->py - cub->hero.py;
		
		odist = (*cub->hero.dirx) * ov[0] + (*cub->hero.diry) * ov[1];

		ratio = cub->near_z / odist;
		drawx = (int)(((*cub->hero.dirx) * ov[1] - (*cub->hero.diry) * ov[0])
			* ratio) + cub->scn_midx;

		dims[0] = (int)(ratio * obj->type->width);
		dims[1] = (int)(ratio * obj->type->height);

		if ((obj == link || odist <= 0 || pframe[2] <= drawx || (drawx + dims[0])
			< pframe[0]) && next_obj(&obj))
//		{
//			obj = obj->next;
			continue ;
//		}

		tex = obj->type->gset->xwalls[obj->tex_idx];
		tincrs[0] = (float)tex->width / (float)dims[0];
		tincrs[1] = (float)tex->height / (float)dims[1];
		toffs[0] = 0;
		toffs[1] = 0;
		
		loffs[0] = drawx - (dims[0] >> 1);
		loffs[1] = cub->scn_midy - (dims[1] >> 1) + (int)(obj->type->draw_offy * ratio);
		loffs[2] = loffs[0] + dims[0];
		loffs[3] = loffs[1] + dims[1];

		if (loffs[0] < pframe[0])
		{
			toffs[0] = pframe[0] - loffs[0];
			dims[0] -= toffs[0];
			loffs[0] = pframe[0];
		}
		if (loffs[1] < pframe[1])
		{
			toffs[1] = pframe[1] - loffs[1];
			dims[1] -= toffs[1];
			loffs[1] = pframe[1];
		}
		if (loffs[2] > pframe[2])
		{
			dims[0] -= (loffs[2] - pframe[2]);
			loffs[2] = pframe[2];
		}
		if (loffs[3] > pframe[3])
		{
			dims[1] -= (loffs[3] - pframe[3]);
			loffs[3] = pframe[3];
		}
		__render_proj_obj(cub, odist, tex, cub->hero.rcast.prtl_proj, dims, loffs, toffs, tincrs);
		obj = obj->next;
	}
}

void	__render_obj(t_cub *cub, int dist, mlx_texture_t *tex, t_rdata *rdata, int *dims,
		int *loffs, int *toffs, float *tex_incrs)//, int *end)
{
	int			toffs_x[SCN_WIDTH];
	int			pbuff_start_off;
	int			pbuff_jump;
	uint32_t	*tbuff;
	uint32_t	*tb;
	uint32_t	*pbuff;
	float		*dbuff;
	int			i;
	int			j;
	uint32_t	tex_col;

	rdata += loffs[0];
	i = 0;
	while (i < dims[0] && rdata[i].dist < dist)
		++i;
	toffs[0] += i;
	dims[0] -= i;
	rdata += i;
	j = -1;
	while (++j < dims[0] && dist <= rdata[j].dist)
//	{
		toffs_x[j] = (int)(j * tex_incrs[0]);
	if (j == 0)
		return ;
	dims[0] = j;
	loffs[0] += i;
	loffs[2] = loffs[0] + j;
	pbuff_start_off = loffs[0] + loffs[1] * SCN_WIDTH - 1;
	pbuff_jump = SCN_WIDTH - dims[0];

	tbuff = ((uint32_t *)tex->pixels) + (int)(toffs[0] * tex_incrs[0])
				+ ((int)(toffs[1] * tex_incrs[1]) * tex->width);
	dbuff = ((float *)cub->renderer.dbuff) + pbuff_start_off;// + (loffs[0]) + (loffs[1]) * SCN_WIDTH - 1;
	pbuff = ((uint32_t *)cub->renderer.objs_layer->pixels) + pbuff_start_off;// + (loffs[0]) + (loffs[1]) * SCN_WIDTH - 1;

	j = -1;
	while (++j < dims[1])
	{
		tb = tbuff + (int)(j * tex_incrs[1]) * tex->width;
		i = -1;
		while (++i < dims[0])
		{
			++dbuff;
			++pbuff;
			tex_col = tb[toffs_x[i]];
			if (!tex_col || (*dbuff && (*dbuff < dist)))
				continue ;
			*pbuff = tex_col;
			*dbuff = dist;
		}

		dbuff += pbuff_jump;		
		pbuff += pbuff_jump;
	}
}

void	__label_isproj(uint32_t *pbuff, char *isproj, int *pframe, int *pdims)
{
	const int		start_offset = pframe[0] + (pframe[1] * SCN_WIDTH);
	const int		buff_jump = SCN_WIDTH - pdims[0];
//	char			*ip;
//	uint32_t		*pb;
	int		w;
	int		h;

//	pb = pbuff + start_offset - 1;
//	ip = isproj + start_offset - 1;
	pbuff += start_offset - 1;
	isproj += start_offset - 1;
	h = pdims[1];
//	printf("label pframe [%d, %d], pdims [%d, %d]\n", pframe[0], pframe[1],
//		pdims[0], pdims[1]);
	while (h--)
	{
//		printf("h %d   - ", h);
//		ft_deltatime_usec_note("start label row");
//		printf("isproj off %zd, pbuff off %zd\n", (size_t)(ip - isproj),
//			(size_t)(pb - pbuff));
//		printf("isproj x : %zd\n", (ip - isproj) % SCN_WIDTH);
		w = pdims[0];
		while (w--)
		{
			++isproj;
			if (*(++pbuff) == PROJ_COLOR)
				*isproj = 1;
		}
//		ft_deltatime_usec_note("end label row");
		pbuff += buff_jump;
		isproj += buff_jump;			
	}
}

void	render_objects(t_cub *cub)
{
	t_oinst			*obj;
	mlx_texture_t	*tex;
	int				drawx;
	float			ratio;
	int				loffs[4];
	int				toffs[2];
	float			tincrs[2];
	int				dims[2];
	
	memset(cub->renderer.objs_layer->pixels, 0, sizeof(uint32_t) * SCN_WIDTH * SCN_HEIGHT);
	memset(cub->renderer.dbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);

	obj = cub->objs.instances;
	while (obj)
	{
		printf("obj->type : %p\n", obj->type);
		if ((!obj->type->is_drawable || !obj->type->gset) && next_obj(&obj))
		{
			if (obj)
				printf("BYPASS OBJ DRAW : id %d, type ptr : %p, drawable : %d, gset ptr : %p\n",
					obj->_id, obj->type, obj->type->is_drawable, obj->type->gset);
//			obj = obj->next;
			continue ;
		}
		printf("obj draw init checks PASSED \n");
		obj->ox = obj->px - cub->hero.px;
		obj->oy = obj->py - cub->hero.py;
		obj->dist = (*cub->hero.dirx) * obj->ox + (*cub->hero.diry) * obj->oy;
		tex = obj->type->gset->xwalls[obj->tex_idx];
		ratio = cub->near_z / obj->dist;
		dims[0] = (int)(ratio * obj->type->width);
		dims[1] = (int)(ratio * obj->type->height);
		drawx = (int)(((*cub->hero.dirx) * obj->oy - (*cub->hero.diry) * obj->ox)
			* ratio) + cub->scn_midx;

		if (((obj->dist <= 1) || (drawx + (dims[0] >> 1)) < 0
			|| SCN_WIDTH <= (drawx - (dims[0] >> 1))) && next_obj(&obj))
//		{
//			obj = obj->next;
			continue ;
//		}

		tincrs[0] = (float)tex->width / (float)dims[0];
		tincrs[1] = (float)tex->height / (float)dims[1];
		toffs[0] = 0;
		toffs[1] = 0;
		
		loffs[0] = drawx - (dims[0] >> 1);
		loffs[1] = cub->scn_midy - (dims[1] >> 1) + (int)(obj->type->draw_offy * ratio);
		loffs[2] = loffs[0] + dims[0];
		loffs[3] = loffs[1] + dims[1];

		if (loffs[0] < 0)
		{
			toffs[0] = -loffs[0];
			dims[0] += loffs[0];
			loffs[0] = 0;
		}
		if (loffs[1] < 0)
		{
			toffs[1] = -loffs[1];
			dims[1] += loffs[1];
			loffs[1] = 0;
		}
		if (loffs[2] > SCN_WIDTH)
		{
			dims[0] -= (loffs[2] - SCN_WIDTH);
			loffs[2] = SCN_WIDTH;
		}
		if (loffs[3] > SCN_HEIGHT)
		{
			dims[1] -= (loffs[3] - SCN_HEIGHT);
			loffs[3] = SCN_HEIGHT;
		}

		if (obj->type->type_enum == OBJ_PORTAL && obj->isactive)
		{
			printf("RENDERING PORTAL OBJECT WITH PROJ! from start %d to end %d\n", loffs[0], loffs[2]);
			
			__render_obj(cub, obj->dist, tex, cub->hero.rcast.rdata, dims, loffs, toffs, tincrs);
			int	*pframe = cub->renderer.pframe;
			int	pdims[2] = {(int)(obj->type->proj_width * ratio), (int)(obj->type->proj_height * ratio)};

			pframe[0] = drawx - (pdims[0] >> 1);
			pframe[2] = ft_clamp(pframe[0] + pdims[0], 0, SCN_WIDTH);
			pframe[0] = ft_clamp(pframe[0], 0, SCN_WIDTH);

			if ((pframe[0] == pframe[2]) && next_obj(&obj))
			{
				printf("BAILOUT ON PROJECTION !!\n");
//				obj = obj->next;
				continue ;
			}
			pframe[1] = cub->scn_midy - (pdims[1] >> 1)
				+ (ratio * obj->type->draw_offy);
			pframe[3] = ft_clamp(pframe[1] + pdims[1], 0, SCN_HEIGHT);
			pframe[1] = ft_clamp(pframe[1], 0, SCN_HEIGHT);
			pdims[0] = pframe[2] - pframe[0];
			pdims[1] = pframe[3] - pframe[1];

			memset(cub->renderer.dpbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);
			memset(cub->renderer.isproj, 0, sizeof(char) * SCN_WIDTH * SCN_HEIGHT);
			
			cub->renderer.portal = obj;
//			ft_deltatime_usec_note(NULL);
			__label_isproj((uint32_t *)cub->renderer.objs_layer->pixels, cub->renderer.isproj, pframe, pdims);
//			ft_deltatime_usec_note("__label_isproj time");
			prtl_proj_vectors(cub->hero.rcast.prtl_proj, &cub->map, obj, cub->renderer.pframe);
			order_draw_call(cub->draw_threads, 3, 5);
//			__render_proj_objects(cub);//, obj, cub->hero.rcast.prtl_proj, pframe);
//			__render_proj_walls(cub);//, cub->hero.rcast.prtl_proj, (uint32_t *)cub->renderer.objs_layer->pixels, pframe);
			__render_proj_floor(cub);
		}
		else
			__render_obj(cub, obj->dist, tex, cub->hero.rcast.rdata, dims, loffs, toffs, tincrs);

		obj = obj->next;
	}
}

