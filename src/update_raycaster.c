/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_raycaster.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 20:10:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/12 21:10:16 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	raycast_init_single_vect(t_rdata *rd)
{
	rd->cx = *rd->pcx;
	rd->cy = *rd->pcy;
	rd->c_offx = (*rd->rx >= 0);
	rd->c_offy = (*rd->ry >= 0);
	rd->cincr_x = (rd->c_offx << 1) - 1;
	rd->cincr_y = (rd->c_offy << 1) - 1;
	rd->a = *rd->ry / *rd->rx;
	rd->inv_a = 1 / rd->a;
	rd->b = *rd->py - (rd->a * *rd->px);
	return (1);
}

static int	raycast_gather_collision_info(t_rdata *rd, float *axs, float *isct, float *dists)
{
	float	ratio;
	int		correction;
	
	if (dists[0] < dists[1])
	{
		rd->hitx = isct[0];
		rd->hity = axs[1];
		rd->dist = dists[0];
		rd->side = 1 + (rd->c_offy << 1);
		ratio = isct[0] - axs[0];
		correction = (*rd->rx < 0) != (*rd->ry < 0);
//		rd->tex_ratio = isct[0] - (axs[0] - rd->c_offx * CELL_WIDTH);
//		if (rd->tex_ratio < 0)
//			fprintf(stderr, "tex intersect %f (from isct[0] : %f - axs[0] : %f), is negative while side : %d\n", rd->tex_ratio, isct[0], axs[0], rd->side);
	}
	else
	{
		rd->hitx = axs[0];
		rd->hity = isct[1];
		rd->dist = dists[1];
		rd->side = rd->c_offx << 1;
		ratio = isct[1] - axs[1];
		correction = (*rd->rx < 0) == (*rd->ry < 0);

//		rd->tex_ratio = isct[1] - (axs[1] - rd->c_offy * CELL_WIDTH);
//		if (rd->tex_ratio < 0)
//			fprintf(stderr, "tex intersect %f (from isct[1] : %f - axs[1] : %f), is negative while side : %d\n", rd->tex_ratio, isct[1], axs[1], rd->side);
	}
//	dprintf(2, "side : %d, corr %d + (ratio %f / CELL_WIDTH)\n", rd->side, correction, ratio);
	if ((rd->side == W_SIDE) || (rd->side == S_SIDE))
	{
//	if (!rd->side | (rd->side == S_SIDE))
//		printf("west or south -> ratio (%f) = -ratio (%f)\n", ratio, -ratio);
		ratio = -ratio;
	}	
//		rd->tex_ratio = 1 - rd->tex_ratio;
	rd->tex_ratio = correction + (ratio * rd->rcast->cub->inv_cw);
//	dprintf(2, "side : %d, corr (rx (%f) < 0 == ry (%f) < 0) %d + (ratio %f / CELL_WIDTH) = %f\n", rd->side, *rd->rx, *rd->ry, correction, ratio, rd->tex_ratio);
//	if (rd->tex_ratio < 0)
//		fprintf(stderr, "idx %d, side %d, tex ratio < 0 : %f, ratio : %f, correctif : %d\n", rd->idx, rd->side, rd->tex_ratio, ratio, correction);
		//fprintf(stderr, "tex intersect %f (from isct[0] : %f - axs[0] : %f), is negative while side : %d\n", rd->tex_ratio, isct[0], axs[0], rd->side);

//	rd->tex_ratio *= rd->rcast->cub->inv_cw;
//	rd->tex_ratio = (rd->tex_ratio + CELL_WIDTH * rd->c_offy)
//		* rd->rcast->cub->inv_cw;


	rd->tex_height = rd->rcast->cub->near_proj_factor / rd->dist;
	return (0);
}

static void	raycast_all_vectors(t_rcast *rcast, t_map *map)
{
	t_rdata	*rd;
	float	*axies;
	float	intersects[2];
	float	dists[2];

	rd = rcast->rdata - 1;
	while ((++rd - rcast->rdata) < SCN_WIDTH && raycast_init_single_vect(rd))
	{
		while (1)
		{
			axies = map->grid_coords[rd->cy + rd->c_offy]
				+ ((rd->cx + rd->c_offx) << 1);
			intersects[1] = rd->a * (*axies) + rd->b;
			intersects[0] = (axies[1] - rd->b) * rd->inv_a;
			dists[0] = (intersects[0] - (*rd->px)) * (*rd->p_dirx)// hori hit
				+ (axies[1] - (*rd->py)) * (*rd->p_diry);
			dists[1] = (axies[0] - (*rd->px)) * (*rd->p_dirx)//	vert hit
				+ (intersects[1] - (*rd->py)) * (*rd->p_diry);
			if (dists[0] < dists[1])
				rd->cy += rd->cincr_y;
			else
				rd->cx += rd->cincr_x;
			if (get_is_wall(rd->rcast->map, rd->cx, rd->cy)
				&& !raycast_gather_collision_info(rd, axies, intersects, dists))
				break ;
		}
	}
}

// If player rotates call this function.
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
	printf("update fov\n");
	cub->fov = fov;
	cub->hfov = 0.5f * fov;
	cub->near_z = (float)cub->scn_midx / tanf(cub->hfov);
	cub->near_proj_factor = CELL_WIDTH * cub->near_z;
	mtx_linspace_update(cub->hero.rcast.theta_offs, -cub->hfov, cub->hfov, 1);
	update_rays(cub);
}
