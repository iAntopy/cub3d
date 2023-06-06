/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_floor_sky_proj.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 17:27:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/06 00:32:42 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
static void	__render_floor_incr(float **ps, const float **rays, uint32_t **bs)
{
	++(*ps);
	++rays[0];
	++rays[1];
	++(bs[0]);
	++(bs[1]);
}

static void	__render_flr_cil_incr(float **ps, const float **rays, uint32_t **bs)
{
	++(*ps);
	++rays[0];
	++rays[1];
	++(bs[0]);
	++(bs[1]);
	++(bs[2]);
	++(bs[3]);
}
*/

typedef struct s_sky_proj_data
{
	int			*soffy;
	float		*dpbuff;
	char		*isproj;
	uint32_t	*tpx;
	int			twidth;
	int			toffs_x[SCN_WIDTH];
}	t_skyp;

void	__init_sky_proj_data(t_cub *cub, t_skyp *sp, int start, int end)
{
	int	i;

	sp->tpx = (uint32_t *)cub->tex.skymap->pixels;
	sp->twidth = cub->tex.skymap->width;
	i = start - 1;
	while (++i < end)
		sp->toffs_x[i] = (int)((i - cub->scn_midx) * cub->inv_sw
				* cub->renderer.sky_fov_to_tex
				+ cub->renderer.sky_ori_offset) % cub->tex.skymap->width;
}

void	__render_proj_sky(t_cub *cub, uint32_t *pbuff, int *pframe)
{
	t_skyp		sp;
	uint32_t	*pb;
	int			*tp;
	int			i;
	int			j;

	__init_sky_proj_data(cub, &sp, pframe[0], pframe[2]);
	j = pframe[1] - 1;
	while (++j < cub->scn_midy)
	{
		sp.soffy = cub->renderer.sky_yoffsets + j;
		sp.dpbuff = cub->renderer.dpbuff + j * SCN_WIDTH;
		sp.isproj = cub->renderer.isproj + j * SCN_WIDTH;
		tp = sp.toffs_x + pframe[0];
		pb = pbuff + pframe[0] + j * SCN_WIDTH;
		i = pframe[0] - 1;
		while (++i < pframe[2])
		{
			if (sp.isproj[i] && !sp.dpbuff[i])
				*pb = ((uint32_t *)cub->tex.skymap->pixels)[*tp
					+ (*sp.soffy) * cub->tex.skymap->width] & 0xafffffff;
			++tp;
			++pb;
		}
	}
}

static void	__floor_ceil_inwall_exception(t_cub *cub, t_pdata *pd, int *cell)
{
	if (is_wall(&cub->map, cell[0], cell[1]))
	{
		if (pd->rdata->side == N_SIDE || pd->rdata->side == S_SIDE)
			cell[1] = pd->cy - pd->rdata->cincr_y;
		else
			cell[0] = pd->cx - pd->rdata->cincr_x;
	}
}

static void	__render_proj_floor_ceiling(t_cub *cub, t_pdata *pdata, uint32_t *pbuff, int *pframe)
{
	float			*params = cub->renderer.floor_factors;
	int				i;
	int				j;
	float			p[2];
	int				c[2];
	float			t[2];
	uint32_t		*pf;
//	uint32_t		*pc;
	float			*dpbuff_flr;
	t_matrx			*pset;
	mlx_texture_t	*tex_flr;
	mlx_texture_t	*tex_cil;
	t_pdata			*pd;
	char			*isproj;
	int				ceil_offset;
	float			ray_scalar;

	j = cub->scn_midy;
	while (++j < pframe[3])
	{
		dpbuff_flr = cub->renderer.dpbuff + cub->buff_offys[j];
		isproj = cub->renderer.isproj + cub->buff_offys[j];
		pf = pbuff + pframe[0] + cub->buff_offys[j] - 1;
		i = pframe[0] - 1;
		pd = pdata + i;
		while (++i < pframe[2])
		{
			++pd;
			++pf;
			if (!isproj[i])
				continue ;
			ray_scalar = params[i + (j - cub->scn_midy) * SCN_WIDTH] - pd->odist;
			p[0] = pd->px + *pd->rdata->rx * ray_scalar;
			p[1] = pd->py + *pd->rdata->ry * ray_scalar;
			if (p[0] < 0.0f || cub->map.width_px <= p[0]
				|| p[1] < 0.0f || cub->map.height_px <= p[1])
				continue ;
			get_cell(p[0], p[1], c, c + 1);
			find_vector_delta(get_grid_coords(&cub->map, c[0], c[1]), p, t);
			__floor_ceil_inwall_exception(cub, pd, c);
			pset = cub->map.mx[c[1]][c[0]];
			
			if (!pset)
				continue ;
			tex_flr = pset->xwalls[0];
			
			if (!dpbuff_flr[i])
				*pf = get_tex_pixel(tex_flr, t[0] * tex_flr->width * cub->inv_cw,
					t[1] * tex_flr->height * cub->inv_cw) & TRANSPARENCY;
			ceil_offset = i + (SCN_WIDTH * (SCN_HEIGHT - j));
			tex_cil = pset->xwalls[1];
			if (!tex_cil || cub->renderer.dpbuff[ceil_offset])
				continue ;
			pbuff[ceil_offset] = get_tex_pixel(tex_cil, t[0] * tex_cil->width * cub->inv_cw,
				t[1] * tex_cil->height * cub->inv_cw) & TRANSPARENCY;
		}
	}
}

void	__render_proj_floor(t_cub *cub)
{
	__render_proj_sky(cub, (uint32_t *)cub->renderer.objs_layer->pixels,
		cub->renderer.pframe);
	__render_proj_floor_ceiling(cub, cub->hero.rcast.pdata, 
		(uint32_t *)cub->renderer.objs_layer->pixels, cub->renderer.pframe);
}
