/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 10:21:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/12 02:40:55 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//static int	is_obj_in_fov(t_hero *hero, t_oinst *obj)
static int	is_point_in_fov(t_hero *hero, int x, int y)
{
//	return ((((*hero->fov_lx) * obj->oy + (*hero->fov_ly) * -obj->ox) > 0)
//		!= (((*hero->fov_rx) * obj->oy + (*hero->fov_ry) * -obj->ox) > 0));
	return ((((*hero->fov_lx) * y + (*hero->fov_ly) * -x) > 0)
		!= (((*hero->fov_rx) * y + (*hero->fov_ry) * -x) > 0));
}

/*
static int	prtl_init_single_vect(t_pdata *pd, const t_rdata *rd, t_oinst *obj)//, t_oinst *link)
{
	float	ray_scalar_to_obj;

//	printf("prtl_init_single_vect pd id : %d, rd id : %d\n", pd->rdata->idx, rd->idx);
	ray_scalar_to_obj = obj->dist * (*pd->fwd_len);
	pd->px = *rd->px;
	pd->py = *rd->py;
	pd->cx = *rd->pcx;
	pd->cy = *rd->pcy;
	pd->tgt_px = pd->px + (int)(ray_scalar_to_obj * (*rd->rx));// + (link->px - obj->px);
	pd->tgt_py = pd->py + (int)(ray_scalar_to_obj * (*rd->ry));// + (link->py - obj->py);
	pd->tgt_cx = (int)(pd->tgt_px * rd->inv_cw);
	pd->tgt_cy = (int)(pd->tgt_py * rd->inv_cw);
	pd->dist = 0;//= obj->dist;
//	printf(" - o_dist %f, fwd %f, ray_scalar %f, rx %f, ry %f, vx %f, vy %f",
//		obj->dist, (*pd->fwd_len), ray_scalar_to_obj, (*rd->rx), (*rd->ry),
//		ray_scalar_to_obj * (*rd->rx), ray_scalar_to_obj * (*rd->ry));
//	printf(" tgt_px %d, tgt_py %d, tgt_cx %d, tgt_cy %d\n",
//		pd->tgt_px, pd->tgt_py, pd->tgt_cx, pd->tgt_cy);
//	pd->dx = //(*rd->rx >= 0);
//	pd->dy = (*rd->ry >= 0);
//	pd->cincr_x = (rd->dx << 1) - 1;
//	pd->cincr_y = (rd->dy << 1) - 1;
//	pd->a = *rd->ry / *rd->rx;
//	pd->inv_a = 1.0f / rd->a;
//	pd->b = *rd->py - (rd->a * *rd->px);
	return (1);
}
*/
/*
static int	prtl_proj_init_single_vect(t_pdata *pd, t_rdata *rd, t_oinst *obj, t_oinst *link)
{
	float	ray_scalar_to_obj;

//	printf("pd id %d\n", pd->rdata->idx);
	ray_scalar_to_obj = obj->dist * (*pd->fwd_len);
//	printf("prtl_proj_init_single_vect, link %p\n", link);
	pd->dist = obj->dist;

/// EXTRAS
	pd->px = *rd->px + (int)(ray_scalar_to_obj * (*rd->rx)) + (link->px - obj->px);
	pd->py = *rd->py + (int)(ray_scalar_to_obj * (*rd->ry)) + (link->py - obj->py);

	pd->cx = (int)(pd->px * rd->inv_cw);
	pd->cy = (int)(pd->py * rd->inv_cw);

	rd->b = pd->py - (rd->a * pd->px);
	return (1);
}

*/
static int	prtl_proj_probe(t_pdata *pd, float *axs, float *isct, float *dists)
{
	const t_rdata	*rd = pd->rdata;
	float	ratio;
	int		correction;

	if (dists[0] < dists[1])//pd->dist == dists[0])
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
//	printf("probe :: ratio : %f, rd->inv_cw: %f\n", ratio, rd->inv_cw);
	pd->tex_ratio = correction + (ratio * rd->inv_cw);
	pd->tex_height = (*rd->near_proj_factor) / pd->dist;
//	printf("probe pd %d : dist %f, side %d, hit (%f, %f), ratio %f\n",
//		pd->rdata->idx, pd->dist, pd->side, pd->hitx, pd->hity, pd->tex_ratio);
	return (0);
}

/*
static int	proj_is_wall(t_map *map, int cx, int cy, int *bailout)
{
//	printf("proj is wall enter\n");
	*bailout = is_wall(map, cx, cy);
//	printf("proj is wall exit\n");
	return (*bailout);
}
*/
/**/
//////// prtl_proj_vector
/*
// Pass the t_pdata * offset in array to the starting ray to cast from 0 <= i < SCN_WIDTH.
// n is th nb of rays to cast from start (end - start).
static int	prtl_proj_vector(t_pdata *pd, t_map *map, t_oinst *obj, int *pframe)//, int n)///int start, int end)
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
//	printf("pframe [%d, %d] [%d, %d], width %d\n", pframe[0], pframe[1], pframe[2], pframe[3], width);
	while (width--)
	{
	//	printf("init single proj vect\n");
		prtl_proj_init_single_vect(pd, pd->rdata, obj, obj->link);
		while (!(is_wall(map, pd->cx, pd->cy) && prtl_proj_probe(pd, axs, isct, dists) == 0))
		{
			axs = map->grid_coords[pd->cy + rd->dy] + ((pd->cx + rd->dx) << 1);
			isct[1] = rd->a * axs[0] + rd->b;
			isct[0] = (axs[1] - rd->b) * rd->inv_a;
			dists[0] = (isct[0] - pd->px) * (*rd->p_dirx)
				+ (axs[1] - pd->py) * (*rd->p_diry);
			dists[1] = (axs[0] - pd->px) * (*rd->p_dirx)
				+ (isct[1] - pd->py) * (*rd->p_diry);
			if (dists[0] < dists[1])
				pd->cy += rd->cincr_y;
			else
				pd->cx += rd->cincr_x;
		}
		++pd;
		++rd;
	}
	return (0);
}


static inline uint32_t	*init_wcol(t_cub *cub, t_rdata *rd, t_rcol *rc, int *tw)
{
	mlx_texture_t	*tex;
	int				tex_start_x;

	//if (rd->cx < 0 || rd->cy < 0 || rd->cx >= cub->map.width || rd->cy >= cub->map.height)
//	printf("ABÉRATION ! cx, cy : %d, %d\n", rd->cx, rd->cy);
	tex = cub->map.mx[rd->cy][rd->cx]->xwalls[rd->side];
//	tex = cub->tex.walls[rd->side];
	*tw = tex->width;
	rc->half_texh = (tex->height >> 1);
	tex_start_x = (int)(rd->tex_ratio * tex->width);
	rc->scn_height = ft_clamp(rd->tex_height, 0, SCN_HEIGHT);
	rc->half_height = (rc->scn_height >> 1);
	rc->ratio = (float)tex->height / (float)rd->tex_height;
	rc->scn_start_y = ((SCN_HEIGHT - rc->scn_height) >> 1);
	return ((uint32_t *)tex->pixels + tex_start_x);
}

// rd is ptr to array of raycasting results data (array len = SCN_WIDTH).
void	render_walls(t_cub *cub, t_rdata *rd)
{
	int			i;
	int			j;
	t_rcol		rc;
	uint32_t	*pxls;
	int			tex_width;

	clear_image_buffer(cub->renderer.walls_layer);
	rc.walls_layer = cub->renderer.walls_layer;
	i = -1;
	while (++i < SCN_WIDTH)
	{
		pxls = init_wcol(cub, rd + i, &rc, &tex_width);
		j = -1;
		while (++j < rc.scn_height)
			cub_put_pixel(rc.walls_layer, i, rc.scn_start_y + j,
				pxls[(int)(((j - rc.half_height) * rc.ratio)
					+ rc.half_texh) *tex_width]);
	}
	cub->renderer.requires_update = 0;
}
*/

static inline uint32_t	*init_proj_wcol(t_cub *cub, t_pdata *pd, t_rcol *rc, int *tshape, int max_h)//, int *start_x)
{
	mlx_texture_t	*tex;
	int				tex_start_x;

	//if (rd->cx < 0 || rd->cy < 0 || rd->cx >= cub->map.width || rd->cy >= cub->map.height)
//	printf("ABÉRATION ! cx, cy : %d, %d\n", rd->cx, rd->cy);
//	printf("pd dist : %f, cx %d, cy %d\n", pd->dist, pd->cx, pd->cy);

//	else if (pd->dist < 0.0f)
//		printf("proj raycasting has negative dist ?! %f\n", pd->dist);
//	printf("tex at cx (%d), cy (%d) : %p\n", pd->cx, pd->cy, cub->map.mx[pd->cy][pd->cx]);
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
	rc->scn_start_y = ((SCN_HEIGHT - rc->scn_height) >> 1);
	rc->scn_end_y = rc->scn_start_y + rc->scn_height;
//	printf("init proj wcol : proj col %p, tex_start_x %d, tex_width %d, scn_height %d, ratio %f, scn_start_y %d\n", 
//		(void *)(size_t)cub->objs.portal.proj_clr, tex_start_x, *tw, rc->scn_height, rc->ratio, rc->scn_start_y);
//	*start_x = tex_start_x;
//	printf("tex_start_x : %d\n", tex_start_x);
//	printf("tex ptr : %p, tex_start_x %d, tex->pixels %p\n", tex, tex_start_x, tex->pixels);
	return ((uint32_t *)tex->pixels + tex_start_x);// + ((int)(rc->scn_start_y * rc->ratio) * tex->width));
}

/*
static void	__render_portal_projection(t_cub *cub, t_pdata *pd, int *start, int *end)
{
//	const uint32_t	pcol = cub->objs.portal.proj_clr;
	int			i;
	int			j;
	t_rcol		rc;
//	uint32_t	*lpxs;
	uint32_t	*pxls;
	uint32_t	*pl;
	uint32_t	*layer_pxls;
	int			tex_width;
	int			xspan;
	uint32_t	tex_col;
//	int			yspan;

	ft_eprintf("RENDER PORTAL PROJECTION ! from [%d, %d] to [%d, %d]\n", start[0], start[1], end[0], end[1]);
//	rc.layer = cub->renderer.objs_layer;
//	clear_image_buffer(rc.layer);
	xspan = end[0] - start[0];
	ft_eprintf("xspan : %d, yspan :%d\n", xspan, end[1] - start[1]);
//	yspan = end[1] - start[1];
//	lpxs = (uint32_t *)rc.layer->pixels;
	layer_pxls = (uint32_t *)rc.layer;
	i = -1;//start[0] - 1;
	while (++i < xspan)//end[0])
	{
		pxls = init_proj_wcol(cub, pd + start[0] + i, &rc, &tex_width, end[1] - start[1]);
		if (!pxls)
			continue ;
		j = -1;//start[1] - 1;
		while (++j < rc.scn_height)// && rc.layer->pixels[(start[0] + i) + (start[1] + j) * SCN_WIDTH] == cub->objs.portal.proj_clr)//end[1])
		{
		//	printf("render proj i %d j %d\n", i, j);
//			if (i == (xspan >> 1) && j == (yspan >> 1))
//				printf("buf vs pcol at (%d, %d) : %p vs %p \n",
//					start[0] + i, cub->scn_midy,
//					(void *)(size_t)lpxs[(start[0] + i) + SCN_WIDTH * cub->scn_midy], (void*)(size_t)pcol);
	//		if (lpxs[(start[0] + i) + (start[1] + j) * SCN_WIDTH] == pcol)
	//			printf("WOWOW : ref color FOUND !\n");
//			printf("sx, sy : %d, %d\n", start[0] + i, rc.scn_start_y + j);
//			if (start[0] + i > (int)rc.layer->width || rc.scn_start_y + j > (int)rc.layer->height
//				|| start[0] + i < 0 || rc.scn_start_y + j < 0)
//				ft_eprintf("WOW drawing out of obj layer ! (x, y) : (%d, %d)\n", start[0] + i, rc.scn_start_y + j);
//			tex_col = pxls[(uint32_t)(((j - rc.half_height) * rc.ratio) + rc.half_texh) * tex_width];
			layer_pxls = rc.layer + (start[0] + i) + (rc.scn_start_y + j) * SCN_WIDTH;
			pl = pxls + (uint32_t)(((j - rc.half_height) * rc.ratio) + rc.half_texh) * tex_width;
			if (!tex_col)
				cub_put_pixel(rc.layer, start[0] + i, rc.scn_start_y + j,//start[1] + j,
					tex_col);//pxls[tbuff_offset]);//(int)(((j - rc.half_height) * rc.ratio) + rc.half_texh) * tex_width]);
		}
	}
//	cub->renderer.requires_update = 0;
}
*/
/*
// plims is the bounding box for the verified projection area. [min x, min y, max x, max y]
// pdims[0] = lims[2]
void	__render_sky_proj(t_cub *cub, int *plims, int *pdims)//, t_pdata *pd)
{
	int			texture_xoffsets[SCN_WIDTH];
	uint32_t	*pxls;
	int			*tofs[2];
	int			x;
	int			y;

//	(void)rd;
	tofs[0] = texture_xoffsets + lims[0] - 1;
	x = plims[0] - 1;//-1;
	while (++x < plims[2])// SCN_WIDTH)
		*(++tofs[0]) = (int)((x - cub->scn_midx) * cub->inv_sw
				* cub->renderer.sky_fov_to_tex
				+ cub->renderer.sky_ori_offset) % cub->tex.skymap->width;
	pxls = (uint32_t *)cub->renderer.bg_layer->pixels;
	tofs[1] = cub->renderer.sky_yoffsets - 1;
	y = plims[1] - 1;// -1;
	while (++y < cub->scn_midy)
	{
	//	if (tofs[] == 0xffbcbbb0)
		tofs[0] = texture_xoffsets - 1;
		++tofs[1];
		x = -1;
		while (++x < SCN_WIDTH)
			*(++pxls) = ((uint32_t *)cub->tex.skymap->pixels)[*(++tofs[0])
				+ (*tofs[1]) * cub->tex.skymap->width];
	}
}
*/
static int	prtl_proj_init_single_vect(t_pdata *pd, t_rdata *rd, t_oinst *obj, t_oinst *link)
{
//	float	ray_scalar_to_obj;

//	printf("pd id %d\n", pd->rdata->idx);
//	ray_scalar_to_obj = obj->dist * (*pd->fwd_len);
//	printf("prtl_proj_init_single_vect, link %p\n", link);
	pd->dist = obj->dist;

	pd->px = *rd->px + pd->dist * (*rd->rx) + (link->px - obj->px);
	pd->py = *rd->py + pd->dist * (*rd->ry) + (link->py - obj->py);
/// EXTRAS
//	pd->px = *rd->px + (int)(ray_scalar_to_obj * (*rd->rx)) + (link->px - obj->px);
//	pd->py = *rd->py + (int)(ray_scalar_to_obj * (*rd->ry)) + (link->py - obj->py);
	pd->cx = (int)(pd->px * rd->inv_cw);
	pd->cy = (int)(pd->py * rd->inv_cw);

	pd->b = pd->py - (rd->a * pd->px);
	return (1);
}

//////// prtl_proj_vector

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
//	printf("pframe [%d, %d] [%d, %d], width %d\n", pframe[0], pframe[1], pframe[2], pframe[3], width);
	while (width--)
	{
	//	printf("init single proj vect\n");
		prtl_proj_init_single_vect(pd, pd->rdata, obj, obj->link);
		while (!(is_wall(map, pd->cx, pd->cy) && prtl_proj_probe(pd, axs, isct, dists) == 0))
		{
			axs = map->grid_coords[pd->cy + rd->dy] + ((pd->cx + rd->dx) << 1);
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
		}
		++pd;
		++rd;
	}
	return (0);
}

void	__render_proj_walls(t_cub *cub, t_pdata *pdata, uint32_t *pbuff, int *pframe)
{
//	const float		pheight_inv = 1.0f / ((float)(pframe[3] - pframe[1]));
//	const float		pwidth_inv = 1.0f / ((float)(pframe[2] - pframe[0]));
//	const int		pframe_mids[2] = {(pframe[2] - pframe[0]) >> 1, (pframe[3] - pframe[1]) >> 1};
	const int		proj_height = pframe[3] - pframe[1];
	
//	int			j_times_sw[SCN_HEIGHT];
	
	int			i;
//	int			ref_i;
	int			j;
	t_pdata		*pd;// pdata ptr
//	uint32_t	*dbuff;// world depth
	float		*dpbuff;
	uint32_t	*pb;// projection buffer ptr;
	t_rcol		rc;

	int			tex_shape[2];
//	int			tex_width;

//	mlx_texture_t	*tex;
//	uint32_t		tex_col;
	float			tex_y;
	uint32_t	*tex_buff;
//	uint32_t	*tb;

	char			*isproj;

//	ft_eprintf("wow");
//	float		divergent_lens_ratio[SCN_HEIGHT];
//	int			j_to_midy[SCN_HEIGHT];
/*
	j = pframe[1] - 1;
	while (++j < pframe[3])
	{
		j_to_midy[j] = j - cub->scn_midy;
		divergent_lens_ratio[j] = cosf(j_to_midy[j] * pheight_inv * LENS_EFFECT_RAD);
	}
*/
//	j = pframe[1] - 1;
//	while (++j < pframe[3])
//	{
//		j_times_sw[j] = j * SCN_WIDTH;
//	}

//	pbuff += pframe[0] - 1;
	i = pframe[0] - 1;
	while (++i < pframe[2])
	{
//		++pd;
//		dbuff = cub->renderer.dbuff + scn_offx * SCN_HEIGHT;
//		ref_i = i + (int)((i - pframe_mids[0]) * cosf((i - pframe_mids[0]) * pwidth_inv * LENS_EFFECT_RAD));
//		printf("ref_i start = %d\n", ref_i);
//		if (ref_i < 0)
//			ref_i = 0;
//		else if (ref_i >= SCN_WIDTH)
//			ref_i = SCN_WIDTH - 1;
//		printf("ref_i after = %d\n", ref_i);
		pd = pdata + i;
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
		isproj = cub->renderer.isproj + i + cub->buff_offys[j] - SCN_WIDTH;
		dpbuff = cub->renderer.dpbuff + i + cub->buff_offys[j] - SCN_WIDTH;
//		pb = pbuff + i + cub->buff_offys[j] - SCN_WIDTH;
		while (++j < rc.scn_end_y)//pframe[3])
		{
			isproj += SCN_WIDTH;
			dpbuff += SCN_WIDTH;
//			pb += SCN_WIDTH;
//			pb = pbuff + j * SCN_WIDTH;//cub->buff_offys[j];
//			isproj += cub->buff_offys[j];
//			dpbuff += cub->buff_offys[j];
//			tb = tex_buff + ();
//			isproj = cub->renderer.isproj + i + cub->buff_offys[j];//j * SCN_WIDTH;
//			dpbuff = cub->renderer.dpbuff + i + cub->buff_offys[j];
//			isproj = cub->renderer.isproj + i + cub->buff_offys[j]; //j * SCN_WIDTH;
//			dpbuff = cub->renderer.dpbuff + i + cub->buff_offys[j];//j * SCN_WIDTH;


//			ray_scalar = params[i + (j - cub->scn_midy) * SCN_WIDTH]//(*pms) - pd->odist;
//				* cosf(divergent_lens_ratio * LENS_EFFECT_RAD) - pd->odist;

//			dpbuff += SCN_WIDTH;
//			isproj += SCN_WIDTH;
//			printf("is_proj ? %d\n", *isproj);
//			if (*isproj)
//				printf("(%d, %d), isproj : %d, dpbuff dist : %f vs %f\n", i, j, *isproj, *dpbuff, pd->dist);
			if (!*isproj || (*dpbuff && *dpbuff < pd->dist))
				continue ;
//			printf("gogo");
//			tex_y = (int)(((j - pframe[1]) - cub->scn_midy) * rc.ratio) + rc.half_texh;//(int)(j_to_midy[j] * divergent_lens_ratio[j] * rc.ratio) + rc.half_texh;
			tex_y = (j - cub->scn_midy) * rc.ratio + rc.half_texh;//(int)(j_to_midy[j] * divergent_lens_ratio[j] * rc.ratio) + rc.half_texh;
	//		printf("tex_y : %f, tex height %d\n", tex_y, tex_shape[1]);
//			if (tex_y < 0 || tex_shape[1] <= tex_y)
//				continue ;
//			printf("j_to_midy %d, diverge lens ratio : %f, rc,ratio : %f, tex_y : %d\n", 
//				j_to_midy[j], divergent_lens_ratio[j], rc.ratio, tex_y);
//			if (tex_y < 0 || tex_shape[1] <= tex_y)
//				continue ;
//			printf("tex_y : %d\n", tex_y);
//			printf("tex_buff[i (%d) + tex_y (%d) * tex_width (%d)] = %d\n", i, tex_y, tex_shape[0],
//				i + tex_y * tex_shape[0]);
//			printf("i, j : (%d, %d), tex_buff : %p,  tex_y (%d) * tex_width (%d)\n", i, j, tex_buff, tex_y, tex_shape[0]);
	//		tex_col = tex_buff[tex_y * tex_shape[0]];//tex_shape[0]];
//			tex_col = tex_buff[((int)(j_to_midy[j] * divergent_lens_ratio[j]
//				* rc.ratio) + rc.half_texh) * tex_width] & 0xafffffff;
			//tex_col = //ppxs[((int)((scn_y - cub->scn_midy) * rc.ratio) + rc.half_texh) * ptex_width]
//			pb[j * SCN_WIDTH] = tex_col;
//			pb[j_times_sw[j]] = //tex_buff[tex_y * tex_shape[0]];
//			pb[cub->buff_offys[j]] = tex_col;
//			*pb = tex_col;
//			pbuff[i + j * SCN_WIDTH] = tex_col;
			pb[cub->buff_offys[j]] = tex_buff[(int)tex_y * tex_shape[0]] & TRANSPARENCY;//tex_col;
//			*pb = tex_buff[(int)tex_y * tex_shape[0]];//tex_col;
//			cub_put_pixel(cub->renderer.objs_layer, scn_offx, loffs_y[j],
//				tex_col);
			*dpbuff = pd->dist;
		}
	}
}


int	__render_portal_empty(t_cub *cub, int dist, mlx_texture_t *tex, t_rdata *rd, int *dims,
		int *loffs, int *toffs, float *tex_incrs, int *pframe)//, int *end)
{
	float		*dbuff = cub->renderer.dbuff;
	char		*isproj = cub->renderer.isproj;
	int			i;
	int			j;
	uint32_t	*pxls;
	uint32_t	tex_col;
	int			scn_offx;
//	int			offys[SCN_HEIGHT];
//	int			buff_offys[SCN_HEIGHT];
//	int			pframe[4];

	pframe[0] = INT_MAX;
	pframe[1] = INT_MAX;
	pframe[2] = 0;
	pframe[3] = 0;

//	j = -1;
//	while (++j < dims[1])
//	{
//		offys[j] = (j + loffs[1]);
//		buff_offys[j] = offys[j] * SCN_WIDTH;
//	}

	pxls = (uint32_t *)tex->pixels;
	i = -1;
	while (++i < dims[0])
	{
		scn_offx = i + loffs[0];

		if (rd[scn_offx].dist < dist)
			continue ;
		dbuff = cub->renderer.dbuff + scn_offx + (loffs[1] * SCN_WIDTH) - SCN_WIDTH;// * SCN_HEIGHT;
		isproj = cub->renderer.isproj + scn_offx + (loffs[1] * SCN_WIDTH) - SCN_WIDTH;
		j = -1;
		
		while (++j < dims[1])
		{
			dbuff += SCN_WIDTH;
			isproj += SCN_WIDTH;
			tex_col = pxls[(int)((i + toffs[0]) * tex_incrs[0])
				+ (int)((j + toffs[1]) * tex_incrs[1]) * tex->width];
			if (!tex_col || (*dbuff && dist > *dbuff))
				continue ;
			if (tex_col == 0xffbcbbb0)
			{
				if (i < pframe[0])
					pframe[0] = i;
				if (j < pframe[1])
					pframe[1] = j;
				if (i > pframe[2])
					pframe[2] = i;
				if (j > pframe[3])
					pframe[3] = j;
				*isproj = 1;
				//isproj[buff_offys[j]] = 1;
//				cub->renderer.isproj[scn_offx + buff_offy] = 1;
			}
			cub_put_pixel(cub->renderer.objs_layer, scn_offx, j + loffs[1],
				tex_col);
			*dbuff = dist;
//			dbuff[buff_offys[j]] = dist;
		}
	}
	if (pframe[0] == INT_MAX)
		return (-1);
	pframe[0] += loffs[0];
	pframe[1] += loffs[1];
	pframe[2] += loffs[0];
	pframe[3] += loffs[1];
//	printf("pframe apres : min x, y : (%d, %d), max x, y, (%d, %d)\n", pframe[0], pframe[1], 
//		pframe[2], pframe[3]);
	return (0);
}

/*
static inline void	obj_put_pixel(mlx_image_t *img, int x, int y, uint32_t col)
{
	if (col)
		((uint32_t *)img->pixels)[x + y * img->width] = col;
}
*/
void	__render_obj(t_cub *cub, int dist, mlx_texture_t *tex, t_rdata *rd, int *dims,
		int *loffs, int *toffs, float *tex_incrs)//, int *end)
{
	int		toffs_y[SCN_HEIGHT];
	int		loffs_y[SCN_HEIGHT];
	float		*dbuff;
	int			i;
	int			j;
	int			scn_offx;
	int			tex_offx;
	uint32_t	*pxls;
	uint32_t	tex_col;

//	printf("dpbuff - dbuff : %ld\n", (cub->renderer.dpbuff - cub->renderer.dbuff) / (SCN_WIDTH * sizeof(float)));
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
		if (rd[i + loffs[0]].dist < dist)
			continue ;
		scn_offx = i + loffs[0];
		dbuff = cub->renderer.dbuff + scn_offx + (loffs[1] - 1) * SCN_WIDTH;
		tex_offx = (int)((i + toffs[0]) * tex_incrs[0]);

		j = -1;
		while (++j < dims[1])
		{
			dbuff += SCN_WIDTH;
//			scn_offs[1] = j + loffs[1];
			tex_col = pxls[tex_offx + toffs_y[j]];//(int)(toffs_y[j] * tex_incrs[1]) * tex->width];
//			++dbuff;
			if (!tex_col || (*dbuff && dist > *dbuff))
				continue ;
			cub_put_pixel(cub->renderer.objs_layer, scn_offx, loffs_y[j], tex_col);
			*dbuff = dist;
	//		dbuff[i + j * SCN_WIDTH] = dist;
		}
	}
}

void	render_objects(t_cub *cub, t_rdata *rd)
{
	t_oinst		*obj;
	int			drawx;
	float		ratio;
//	int	i;
	float		rightwardx_halfw;
	float		rightwardy_halfw;
//	int			scn_width;
//	int			scn_height;
//	float		tex_incrx;
//	float		tex_incry;
//	int			tex_offx;
//	int			tex_offy;
///	int			start[2];
//	int			end[2];
//	float	wtos;// world obj width to screen obj width;
//	uint32_t	*pxls;
//	int	i;
//	int	j;
	mlx_texture_t	*tex;

	int		loffs[4];
	int		toffs[2];
	float	tincrs[2];
	int		dims[2];
	int		pframe[4];

	(void)rd;
	clear_image_buffer(cub->renderer.objs_layer);
	memset(cub->renderer.dbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);
	memset(cub->renderer.dpbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);
	memset(cub->renderer.isproj, 0, sizeof(char) * SCN_WIDTH * SCN_HEIGHT);

	obj = cub->objs.instances;
	while (obj)
	{
//		printf("\n\n Rendering obj %p\n", obj);
		obj->ox = obj->px - cub->hero.px;
		obj->oy = obj->py - cub->hero.py;
		
		obj->dist = (*cub->hero.dirx) * obj->ox + (*cub->hero.diry) * obj->oy;
//		obj->ux = obj->ox / obj->dist;
//		obj->uy = obj->oy / obj->dist;
		rightwardx_halfw = (*cub->hero.diry) * obj->type->half_w;
		rightwardy_halfw = (*cub->hero.dirx) * obj->type->half_w;
		obj->ox_left = obj->ox - rightwardx_halfw;//(*cub->hero.diry) * obj->type->half_w;//obj->px - obj->uy;
		obj->oy_left = obj->oy + rightwardy_halfw;//(*cub->hero.dirx) * obj->type->half_w;//obj->py + obj->ux;
		obj->ox_right = obj->ox + rightwardx_halfw;//*cub->hero.diry) * obj->type->half_w;
		obj->oy_right = obj->oy - rightwardy_halfw;//(*cub->hero.dirx) * obj->type->half_w;
//		obj->ox_right = obj->px + obj->uy;
//		obj->oy_right = obj->py - obj->ux;
//		printf("(ox, oy) : (%f, %f), (lx, ly) : (%f, %f), (rx, ry) : (%f, %f)\n",
//			obj->ox, obj->oy,
//			obj->ox - (*cub->hero.diry) * obj->type->width,
//			obj->oy + (*cub->hero.dirx) * obj->type->width,
//			obj->ox + (*cub->hero.diry) * obj->type->width,
//			obj->oy - (*cub->hero.dirx) * obj->type->width);

		if ((obj->dist <= 0)
			|| !(is_point_in_fov(&cub->hero, obj->ox, obj->oy)
			|| is_point_in_fov(&cub->hero, obj->ox_left, obj->oy_left)
//				obj->ox - (*cub->hero.diry) * obj->type->half_w,
//				obj->oy + (*cub->hero.dirx) * obj->type->half_w)
			|| is_point_in_fov(&cub->hero, obj->ox_right, obj->oy_right)))
//				obj->ox + (*cub->hero.diry) * obj->type->half_w,
//				obj->oy - (*cub->hero.dirx) * obj->type->half_w)))
		{
//			printf("BAILOUT\n");
			obj = obj->next;
			continue ;
		}
//		printf("DRAW\n");
	//	drawx = (*cub->hero.diry) * obj->ox
	//		- (*cub->hero.dirx) * obj->oy
	//		+ cub->scn_midx;
	//	drawx = cub->near_proj_factor / u_proj;
		ratio = cub->near_z / obj->dist;
		drawx = (int)(((*cub->hero.dirx) * obj->oy - (*cub->hero.diry) * obj->ox)
			* ratio) + cub->scn_midx;

//		printf("mid 1\n");	
		dims[0] = (int)(ratio * obj->type->width);
		dims[1] = (int)(ratio * obj->type->height);
		tex = obj->type->texs[obj->tex_idx];
		tincrs[0] = (float)tex->width / (float)dims[0];
		tincrs[1] = (float)tex->height / (float)dims[1];
//		tex_offx = 0;
//		tex_offy = 0;
		toffs[0] = 0;
		toffs[1] = 0;
		
		loffs[0] = drawx - (dims[0] >> 1);
		loffs[1] = cub->scn_midy - (dims[1] >> 1);
		loffs[2] = loffs[0] + dims[0];
		loffs[3] = loffs[1] + dims[1];
//		start[0] = drawx - (scn_width >> 1);//scn_halfw;
//		start[1] = cub->scn_midy - (scn_height >> 1);//scn_halfh;
//		end[0] = start[0] + scn_width;//drawx + scn_halfw;
//		end[1] = start[1] + scn_height;//cub->scn_midy + scn_halfh;

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
		if (loffs[2] >= SCN_WIDTH)
		{
			dims[0] -= (loffs[2] - SCN_WIDTH);// * tex_incx;
			loffs[2] = SCN_WIDTH;
		}
		//	pxls -= (end[0] - SCN_WIDTH) * tex_incrx;
		if (loffs[3] >= SCN_HEIGHT)
		{
			dims[1] -= (loffs[3] - SCN_HEIGHT);// * tex_incy;
			loffs[3] = SCN_HEIGHT;
		}

		if (obj->type->type_enum == OBJ_PORTAL && obj->isactive)
		{
//			printf("RENDERING PORTAL OBJECT WITH PROJ! from start %d to end %d\n", loffs[0], loffs[2]);
		//	if (prtl_proj_vectors(cub->hero.rcast.prtl_proj + loffs[0],
		//		&cub->map, obj);//, dims[0]))
			//	__render_portal_projection(cub, cub->hero.rcast.prtl_proj, start, end);
	//		prtl_proj_vectors(cub->hero.rcast.prtl_proj + loffs[0], &cub->map, obj);
		//	__render_portal_with_projection(cub, obj->dist, obj, tex, cub->hero.rcast.prtl_proj, dims, loffs, toffs, tincrs);
			if (__render_portal_empty(cub, obj->dist, tex, cub->hero.rcast.rdata, dims, loffs, toffs, tincrs, pframe) < 0)
			{
				obj = obj->next;
				continue ;
			}
//			printf("proj vectors\n");
			prtl_proj_vectors(cub->hero.rcast.prtl_proj, &cub->map, obj, pframe);
			render_floor_sky_proj(cub, (uint32_t *)cub->renderer.objs_layer->pixels, cub->hero.rcast.prtl_proj, pframe);
			__render_proj_walls(cub, cub->hero.rcast.prtl_proj, (uint32_t *)cub->renderer.objs_layer->pixels, pframe);
//			printf("render_floor_sky\n");
//			else
//			{
//				printf("prtl_proj continue \n");
//				obj = obj->next;
//				continue ;
//			}
		}
		else
//		void	__render_obj(t_cub *cub, int dist, mlx_texture_t *tex, t_rdata *rd, int *dims,
//				int *start, int *tex_offs, int *tex_incrs)//, int *end)
			__render_obj(cub, obj->dist, tex, cub->hero.rcast.rdata, dims, loffs, toffs, tincrs);
			//printf("RENDERING PORTAL OBJECT WITHOUT PROJ!\n");


/*
		printf("Drawing Portal frame\n");


		uint32_t	*obj_lyr_pix_p;// = cub->renderer.objs_layer + (j + start[0]) + (i + start[1]) * cub->renderer.objs_layer->width
		uint32_t	tex_col;
		uint32_t	proj_col;
		
		

		j = -1;
		while (++j < scn_width)
		{
			if (rd[j + start[0]].dist < obj->dist)
			{
//				printf("walled at j = %d, ", j);
				continue ;
			}
//			if (obj->type->type_enum == OBJ_PORTAL && obj->isactive)
//				prtl_proj_vectors(cub->hero.rcast.prtl_proj + start[0] + j,
//					&cub->map, obj);
		//	else
				//printf("scn_height : %d\n", scn_height);
		//		printf("type->width, height : %d, %d\n", obj->type->width, obj->type->height);
			i = -1;
			while (++i < scn_height)// && ((u_proj < rd[j + start[0]].dist)
			//	&& (cub->renderer.objs_layer->pixels[j * start[0]
			//		+ i * start[1] * SCN_WIDTH] >> 24) == 0))
			{
//				obj_lyr_pix_p = cub->renderer.objs_layer + (j + start[0]) + (i + start[1]) * cub->renderer.objs_layer->width
//				printf("put tex px (%d, %d) at scn coord (%d, %d).\n", (int)(j * tex_incrx),
//					(int)(i * tex_incry), j + start[0], i + start[1]);
				//cub_put_pixel(cub->renderer.objs_layer, j + start[0], i + start[1],
				obj_put_pixel(cub->renderer.objs_layer, j + start[0], i + start[1],
					((uint32_t *)tex->pixels)[
						(int)((j + tex_offx) * tex_incrx)
						+ (int)((i + tex_offy) * tex_incry) * tex->width]);
			//		pxls[(int)(j * tex_incrx + i * tex_incry * tex->width)]);
			
				//	pxls[(int)(j * tex_incrx + i * tex_incry * obj->type->texs[obj->tex_idx]->width)]);
			//		get_texture_pixel(tex,
			//			(int)((j + tex_offx) * tex_incrx),
			//			(int)((i + tex_offy) * tex_incry)));
			//	rd[j + start[0]].dist = u_proj;
			}
		//	pxls += (int)(tex_incry * tex->width);
		//	++start[0];
		//	++start[1];
		}
			
		//printf("draw obj at scn col : %d, u_proj : %f, (ox, oy) : (%f, %f), scn w/h : %f, %f\n",
		//	(int)drawx, u_proj, obj->ox, obj->oy, scn_width, scn_height);
*/
		obj = obj->next;
	}
}

