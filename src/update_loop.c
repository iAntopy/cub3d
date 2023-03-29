/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 21:21:13 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/13 01:42:15 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


static void	apply_move_and_resist(t_cub *cub, double d_time)
{
	cub_player_move(cub,
		mlx_is_key_down(cub->mlx, MLX_KEY_W) - mlx_is_key_down(cub->mlx, MLX_KEY_S),
		mlx_is_key_down(cub->mlx, MLX_KEY_A) - mlx_is_key_down(cub->mlx, MLX_KEY_D),
		d_time);
	if (fabsf(cub->hero.movx) < 0.1 && fabsf(cub->hero.movy) < 0.1)
	{
		cub->hero.movx = 0.0f;
		cub->hero.movy = 0.0f;
	}
	else
		cub->renderer.requires_update = 1;
	printf("Player movx, movy : (%f, %f)\n", cub->hero.movx, cub->hero.movy);
	if (cub->hero.movy > 0)
		cub->hero.movy *= 1 - PLR_RESIST_FRW * d_time;
	else
		cub->hero.movy *= 1 - PLR_RESIST * d_time;
		//cub->hero.movy *= PLR_RESIST;
	cub->hero.movx *= (1 - PLR_RESIST) * d_time;

//	cub->hero.px += (cub->hero.movx * (*cub->hero.dirx)) - (cub->hero.movy * (*cub->hero.diry));
//	cub->hero.py += (cub->hero.movx * (*cub->hero.dirx)) + (cub->hero.movy * (*cub->hero.diry));
	cub->hero.px += (cub->hero.movy * (*cub->hero.dirx)) + (cub->hero.movx * -(*cub->hero.diry));
	cub->hero.py += (cub->hero.movy * (*cub->hero.diry)) + (cub->hero.movx * (*cub->hero.dirx));
	cub->hero.cell_x = (int)(cub->inv_cw * cub->hero.px);
	cub->hero.cell_y = (int)(cub->inv_cw * cub->hero.py);
}

void    cub_on_update(void *param)
{
	t_cub   *cub;

	usleep(FRAME_UTIME);
	cub = (t_cub *)param;

	apply_move_and_resist(cub, cub->mlx->delta_time);
//	activate_keypress_event(cub, cub->mlx->delta_time);

	if (cub->renderer.requires_update)
	{
		printf("Rendering SCENE !!\n");
		update_rays(cub);
		render_scene(cub);
	}
	printf("delta time : %lf\n", cub->mlx->delta_time);
}