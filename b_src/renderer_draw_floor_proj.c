/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_floor_proj.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 17:27:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/27 19:20:14 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

typedef struct s_floor_ceiling_proj_data
{
	t_cub			*cub;
	t_pdata			*pdata;
	t_pdata			*pd;
	int				*pframe;
	uint32_t		*pf;
	uint32_t		*pbuff;
	int				x;
	int				y;
	float			*params;
	float			*dpbuff;
	float			*dpbuff_flr;
	char			*isproj;
	float			ray_scalar;
	mlx_texture_t	**tex_arr;
}	t_flrp;

static void	__init_flr_ceil_proj_data(t_cub *cub, t_flrp *fp)
{
	fp->cub = cub;
	fp->pdata = cub->hero.rcast.pdata;
	fp->pframe = cub->renderer.pframe;
	fp->pbuff = (uint32_t *)cub->renderer.objs_layer->pixels;
	fp->y = cub->scn_midy;
	fp->params = cub->renderer.floor_factors - 1;
	fp->dpbuff = cub->renderer.dpbuff;
	fp->dpbuff_flr = fp->dpbuff + cub->yoffs[fp->y];
	fp->isproj = cub->renderer.isproj + cub->yoffs[fp->y];
}

static void	__init_flr_ceil_proj_row(t_cub *cub, t_flrp *fp)
{
	const int	y_offset = cub->yoffs[fp->y];

	fp->dpbuff_flr = cub->renderer.dpbuff + y_offset;
	fp->isproj = cub->renderer.isproj + y_offset;
	fp->pf = fp->pbuff + fp->pframe[0] + y_offset - 1;
	fp->x = fp->pframe[0] - 1;
	fp->pd = fp->pdata + fp->x;
}

static int	__isvalid_floor_intersect(t_flrp *fp, float *p, int *c)
{
	p[0] = fp->pd->px + *fp->pd->rdata->rx * fp->ray_scalar;
	p[1] = fp->pd->py + *fp->pd->rdata->ry * fp->ray_scalar;
	if (p[0] < 0.0f || fp->cub->map.width_px <= p[0]
		|| p[1] < 0.0f || fp->cub->map.height_px <= p[1])
		return (-1);
	get_cell(p[0], p[1], c, c + 1);
	if (c[0] >= fp->cub->map.width || c[1] >= fp->cub->map.height)
		return (-1);
	return (0);
}

static void	__floor_ceil_draw_pixel(t_flrp *fp, t_pdata *pd, int *c, float *t)
{
	const float		inv_cw = fp->cub->inv_cw;
	const t_rdata	*rd = pd->rdata;
	mlx_texture_t	**tarr;
	int				coff;

	if (is_wall(&fp->cub->map, c[0], c[1]))
	{
		if (rd->side == N_SIDE || rd->side == S_SIDE)
			c[1] = pd->cy - rd->cincr_y;
		else
			c[0] = pd->cx - rd->cincr_x;
	}
	tarr = fp->cub->map.mx[c[1]][c[0]]->xwalls;
	if (!tarr)
		return ;
	if (!fp->dpbuff_flr[fp->x])
		*fp->pf = get_tex_pixel(tarr[0], t[0] * tarr[0]->width * inv_cw,
				t[1] * tarr[0]->height * inv_cw) & TRANSPARENCY;
	coff = fp->x + (SCN_WIDTH * (SCN_HEIGHT - fp->y));
	if (!tarr[1] || fp->dpbuff[coff])
		return ;
	fp->pbuff[coff] = get_tex_pixel(tarr[1], t[0] * tarr[1]->width * inv_cw,
			t[1] * tarr[1]->height * inv_cw) & TRANSPARENCY;
}

void	__render_proj_floor(t_cub *cub)
{
	t_flrp			fp;
	float			p[2];
	int				c[2];
	float			t[2];

	__init_flr_ceil_proj_data(cub, &fp);
	__render_proj_sky(cub, fp.pbuff, fp.pframe);
	fp.y = cub->scn_midy;
	while (++fp.y < fp.pframe[3])
	{
		__init_flr_ceil_proj_row(cub, &fp);
		while (++fp.x < fp.pframe[2])
		{
			++fp.pd;
			++fp.pf;
			if (!fp.isproj[fp.x])
				continue ;
			fp.ray_scalar = fp.params[fp.x
				+ (fp.y - cub->scn_midy) * SCN_WIDTH] - fp.pd->odist;
			if (__isvalid_floor_intersect(&fp, p, c) < 0)
				continue ;
			find_vector_delta(get_grid_coords(&cub->map, c[0], c[1]), p, t);
			__floor_ceil_draw_pixel(&fp, fp.pd, c, t);
		}
	}
}
