/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 10:21:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/14 19:38:22 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
/*
//static int	is_obj_in_fov(t_hero *hero, t_oinst *obj)
static int	is_point_in_fov(t_hero *hero, int x, int y)
{
//	return ((((*hero->fov_lx) * obj->oy + (*hero->fov_ly) * -obj->ox) > 0)
//		!= (((*hero->fov_rx) * obj->oy + (*hero->fov_ry) * -obj->ox) > 0));
	return ((((*hero->fov_lx) * y + (*hero->fov_ly) * -x) > 0)
		!= (((*hero->fov_rx) * y + (*hero->fov_ry) * -x) > 0));
}

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

static int	prtl_proj_init_single_vect(t_pdata *pd, t_rdata *rd, t_oinst *obj, t_oinst *link)
{
//	float	ray_scalar_to_obj;

//	printf("pd id %d\n", pd->rdata->idx);
//	ray_scalar_to_obj = obj->dist * (*pd->fwd_len);
//	printf("prtl_proj_init_single_vect, link %p\n", link);
	pd->odist = obj->dist * (*pd->fwd_len);
	pd->dist = pd->odist;

	pd->px = *rd->px + pd->odist * (*rd->rx) + (link->px - obj->px);
	pd->py = *rd->py + pd->odist * (*rd->ry) + (link->py - obj->py);
/// EXTRAS
//	pd->px = *rd->px + (int)(ray_scalar_to_obj * (*rd->rx)) + (link->px - obj->px);
//	pd->py = *rd->py + (int)(ray_scalar_to_obj * (*rd->ry)) + (link->py - obj->py);
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
	rc->scn_end_y = ((SCN_HEIGHT + rc->scn_height) >> 1);
//	rc->scn_end_y = rc->scn_start_y + rc->scn_height;
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
	dists[0] = 0;
	dists[1] = 0;
	isct[0] = 0;
	isct[1] = 0;
	axs = NULL;
//	printf("pframe [%d, %d] [%d, %d], width %d\n", pframe[0], pframe[1], pframe[2], pframe[3], width);
	while (width--)
	{
	//	printf("init single proj vect\n");
		prtl_proj_init_single_vect(pd, pd->rdata, obj, obj->relative);
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

void	__render_proj_walls(t_cub *cub)//, t_pdata *pdata, uint32_t *pbuff, int *pframe)
{
//	const float		pheight_inv = 1.0f / ((float)(pframe[3] - pframe[1]));
//	const float		pwidth_inv = 1.0f / ((float)(pframe[2] - pframe[0]));
//	const int		pframe_mids[2] = {(pframe[2] - pframe[0]) >> 1, (pframe[3] - pframe[1]) >> 1};
	const int		*pframe = cub->renderer.pframe;
	const int		proj_height = pframe[3] - pframe[1];

	
//	int			j_times_sw[SCN_HEIGHT];
	
	int			i;
//	int			ref_i;
	int			j;
	t_pdata		*pd;// pdata ptr
//	uint32_t	*dbuff;// world depth

	uint32_t	*pbuff;

	float		*dpbuff;
//	float		*dp;
	
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
//	char			*ip;

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
	pbuff = (uint32_t *)cub->renderer.objs_layer->pixels;
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
		//printf("scn_end_y : %d\n", rc.scn_end_y);
		while (++j < rc.scn_end_y)//pframe[3])
		{
			isproj += SCN_WIDTH;
			dpbuff += SCN_WIDTH;
//			ip = isproj + (j * SCN_WIDTH);
//			dp = dpbuff + (j * SCN_WIDTH);
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
	//		if (*isproj)
			//	printf("(%d, %d), isproj (%p) : %d, dpbuff (%p) dist : %f vs %f\n", i, j, isproj, *isproj, dpbuff, *dpbuff, pd->dist);
	//			printf("(%d, %d), isproj (%p), dpbuff (%p)\n", i, j, isproj, dpbuff);
//			if (!*isproj || (*dpbuff && *dpbuff < pd->dist))
			// if (j == 0)
			// {
			// 	printf("i %d, j %d\n", i, j);
			// 	printf("pd : %p\n", pd);
			// 	printf("pd->dist : %f\n", pd->dist);
			// 	printf("dp : %p\n", dp);
			// 	printf("*dp : %f\n", *dp);
			// 	printf("ip : %p\n", ip);
			// 	printf("*ip : %d\n", *ip);
			// }
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

/*
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
*/
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

//	printf("dpbuff - dbuff : %ld\n", (cub->renderer.dpbuff - cub->renderer.dbuff) / (SCN_WIDTH * sizeof(float)));
	j = -1;
	while (++j < dims[1])
	{
		toffs_y[j] = (int)((j + toffs[1]) * tex_incrs[1]) * tex->width;
		loffs_y[j] = j + loffs[1];
	}
//	printf("render proj dist : %d, dims (%d, %d), loffs (%d, %d), toffs (%d, %d), tex_incrs (%f, %f)\n",
//		dist, dims[0], dims[1], loffs[0], loffs[1], toffs[0], toffs[1], tex_incrs[0], tex_incrs[1]);
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
	int		drawx;
	float		ratio;
	float		odist;
//	float		rightwardx_halfw;
//	float		rightwardy_halfw;
	mlx_texture_t	*tex;

	int		loffs[4];
	int		toffs[2];
	float		tincrs[2];
	int		dims[2];
//	int		pframe[4];

	obj = cub->objs.instances;
	prtl = (t_oinst *)cub->renderer.portal;
	link = (t_oinst *)cub->renderer.portal->relative;
	ppos[0] = cub->hero.px + (link->px - prtl->px);
	ppos[1] = cub->hero.py + (link->py - prtl->py);
//	printf("\nlink id %d %p, (%.2f, %.2f)\n", link->_id, link, link->px, link->py);
	while (obj)
	{
//		printf("\n\n Rendering obj %p\n", obj);
//		ov[0] = obj->px - link->px;
//		ov[1] = obj->py - link->py;
		ov[0] = obj->px - ppos[0];
		ov[1] = obj->py - ppos[1];
//		printf("obj id %d (%.2f, %.2f) to portal id %d (%.2f, %.2f)\n", obj->_id, obj->px, obj->py, link->_id, link->px, link->py);
//		obj->ox = obj->px - cub->hero.px;
//		obj->oy = obj->py - cub->hero.py;
		
		odist = (*cub->hero.dirx) * ov[0] + (*cub->hero.diry) * ov[1];
//		printf("odist (prtl->dist (%f) + dirx (%f) * ox (%f) + diry (%f) * oy (%f)) = %f\n",
//			prtl->dist, (*cub->hero.dirx), ov[0], (*cub->hero.diry), ov[1], odist);
///		obj->dist = (*cub->hero.dirx) * obj->ox + (*cub->hero.diry) * obj->oy;
//		rightwardx_halfw = (*cub->hero.diry) * obj->type->half_w;
//		rightwardy_halfw = (*cub->hero.dirx) * obj->type->half_w;
//		obj->ox_left = obj->ox - rightwardx_halfw;//(*cub->hero.diry) * obj->type->half_w;//obj->px - obj->uy;
//		obj->oy_left = obj->oy + rightwardy_halfw;//(*cub->hero.dirx) * obj->type->half_w;//obj->py + obj->ux;
//		obj->ox_right = obj->ox + rightwardx_halfw;//*cub->hero.diry) * obj->type->half_w;
//		obj->oy_right = obj->oy - rightwardy_halfw;//(*cub->hero.dirx) * obj->type->half_w;

		ratio = cub->near_z / odist;
		drawx = (int)(((*cub->hero.dirx) * ov[1] - (*cub->hero.diry) * ov[0])
			* ratio) + cub->scn_midx;

		dims[0] = (int)(ratio * obj->type->width);
		dims[1] = (int)(ratio * obj->type->height);

//		printf("ratio : %f, drawx : %d, dims : (%d, %d)\n", ratio, drawx, dims[0], dims[1]);

		if (obj == link || odist <= 0 || pframe[2] <= drawx || (drawx + dims[0]) < pframe[0])
//			|| !(is_point_in_fov(&cub->hero, obj->ox, obj->oy)
//			|| is_point_in_fov(&cub->hero, obj->ox_left, obj->oy_left)
//			|| is_point_in_fov(&cub->hero, obj->ox_right, obj->oy_right)))
		{
//			printf("BAILOUT\n");
//			printf("obj == link %d, pframe [%d, %d] to [%d, %d], drawx %d, drawx + dims[0] %d\n", 
//				obj == link, pframe[0], pframe[1], pframe[2], pframe[3], drawx, drawx + dims[0]);
			obj = obj->next;
			continue ;
		}

//		printf("mid 1\n");	
		tex = obj->type->texs[obj->tex_idx];
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
			toffs[0] = pframe[0] - loffs[0];//-loffs[0];
			dims[0] -= toffs[0];//+= loffs[0];
			loffs[0] = pframe[0];//0;
		}
		if (loffs[1] < pframe[1])
		{
			toffs[1] = pframe[1] - loffs[1];//-loffs[1];
			dims[1] -= toffs[1];//+= loffs[1];
			loffs[1] = pframe[1];//0;
		}
		if (loffs[2] > pframe[2])//SCN_WIDTH)
		{
			dims[0] -= (loffs[2] - pframe[2]);//-= (loffs[2] - SCN_WIDTH);// * tex_incx;
			loffs[2] = pframe[2];//SCN_WIDTH;
		}
		if (loffs[3] > pframe[3])//SCN_HEIGHT)
		{
			dims[1] -= (loffs[3] - pframe[3]);//(loffs[3] - SCN_HEIGHT);// * tex_incy;
			loffs[3] = pframe[3];//SCN_HEIGHT;
		}

		__render_proj_obj(cub, odist, tex, cub->hero.rcast.prtl_proj, dims, loffs, toffs, tincrs);
/*
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
			__render_obj(cub, obj->dist, tex, cub->hero.rcast.rdata, dims, loffs, toffs, tincrs);
*/
		obj = obj->next;
	}
}

/*
static inline void	obj_put_pixel(mlx_image_t *img, int x, int y, uint32_t col)
{
	if (col)
		((uint32_t *)img->pixels)[x + y * img->width] = col;
}
*/
/*
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
*/
void	__render_obj(t_cub *cub, int dist, mlx_texture_t *tex, t_rdata *rdata, int *dims,
		int *loffs, int *toffs, float *tex_incrs)//, int *end)
{
	int		toffs_x[SCN_WIDTH];
	int		pbuff_start_off;
	int		pbuff_jump;// = SCN_WIDTH - dims[0];
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
	loffs[2] = loffs[0] + j;//i + j;//(dims[0] - loffs[2]);
	pbuff_start_off = loffs[0] + loffs[1] * SCN_WIDTH - 1;
	pbuff_jump = SCN_WIDTH - dims[0];

//	printf("dpbuff - dbuff : %ld\n", (cub->renderer.dpbuff - cub->renderer.dbuff) / (SCN_WIDTH * sizeof(float)));
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
		}

		dbuff += pbuff_jump;		
		pbuff += pbuff_jump;
	}
}

void	__label_isproj(uint32_t *pbuff, char *isproj, int *pframe, int *pdims)
{
	const int		start_offset = pframe[0] + (pframe[1] * SCN_WIDTH);
	const int		buff_jump = SCN_WIDTH - pdims[1];
	int		w;
	int		h;
//	uint32_t	*pb;

	pbuff += start_offset - 1;
	isproj += start_offset - 1;
//	printf("start_offset %d, buff_jump : %d\n", start_offset, buff_jump);
	h = pdims[1];
	while (h--)
	{
		w = pdims[0];
		while (w--)
		{
			++isproj;
			if (*(++pbuff) == PROJ_COLOR)
				*isproj = 1;
		}
		pbuff += buff_jump;
		isproj += buff_jump;			
	}
}
/*
void	__label_isproj(uint32_t *pbuff, char *isproj, int *pframe, int *pdims)
{
	int	i;
	int	j;

	(void)pdims;
	j = pframe[1] - 1;
	while (++j < pframe[3])
	{
		i = pframe[0] - 1;
		while (++i < pframe[2])
		{
			if (pbuff[i + j * SCN_WIDTH] == PROJ_COLOR)
				isproj[i + j * SCN_WIDTH] = 1;
		}
	}
}
*/


void	render_objects(t_cub *cub)//, t_rdata *rd)
{
	t_oinst		*obj;
	int			drawx;
	float		ratio;
//	int	i;
//	float		rightwardx_halfw;
//	float		rightwardy_halfw;
	mlx_texture_t	*tex;

	int		loffs[4];
	int		toffs[2];
	float		tincrs[2];
	int		dims[2];
//	int		pframe[4];

	
	memset(cub->renderer.objs_layer->pixels, 0, sizeof(uint32_t) * SCN_WIDTH * SCN_HEIGHT);
	memset(cub->renderer.dbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);

	obj = cub->objs.instances;
	while (obj)
	{
//		printf("\n\n Rendering obj %p\n", obj);
		obj->ox = obj->px - cub->hero.px;
		obj->oy = obj->py - cub->hero.py;
		
		obj->dist = (*cub->hero.dirx) * obj->ox + (*cub->hero.diry) * obj->oy;
//		rightwardx_halfw = (*cub->hero.diry) * obj->type->half_w;
//		rightwardy_halfw = (*cub->hero.dirx) * obj->type->half_w;
//		obj->ox_left = obj->ox - rightwardx_halfw;//(*cub->hero.diry) * obj->type->half_w;//obj->px - obj->uy;
//		obj->oy_left = obj->oy + rightwardy_halfw;//(*cub->hero.dirx) * obj->type->half_w;//obj->py + obj->ux;
//		obj->ox_right = obj->ox + rightwardx_halfw;//*cub->hero.diry) * obj->type->half_w;
//		obj->oy_right = obj->oy - rightwardy_halfw;//(*cub->hero.dirx) * obj->type->half_w;
		tex = obj->type->texs[obj->tex_idx];
		ratio = cub->near_z / obj->dist;
		dims[0] = (int)(ratio * obj->type->width);
		dims[1] = (int)(ratio * obj->type->height);
		drawx = (int)(((*cub->hero.dirx) * obj->oy - (*cub->hero.diry) * obj->ox)
			* ratio) + cub->scn_midx;

		if ((obj->dist <= 0) || (drawx + dims[0]) < 0 || SCN_WIDTH <= drawx)
//			|| !(is_point_in_fov(&cub->hero, obj->ox, obj->oy)
//			|| is_point_in_fov(&cub->hero, obj->ox_left, obj->oy_left)
//			|| is_point_in_fov(&cub->hero, obj->ox_right, obj->oy_right)))
		{
//			printf("BAILOUT\n");
			obj = obj->next;
			continue ;
		}

//		printf("mid 1\n");	
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
			dims[0] -= (loffs[2] - SCN_WIDTH);// * tex_incx;
			loffs[2] = SCN_WIDTH;
		}
		if (loffs[3] > SCN_HEIGHT)
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
			ft_deltatime_usec_note(NULL);
			
			__render_obj(cub, obj->dist, tex, cub->hero.rcast.rdata, dims, loffs, toffs, tincrs);
//			ft_deltatime_usec_note("empty portal draw time");
			int	*pframe = cub->renderer.pframe;
			int	pdims[2] = {(int)(obj->type->proj_width * ratio), (int)(obj->type->proj_height * ratio)};

			pframe[0] = drawx - (pdims[0] >> 1);
			pframe[2] = ft_clamp(pframe[0] + pdims[0], 0, SCN_WIDTH);
			pframe[0] = ft_clamp(pframe[0], 0, SCN_WIDTH);


//			__render_portal_empty(cub, obj->dist, tex, cub->hero.rcast.rdata,
//				dims, loffs, toffs, tincrs);
//			printf("pframe x [%d, %d]\n", 
//				pframe[0], pframe[2]);
			if (pframe[0] == pframe[2])// || pframe[1] == pframe[3])
			{
				printf("BAILOUT ON PROJECTION !!\n");
				obj = obj->next;
				continue ;
			}
			pframe[1] = cub->scn_midy - (pdims[1] >> 1)
				+ (ratio * obj->type->draw_offy);
			pframe[3] = ft_clamp(pframe[1] + pdims[1], 0, SCN_HEIGHT);
			pframe[1] = ft_clamp(pframe[1], 0, SCN_HEIGHT);

			memset(cub->renderer.dpbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);
			memset(cub->renderer.isproj, 0, sizeof(char) * SCN_WIDTH * SCN_HEIGHT);
			
			cub->renderer.portal = obj;
//			ft_deltatime_usec_note("mem clear + pframe time");
			__label_isproj((uint32_t *)cub->renderer.objs_layer->pixels, cub->renderer.isproj, pframe, pdims);
//			ft_deltatime_usec_note("label isproj time");
//			printf("proj vectors\n");
//			printf("portal link : %p, (%.2f, %.2f)\n", obj->relative, ((t_oinst *)(obj->relative))->px,
//				((t_oinst *)(obj->relative))->py);
//			printf("portal : %p\n", cub->renderer.portal);
			prtl_proj_vectors(cub->hero.rcast.prtl_proj, &cub->map, obj, cub->renderer.pframe);
//			ft_deltatime_usec_note("projection vectors time");
//			printf("portal : %p\n", cub->renderer.portal);
			order_draw_call(cub, cub->draw_threads, 3, 6);
//			ft_deltatime_usec_note("multithreaded projection time");
//			__render_proj_objects(cub);//, obj, cub->hero.rcast.prtl_proj, pframe);
//			ft_deltatime_usec_note("Object proj render time");
//			__render_proj_walls(cub);//, cub->hero.rcast.prtl_proj, (uint32_t *)cub->renderer.objs_layer->pixels, pframe);
//			ft_deltatime_usec_note("Walls proj render time");
//			__render_proj_floor(cub);//, (uint32_t *)cub->renderer.objs_layer->pixels, cub->hero.rcast.prtl_proj, pframe);
//			ft_deltatime_usec_note("floor sky proj render time");
//			printf("render_floor_sky\n");
		}
		else
			__render_obj(cub, obj->dist, tex, cub->hero.rcast.rdata, dims, loffs, toffs, tincrs);

		obj = obj->next;
	}
}

