/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 05:52:18 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/13 19:49:53 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Pass some positive/negative increment to rotate right/left
void	cub_player_rotate(t_cub *cub, float rot)
{
	cub->hero.ori += rot;
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

	dx = (d_walk * (*cub->hero.dirx)) - (d_strafe * (*cub->hero.diry));
	dy = (d_walk * (*cub->hero.diry)) + (d_strafe * (*cub->hero.dirx));
	cx = (int)(cub->inv_cw * (cub->hero.px + dx));
	cy = (int)(cub->inv_cw * (cub->hero.py + dy));
	if (!get_is_cell_within_bounds(&cub->map, cx, cy))
		return ;
	if (is_wall(&cub->map, cx, cy))
	{
		cub->hero.px += dx * (cub->hero.cell_x == cx);
		cub->hero.py += dy * (cub->hero.cell_y == cy);
	}
	else
	{
		cub->hero.px += dx;
		cub->hero.py += dy;
	}
	cub->hero.cell_x = (int)(cub->inv_cw * cub->hero.px);
	cub->hero.cell_y = (int)(cub->inv_cw * cub->hero.py);
	update_rays(cub);
	cub->renderer.requires_update = 1;
}

// Pass some positive/negative increment to zoom in/out by this factor.
// ex: dz = +0.1 would multiply the current fov by 1.1 (1 + dz)
// ex: dz = -0.1 would multiply the current fov by 0.9 (1 + dz)
void	cub_player_zoom(t_cub *cub, float dz)
{
	float	new_fov;

	if (fabsf(dz) > 0.5)
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
