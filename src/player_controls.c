/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 05:52:18 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/31 22:25:01 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Pass some positive/negative increment to rotate right/left
void	cub_player_rotate(t_cub *cub, float rot)
{
	obj_rotate(cub, cub->hero.ply_obj, rot);
	cub->renderer.sky_ori_offset = (int)(cub->hero.ply_obj->ori
		* cub->renderer.sky_radial_width);
	update_rays(cub);
	cub->renderer.requires_update = 1;
}

// Pass some positive/negative increment to d_walk to move forward/backward
// Pass some positive/negative increment to d_strafe to move left/right
void	cub_player_move(t_cub *cub, float d_walk, float d_strafe)
{
	obj_move_rel(cub, cub->hero.ply_obj, d_walk, d_strafe);
	update_rays(cub);
	cub->renderer.requires_update = 1;
}
/*
	
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
}
*/
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
		update_fov(cub, new_fov);
		cub->renderer.requires_update = 1;
	}
}
