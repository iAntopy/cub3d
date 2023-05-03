/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 10:21:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/03 00:24:46 by iamongeo         ###   ########.fr       */
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

static int	prtl_proj_init_single_vect(t_pdata *pd, t_rdata *rd, t_oinst *obj, t_oinst *link)
{
	float	ray_scalar_to_obj;

	ray_scalar_to_obj = obj->dist * (*pd->fwd_len);
//	printf("prtl_proj_init_single_vect, link %p\n", link);
	pd->dist = obj->dist;

/// EXTRAS
	pd->px = *rd->px + (int)(ray_scalar_to_obj * (*rd->rx)) + (link->px - obj->px);
	pd->py = *rd->py + (int)(ray_scalar_to_obj * (*rd->ry)) + (link->py - obj->py);
//	pd->py = pd->tgt_py + (link->py - obj->py);
///
	
//	pd->px = pd->tgt_px + (link->px - obj->px);
//	pd->py = pd->tgt_py + (link->py - obj->py);
	pd->cx = (int)(pd->px * rd->inv_cw);
	pd->cy = (int)(pd->py * rd->inv_cw);
//	printf("Init projected ray : (px py) : (%d %d), (cx cy) : (%d %d) \n", pd->px, pd->py, pd->cx, pd->cy);
//	pd->cx = pd->tgt_cx;
//	pd->cy = pd->tgt_cy;
//	pd->tgt_px = ray_scalar_to_obj * (*rd->rx);// + (link->px - obj->px);
//	pd->tgt_py = ray_scalar_to_obj * (*rd->ry);// + (link->py - obj->py);
///à	pd->tgt_cx = (int)(pd->tgt_px * rd->inv_cw);
//	pd->tgt_cy = (int)(pd->tgt_py * rd->inv_cw);
//	pd->dx = //(*rd->rx >= 0);
//	pd->dy = (*rd->ry >= 0);
//	pd->cincr_x = (rd->dx << 1) - 1;
//	pd->cincr_y = (rd->dy << 1) - 1;
//	pd->a = *rd->ry / *rd->rx;
//	pd->inv_a = 1.0f / rd->a;
	rd->b = pd->py - (rd->a * pd->px);
	return (1);
}

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
// Pass the t_pdata * offset in array to the starting ray to cast from 0 <= i < SCN_WIDTH.
// n is th nb of rays to cast from start (end - start).
static int	prtl_proj_vector(t_pdata *pd, t_map *map, t_oinst *obj)//, int n)///int start, int end)
{
	t_rdata	*rd;
	float	*axs;
	float	isct[2];
	float	dists[2];

//	printf("Portal Projecting \n");
	rd = pd->rdata;// - 1;

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
	return (0);//render_proj);
}
/*
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
static inline uint32_t	*init_proj_wcol(t_cub *cub, t_pdata *pd, t_rcol *rc, int *tw, int max_h)
{
	mlx_texture_t	*tex;
	int				tex_start_x;

	//if (rd->cx < 0 || rd->cy < 0 || rd->cx >= cub->map.width || rd->cy >= cub->map.height)
//	printf("ABÉRATION ! cx, cy : %d, %d\n", rd->cx, rd->cy);
//	printf("pd dist : %f, cx %d, cy %d\n", pd->dist, pd->cx, pd->cy);
	if ((int)pd->dist == 0)
		return (NULL);
	tex = cub->map.mx[pd->cy][pd->cx]->xwalls[pd->side];
//	tex = cub->tex.walls[rd->side];
	*tw = tex->width;
	rc->half_texh = (tex->height >> 1);
	tex_start_x = (int)(pd->tex_ratio * tex->width);
	if (max_h > SCN_HEIGHT)
		max_h = SCN_HEIGHT;
	rc->scn_height = ft_clamp(pd->tex_height, 0, max_h);
	rc->half_height = (rc->scn_height >> 1);
	rc->ratio = (float)tex->height / (float)pd->tex_height;
	rc->scn_start_y = ((SCN_HEIGHT - rc->scn_height) >> 1);
//	printf("init proj wcol : proj col %p, tex_start_x %d, tex_width %d, scn_height %d, ratio %f, scn_start_y %d\n", 
//		(void *)(size_t)cub->objs.portal.proj_clr, tex_start_x, *tw, rc->scn_height, rc->ratio, rc->scn_start_y);

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
static void	__render_portal_with_projection(t_cub *cub, int dist, t_oinst *obj, mlx_texture_t *tex,
		t_pdata *pd, int *dims, int *loffs, int *toffs, float *tincrs)
//static void	__render_portal_with_projection(t_cub *cub, t_pdata *pd, int *start, int *end)
{
//	const uint32_t	pcol = cub->objs.portal.proj_clr;
	int			i;
	int			j;
	t_rcol		rc;
	uint32_t	*lpxs;// layer pixel array
	uint32_t	*tpxs;// obj tex pixel array
	uint32_t	*ppxs;// projection wall pixel array
//	uint32_t	*pl;
//	uint32_t	*layer_pxls;
	int			tex_width;
//	int			xspan;
	uint32_t	tex_col;
//	int			yspan;

//	ft_eprintf("RENDER PORTAL PROJECTION ! from [%d, %d] to [%d, %d]\n", start[0], start[1], end[0], end[1]);
	rc.layer = cub->renderer.objs_layer;
	pd += loffs[0];
	tpxs = (uint32_t *)tex->pixels;
//	xspan = end[0] - start[0];
//	ft_eprintf("xspan : %d, yspan :%d\n", xspan, end[1] - start[1]);
//	yspan = end[1] - start[1];
//	lpxs = (uint32_t *)rc.layer->pixels;
//	layer_pxls = (uint32_t *)rc.layer->pixels;
//	layer_pxls = (uint32_t *)rc.layer;
	i = -1;//start[0] - 1;
	while (++i < dims[0])//end[0])
	{
		if (pd->rdata->dist < dist)
			continue ;
//		printf("Projecting Column\n");
		prtl_proj_vector(pd, &cub->map, obj);
		ppxs = init_proj_wcol(cub, pd, &rc, &tex_width, dims[1]);// init projected wall data.
//		printf("bypass col : %p\n", (void *)(size_t)(*tpxs));
		j = -1;//start[1] - 1;
		while (++j < rc.scn_height)// && rc.layer->pixels[(start[0] + i) + (start[1] + j) * SCN_WIDTH] == cub->objs.portal.proj_clr)//end[1])
		{
	//		printf("pd idx %d, i %d, ", pd->rdata->idx, i);
			printf("layer (x, y) : (%d, %d), i, j (%d, %d), offx %d, offy %d\n", (loffs[0] + i), (loffs[1] + j), i, j, loffs[0], loffs[1]);
			lpxs = ((uint32_t *)cub->renderer.objs_layer) + ((loffs[0] + i) + (loffs[1] + j) * SCN_WIDTH);
			printf("obj_layer : %p\n", cub->renderer.objs_layer);
			tex_col = tpxs[(int)((i + toffs[0]) * tincrs[0])
					+ (int)((j + toffs[1]) * tincrs[1]) * tex_width];
			printf("lpxs : %p, len : %ld, len2 : %d, tex_col : %p\n", lpxs, lpxs - (uint32_t *)cub->renderer.objs_layer, (loffs[0] + i) + (loffs[1] + j) * SCN_WIDTH, (void *)(size_t)tex_col);
//			printf("obj_lyr[0] : %d\n", ((uint32_t *)cub->renderer.objs_layer)[0]);
			printf("objlyr at offset : %p\n", (void *)(size_t)((uint32_t *)cub->renderer.objs_layer)[270 * SCN_WIDTH]);
//			printf("lpxs[0] : %d\n", lpxs[-277500]);
			*lpxs = tex_col;
//			obj_put_pixel(cub->renderer.objs_layer, j + loffs[0], i + loffs[1],
//					pxls[(int)((j + toffs[0]) * tex_incrs[0])
//						+ (int)((i + toffs[1]) * tex_incrs[1]) * tex->width]);
			
			if (!tex_col)
			{
	//			printf("!tex_col at tpxl (x, y) = (%d, %d)\n", (int)((i + toffs[0]) * tincrs[0]), (int)((j + toffs[1]) * tincrs[1]));
				continue ;
			}
			else if (tex_col == 0xffffffff)// Render projection
			{
	//			printf("tex_col WHITE ! Projecting \n");
				tex_col = ppxs[(int)(((j - rc.half_height) * rc.ratio) + rc.half_texh) * tex_width];
				
//				printf("(%d, %d) : (%d, %d), proj px %p, tex px %p\n", loffs[0] + i, loffs[1] + j);
//				if (!tex_col)
//					printf("Trading one devil with the next \n");
			}
			*lpxs = tex_col;
//			cub_put_pixel(rc.layer, loffs[0] + i, rc.scn_start_y + j,//start[1] + j,
//				tex_col);//pxls[tbuff_offset]);//(int)(((j - rc.half_height) * rc.ratio) + rc.half_texh) * tex_width]);
			
		}
		pd++;
	}
//	cub->renderer.requires_update = 0;
}
*/
static void	__render_portal_with_projection(t_cub *cub, int dist, t_oinst *obj, mlx_texture_t *tex,
		t_pdata *pd, int *dims, int *loffs, int *toffs, float *tincrs)
{
	int			i;
	int			j;
	t_rcol		rc;
//	uint32_t	*lpxs;// layer pixel array
	uint32_t	*tpxs;// obj tex pixel array
	uint32_t	*ppxs;// projection wall pixel array
	int			ptex_width;
	int			ttex_width;
	uint32_t	tex_col;
	int			scn_y;

	(void)obj;
	(void)prtl_proj_vector;
//	printf("tincrs : %f, %f, toffs : %d, %d, tex px : (%d, %d)\n", tincrs[0], tincrs[1], toffs[0], toffs[1], (int)(toffs[0] * tincrs[0]), (int)(toffs[1] * tincrs[1]));
	rc.layer = cub->renderer.objs_layer;
	pd += loffs[0];
	tpxs = (uint32_t *)tex->pixels;
	ttex_width = tex->width;
	i = -1;
	while (++i < dims[0])
	{
		if (pd[i].rdata->dist < dist)
			continue ;
		prtl_proj_vector(pd + i, &cub->map, obj);
		ppxs = init_proj_wcol(cub, pd + i, &rc, &ptex_width, dims[1]);// init projected wall data.
		j = -1;
		while (++j < dims[1])//rc.scn_height)
		{
			tex_col = tpxs[(int)((i + toffs[0]) * tincrs[0])
					+ ((int)((j + toffs[1]) * tincrs[1])) * ttex_width];
			scn_y = (loffs[1] + j);
//			printf(" rc.scn_start_y (%d) < (loffs[1] + j) (%d) < (SCN_HEIGHT - rc.scn_start_y) (%d) ? %d\n", rc.scn_start_y, (loffs[1] + j), (SCN_HEIGHT - rc.scn_start_y), rc.scn_start_y < (loffs[1] + j) &&  (loffs[1] + j) < (SCN_HEIGHT - rc.scn_start_y));
			if (tex_col == 0xffbcbbb0 && rc.scn_start_y < scn_y && scn_y < (SCN_HEIGHT - rc.scn_start_y))
			{
			//	tex_col = ppxs[(int)(((j - rc.half_height) * rc.ratio) + rc.half_texh) * ptex_width];
				//tex_col = ppxs[((int)((i + toffs[1]) * tincrs[1])) * tex->width];
				//printf("scn delta y : %d, tex y : %f\n", (scn_y - cub->scn_midy), (scn_y - cub->scn_midy) * rc.ratio + rc.half_texh);
				tex_col = ppxs[((int)((scn_y - cub->scn_midy) * rc.ratio) + rc.half_texh) * tex->width];

//				obj_put_pixel(cub->renderer.objs_layer, j + loffs[0], i + loffs[1],
//					pxls[(int)((j + toffs[0]) * tex_incrs[0])
//						+ (int)((i + toffs[1]) * tex_incrs[1]) * tex->width]);		
			}
			cub_put_pixel(rc.layer, loffs[0] + i, loffs[1] + j, tex_col);
		}
//		pd++;
	}
}

static inline void	obj_put_pixel(mlx_image_t *img, int x, int y, uint32_t col)
{
	if (col)
		((uint32_t *)img->pixels)[x + y * img->width] = col;
}

void	__render_obj(t_cub *cub, int dist, mlx_texture_t *tex, t_rdata *rd, int *dims,
		int *loffs, int *toffs, float *tex_incrs)//, int *end)
{
	int			i;
	int			j;
	uint32_t	*pxls;

	pxls = (uint32_t *)tex->pixels;
	j = -1;
		while (++j < dims[0])
		{
			if (rd[j + loffs[0]].dist < dist)
				continue ;
			i = -1;
			while (++i < dims[1])
			{
				obj_put_pixel(cub->renderer.objs_layer, j + loffs[0], i + loffs[1],
					pxls[(int)((j + toffs[0]) * tex_incrs[0])
						+ (int)((i + toffs[1]) * tex_incrs[1]) * tex->width]);
			}
		}
}

void	render_objects(t_cub *cub)//, t_rdata *rd)
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

	clear_image_buffer(cub->renderer.objs_layer);

	obj = cub->objs.instances;
	while (obj)
	{
		printf("\n\n Rendering obj %p\n", obj);
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
			printf("BAILOUT\n");
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
			printf("RENDERING PORTAL OBJECT WITH PROJ! from start %d to end %d\n", loffs[0], loffs[2]);
		//	if (prtl_proj_vectors(cub->hero.rcast.prtl_proj + loffs[0],
		//		&cub->map, obj);//, dims[0]))
			//	__render_portal_projection(cub, cub->hero.rcast.prtl_proj, start, end);
	//		prtl_proj_vectors(cub->hero.rcast.prtl_proj + loffs[0], &cub->map, obj);
			__render_portal_with_projection(cub, obj->dist, obj, tex, cub->hero.rcast.prtl_proj, dims, loffs, toffs, tincrs);
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

