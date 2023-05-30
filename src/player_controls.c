/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 05:52:18 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/29 23:36:58 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Pass some positive/negative increment to rotate right/left
void	cub_player_rotate(t_cub *cub, float rot)
{
	if (!rot)
		return ;
//	printf("rot : %f\n", rot);
	cub->hero.ply_obj->ori += rot;
	if (cub->hero.ply_obj->ori < 0)
		cub->hero.ply_obj->ori += M_TAU;
	else if (cub->hero.ply_obj->ori > M_TAU)
		cub->hero.ply_obj->ori -= M_TAU;
	cub->renderer.sky_ori_offset = (int)(cub->hero.ply_obj->ori
		* cub->renderer.sky_radial_width);
//	update_sky_toffs(cub, cub->renderer.sky_base_toffs,
//		cub->renderer.sky_toffs);
	update_rays(cub);
	cub->renderer.requires_update = 1;
}

// Pass some positive/negative increment to d_walk to move forward/backward
// Pass some positive/negative increment to d_strafe to move left/right
void	cub_player_move(t_cub *cub, float d_walk, float d_strafe)
{
	float	dx;
	float	dy;
	int		cx;
	int		cy;

	if (!d_strafe && !d_walk)
		return ;
	dx = (d_walk * (*cub->hero.dirx)) - (d_strafe * (*cub->hero.diry));
	dy = (d_walk * (*cub->hero.diry)) + (d_strafe * (*cub->hero.dirx));
	cx = (int)(cub->inv_cw * (cub->hero.ply_obj->px + dx));
	cy = (int)(cub->inv_cw * (cub->hero.ply_obj->py + dy));
	if (!get_is_cell_within_bounds(&cub->map, cx, cy))
		return ;
	if (is_wall(&cub->map, cx, cy))
	{
		cub->hero.ply_obj->px += dx * (cub->hero.ply_obj->cx == cx);
		cub->hero.ply_obj->py += dy * (cub->hero.ply_obj->cy == cy);
	}
	else
	{
		cub->hero.ply_obj->px += dx;
		cub->hero.ply_obj->py += dy;
	}
	cub->hero.ply_obj->cx = (int)(cub->inv_cw * cub->hero.ply_obj->px);
	cub->hero.ply_obj->cy = (int)(cub->inv_cw * cub->hero.ply_obj->py);
	update_rays(cub);
	cub->renderer.requires_update = 1;
}

// Pass some positive/negative increment to zoom in/out by this factor.
// ex: dz = +0.1 would multiply the current fov by 1.1 (1 + dz)
// ex: dz = -0.1 would multiply the current fov by 0.9 (1 + dz)
void	cub_player_zoom(t_cub *cub, float dz)
{
	float	new_fov;

	if (fabsf(dz) > 1.0f)
		return ;
	new_fov = cub->fov * (1 + dz);
	if (new_fov < FOV_MIN)
		new_fov = FOV_MIN;
	else if (new_fov > FOV_MAX)
		new_fov = FOV_MAX;
	if (new_fov != cub->fov)
	{
//		update_sky_base_toffs(cub, cub->renderer.sky_base_toffs,
//			cub->renderer.sky_toffs);
		update_fov(cub, new_fov);
		cub->renderer.requires_update = 1;
	}
}
