/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_walls_proj.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:28:02 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/27 20:43:01 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	prtl_proj_init_vect(t_pdata **pdata, t_rdata **rdata, t_oinst *obj)
{
	const t_oinst	*link = (t_oinst *)obj->relative;
	t_pdata			*pd;
	t_rdata			*rd;

	pd = ++(*pdata);
	rd = pd->rdata;
	pd->odist = obj->dist * (*pd->fwd_len);
	pd->dist = pd->odist;
	pd->px = *rd->px + pd->odist * (*rd->rx) + (link->px - obj->px);
	pd->py = *rd->py + pd->odist * (*rd->ry) + (link->py - obj->py);
	pd->cx = (int)(pd->px * rd->inv_cw);
	pd->cy = (int)(pd->py * rd->inv_cw);
	pd->b = pd->py - (rd->a * pd->px);
	*rdata = rd;
	return (1);
}

static int	probe(t_pdata *pd, float *axs, float *isct, float *dists)
{
	float	ratio;
	int		correction;

	pd->dist += dists[dists[0] >= dists[1]];
	if (dists[0] < dists[1])
	{
		pd->hitx = isct[0];
		pd->hity = axs[1];
		pd->side = 1 + (pd->rdata->dy << 1);
		ratio = isct[0] - axs[0];
		correction = (*pd->rdata->rx < 0) != (*pd->rdata->ry < 0);
	}
	else
	{
		pd->hitx = axs[0];
		pd->hity = isct[1];
		pd->side = pd->rdata->dx << 1;
		ratio = isct[1] - axs[1];
		correction = (*pd->rdata->rx < 0) == (*pd->rdata->ry < 0);
	}
	if ((pd->side == W_SIDE) || (pd->side == S_SIDE))
		ratio = -ratio;
	pd->tex_ratio = correction + (ratio * pd->rdata->inv_cw);
	pd->tex_height = (*pd->rdata->near_proj_factor) / pd->dist;
	return (0);
}

// Pass the t_pdata * offset in array to the starting ray to cast
// from 0 <= i < SCN_WIDTH. n is th nb of rays to cast from start (end - start).
int	prtl_proj_vectors(t_pdata *pd, t_map *map, t_oinst *obj, int n)
{
	t_rdata	*rd;
	float	*axs;
	float	isct[2];
	float	dists[2];

	while (n-- && prtl_proj_init_vect(&pd, &rd, obj))
	{
		while (1)
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
			if (is_wall(map, pd->cx, pd->cy) && !probe(pd, axs, isct, dists))
				break ;
		}
	}
	return (0);
}

typedef struct s_walls_proj_data
{
	int			x;
	int			y;
	t_pdata		*pd;
	uint32_t	*pbuff;
	uint32_t	*pb;
	float		*dpbuff;
	char		*isproj;
	uint32_t	*tex_buff;
	uint32_t	tex_shape[2];
	int			buff_offset;
}	t_walp;

static inline void	__init_wproj(t_cub *cub, t_walp *wp, t_rcol *rc, int prj_h)
{
	mlx_texture_t	*tex;
	int				tex_start_x;

	wp->pd = cub->hero.rcast.pdata + wp->x;
	wp->pbuff = (uint32_t *)cub->renderer.objs_layer->pixels;
	wp->pb = wp->pbuff + wp->x;
	tex = cub->map.mx[wp->pd->cy][wp->pd->cx]->xwalls[wp->pd->side];
	wp->tex_shape[0] = tex->width;
	wp->tex_shape[1] = tex->height;
	rc->half_texh = (tex->height >> 1);
	tex_start_x = (int)(wp->pd->tex_ratio * tex->width);
	rc->scn_height = ft_clamp(wp->pd->tex_height, 0, prj_h - 2);
	rc->half_height = (rc->scn_height >> 1);
	rc->ratio = (float)tex->height / (float)wp->pd->tex_height;
	rc->scn_start_y = ((SCN_HEIGHT - rc->scn_height) >> 1);
	rc->scn_end_y = ((SCN_HEIGHT + rc->scn_height) >> 1);
	wp->tex_buff = (uint32_t *)tex->pixels + tex_start_x;
	wp->y = rc->scn_start_y - 1;
	wp->buff_offset = wp->x + cub->yoffs[wp->y + 1] - SCN_WIDTH;
	wp->isproj = cub->renderer.isproj + wp->buff_offset;
	wp->dpbuff = cub->renderer.dpbuff + wp->buff_offset;
}

void	__render_proj_walls(t_cub *cub)
{
	const int		*pframe = cub->renderer.pframe;
	const int		prj_height = pframe[3] - pframe[1];
	t_walp			wp;
	t_rcol			rc;
	uint32_t		tex_y;

	wp.x = pframe[0] - 1;
	while (++wp.x < pframe[2])
	{
		__init_wproj(cub, &wp, &rc, prj_height);
		while (++wp.y < rc.scn_end_y)
		{
			wp.isproj += SCN_WIDTH;
			wp.dpbuff += SCN_WIDTH;
			if (!*wp.isproj || (*wp.dpbuff && *wp.dpbuff < wp.pd->dist))
				continue ;
			tex_y = (int)(wp.y - cub->scn_midy) * rc.ratio + rc.half_texh;
			tex_y = ft_clamp(tex_y, 0, wp.tex_shape[1] - 1);
			wp.pb[cub->yoffs[wp.y]] = wp.tex_buff[tex_y
				* wp.tex_shape[0]] & TRANSPARENCY;
			*wp.dpbuff = wp.pd->dist;
		}
	}
}
