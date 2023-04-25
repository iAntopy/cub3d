/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_raycaster.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 20:10:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/24 21:02:20 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	raycast_init_single_vect(t_rdata *rd)
{
	rd->cx = *rd->pcx;
	rd->cy = *rd->pcy;
	rd->dx = (*rd->rx >= 0);
	rd->dy = (*rd->ry >= 0);
	rd->cincr_x = (rd->dx << 1) - 1;
	rd->cincr_y = (rd->dy << 1) - 1;
	rd->a = *rd->ry / *rd->rx;
	rd->inv_a = 1.0f / rd->a;
	rd->b = *rd->py - (rd->a * *rd->px);
	return (1);
}

static int	probe(t_rdata *rd, float *axs, float *isct, float *dists)
{
	float	ratio;
	int		correction;

	rd->dist = dists[dists[0] >= dists[1]];
	if (rd->dist == dists[0])
	{
		rd->hitx = isct[0];
		rd->hity = axs[1];
		rd->side = 1 + (rd->dy << 1);
		ratio = isct[0] - axs[0];
		correction = (*rd->rx < 0) != (*rd->ry < 0);
	}
	else
	{
		rd->hitx = axs[0];
		rd->hity = isct[1];
		rd->side = rd->dx << 1;
		ratio = isct[1] - axs[1];
		correction = (*rd->rx < 0) == (*rd->ry < 0);
	}
	if ((rd->side == W_SIDE) || (rd->side == S_SIDE))
		ratio = -ratio;
	rd->tex_ratio = correction + (ratio * rd->rcast->cub->inv_cw);
	rd->tex_height = rd->rcast->cub->near_proj_factor / rd->dist;
	return (0);
}

static void	raycast_all_vectors(t_rcast *rcast, t_map *map)
{
	t_rdata	*rd;
	float	*axs;
	float	isct[2];
	float	dists[2];

	rd = rcast->rdata - 1;
	while ((++rd - rcast->rdata) < SCN_WIDTH && raycast_init_single_vect(rd))
	{
		while (1)
		{
			axs = map->grid_coords[rd->cy + rd->dy] + ((rd->cx + rd->dx) << 1);
			isct[1] = rd->a * axs[0] + rd->b;
			isct[0] = (axs[1] - rd->b) * rd->inv_a;
			dists[0] = (isct[0] - (*rd->px)) * (*rd->p_dirx)
				+ (axs[1] - (*rd->py)) * (*rd->p_diry);
			dists[1] = (axs[0] - (*rd->px)) * (*rd->p_dirx)
				+ (isct[1] - (*rd->py)) * (*rd->p_diry);
			if (dists[0] < dists[1])
				rd->cy += rd->cincr_y;
			else
				rd->cx += rd->cincr_x;
			if (is_wall(map, rd->cx, rd->cy) && !probe(rd, axs, isct, dists))
				break ;
		}
	}
}

// If player moves, rotates or changes fov, call this function.
void	update_rays(t_cub *cub)
{
	_mtx_addf_pscalar(cub->hero.rcast.theta_offs, cub->hero.ori,
		cub->hero.rcast.ray_thetas);
	mtx_linspace_update(cub->hero.rcast.ray_thetas,
		cub->hero.ori - cub->hfov, cub->hero.ori + FOV_HF, 1);
	mtx_cos(cub->hero.rcast.ray_thetas, cub->hero.rcast.rays[0]);
	mtx_sin(cub->hero.rcast.ray_thetas, cub->hero.rcast.rays[1]);
	raycast_all_vectors(&cub->hero.rcast, &cub->map);
}

// If Zoom level (fov) changes call this function.
// fov in radians
// near_z is the distance from the player to the projection plan.
// near_proj_factor is a const as long as fov stays the same. Divide this 
//	factor by the rays length to get draw height.
void	update_fov(t_cub *cub, float fov)
{
	cub->fov = fov;
	cub->hfov = 0.5f * fov;
	cub->near_z = (float)cub->scn_midx / tanf(cub->hfov);
	cub->near_proj_factor = CELL_WIDTH * cub->near_z;
	update_floorcaster_params(cub);
	mtx_linspace_update(cub->hero.rcast.theta_offs, -cub->hfov, cub->hfov, 1);
	update_rays(cub);
}
