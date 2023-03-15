/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 05:52:18 by iamongeo          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/03/11 15:42:52 by iamongeo         ###   ########.fr       */
=======
/*   Updated: 2023/03/13 01:39:14 by iamongeo         ###   ########.fr       */
>>>>>>> origin/ian_bonus
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Pass some positive/negative increment to rotate right/left
void	cub_player_rotate(t_cub *cub, float rot)
{
	cub->hero.ori += rot;
//	printf("tex_offset before : %d\n", cub->skymap_tex_offset);
	cub->skymap_tex_offset = (int)(cub->hero.ori * cub->skymap_radial_width);
//	printf("tex_offset after : %d\n", cub->skymap_tex_offset);
//	cub->hero.ori_factor = fabsf(cub->hero.ori * cub->inv_two_pi);
//	printf("ori / ori_factor : %f / %f\n", cub->hero.ori, cub->hero.ori_factor);
<<<<<<< HEAD
	ft_deltatime_usec_note(NULL);
	update_rays(cub);
//	printf("p_dirx, p_diry : (%f, %f)\n", *cub->hero.dirx, *cub->hero.diry);
	render_scene(cub);//	REMOVE ME !! Move this to on_update() function eventually.
	ft_deltatime_usec_note("Believe it or not !");
=======
//	update_rays(cub);

	cub->renderer.requires_update = 1;
//	render_scene(cub);//	REMOVE ME !! Move this to on_update() function eventually.
>>>>>>> origin/ian_bonus
}

//void	cub_player_wall_collision_correction(t_cub *cub, float dx, float dy)

// Pass some positive/negative increment to d_walk to move forward/backward
// Pass some positive/negative increment to d_strafe to move left/right
void	cub_player_move(t_cub *cub, float d_walk, float d_strafe, float d_time)
{
<<<<<<< HEAD
	float	dx;
	float	dy;
	int		cx;
	int		cy;
	
//	printf("\nMoving player : d_walk = %f, d_strafe = %f\n", d_walk, d_strafe);
//	printf("pos berfore -> (%f, %f)\n", cub->hero.px, cub->hero.py);
	dx = (d_walk * (*cub->hero.dirx)) - (d_strafe * (*cub->hero.diry));
	dy = (d_walk * (*cub->hero.diry)) + (d_strafe * (*cub->hero.dirx));

//	printf("pos after -> (%f, %f)\n", cub->hero.px + dx, cub->hero.py + dy);
//	printf("cell x, y before -> (%d, %d)\n", cub->hero.cell_x, cub->hero.cell_y);
	cx = (int)(cub->inv_cw * (cub->hero.px + dx));
	cy = (int)(cub->inv_cw * (cub->hero.py + dy));
//	printf("Checking if (%d, %d) is wall !\n", cx, cy);
	if (get_is_cell_within_bounds(&cub->map, cx, cy)
		&& !get_is_wall(&cub->map, cx, cy))
	{
//		printf("Is NOT wall !\n");
		cub->hero.px += dx;//(d_walk * (*cub->hero.dirx)) + (d_strafe * -(*cub->hero.diry));
		cub->hero.py += dy;//(d_walk * (*cub->hero.diry)) + (d_strafe * (*cub->hero.dirx));
		cub->hero.cell_x = cx;//(int)(cub->inv_cw * cub->hero.px);
		cub->hero.cell_y = cy;//(int)(cub->inv_cw * cub->hero.py);
		update_rays(cub);
//		printf("rays updated\n");
		render_scene(cub);//	REMOVE ME !! Move this to on_update() function eventually.
//		printf("scene rendered\n");
		cub->renderer.requires_update = 1;
	}
	printf("cell x, y after -> (%d, %d)\n", cx, cy);//cub->hero.cell_x, cub->hero.cell_y);
=======
//	float	dx;
//	float	dy;
	float	len;
	
	if (!(d_walk || d_strafe))
		return ;
	printf("Moving player : d_walk = %f, d_strafe = %f\n", d_walk, d_strafe);
	printf("pos berfore -> (%f, %f)\n", cub->hero.px, cub->hero.py);
//	dx = (d_walk * (*cub->hero.dirx)) + (d_strafe * -(*cub->hero.diry));
//	dy = (d_walk * (*cub->hero.diry)) + (d_strafe * (*cub->hero.dirx));
	len = sqrtf(d_strafe * d_strafe + d_walk * d_walk);
	printf("movx calc (d_strafe / len) : %f\n", d_strafe / len);
	cub->hero.movx += (d_strafe / len) * PLR_ACCEL * d_time;
	cub->hero.movy += (d_walk / len) * PLR_ACCEL * d_time;


//	cub->hero.px += (d_walk * (*cub->hero.dirx)) + (d_strafe * -(*cub->hero.diry));
//	cub->hero.py += (d_walk * (*cub->hero.diry)) + (d_strafe * (*cub->hero.dirx));
//	printf("pos after -> (%f, %f)\n", cub->hero.px, cub->hero.py);

//	printf("cell x, y before -> (%d, %d)\n", cub->hero.cell_x, cub->hero.cell_y);
//	cub->hero.cell_x = (int)(cub->inv_cw * cub->hero.px);
//	cub->hero.cell_y = (int)(cub->inv_cw * cub->hero.py);
//	printf("cell x, y after -> (%d, %d)\n", cub->hero.cell_x, cub->hero.cell_y);

	cub->renderer.requires_update = 1;
//	render_scene(cub);//	REMOVE ME !! Move this to on_update() function eventually.
>>>>>>> origin/ian_bonus
}

// Pass some positive/negative increment to zoom in/out by this factor.
// ex: dz = +0.1 would multiply the current fov by 1.1 (1 + dz)
// ex: dz = -0.1 would multiply the current fov by 0.9 (1 + dz)
void	cub_player_zoom(t_cub *cub, float dz)
{
	float	new_fov;

	if (fabsf(dz) > 0.5)
		return ;
<<<<<<< HEAD
	new_fov = cub->fov * (1 + dz);
	if (new_fov < FOV_MIN)
		new_fov = FOV_MIN;
	else if (new_fov > FOV_MAX)
		new_fov = FOV_MAX;
	if (new_fov != cub->fov)
	{
		update_fov(cub, new_fov);
		cub->renderer.requires_update = 1;
		render_scene(cub);//	REMOVE ME !! Move this to on_update() function eventually.
	}
=======
	update_fov(cub, cub->fov * (1 + dz));
	cub->renderer.requires_update = 1;
//	render_scene(cub);//	REMOVE ME !! Move this to on_update() function eventually.
>>>>>>> origin/ian_bonus
}
