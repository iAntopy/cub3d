/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghebert <ghebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:22:30 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/22 13:54:38 by ghebert          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "cub3d.h"

void	on_keypress(mlx_key_data_t event, void *param)
{
	t_cub	*cub;
	t_oinst	*obj;

	cub = (t_cub *)param;
	if (event.action != MLX_PRESS)
		return ;
	if (event.key == MLX_KEY_SPACE)
	{
		obj = cub->hero.ply_obj->spawnpoint;
		if (obj)
			spawn_new_player(obj, 1);
	}
	if (event.key == MLX_KEY_ESCAPE)
		on_close(param);
	else if (event.key == MLX_KEY_LEFT)
		cub_player_rotate(cub, -10.0f * ROT_FACTOR);
	else if (event.key == MLX_KEY_RIGHT)
		cub_player_rotate(cub, 10.0f * ROT_FACTOR);
	else if (event.key == MLX_KEY_UP)
		cub_player_zoom(cub, -0.1);
	else if (event.key == MLX_KEY_DOWN)
		cub_player_zoom(cub, 0.1);
}

void	on_cursor_move(double xpos, double ypos, void *param)
{
	static int	init;
	t_cub		*cub;
	float		dx;

	(void)ypos;
	cub = (t_cub *)param;
	dx = (float)(xpos - cub->scn_midx);
	if (init == 0)
	{
		init = 1;
		mlx_set_mouse_pos(cub->mlx, cub->scn_midx, cub->scn_midy);
		return ;
	}
	cub_player_rotate(cub, dx * ROT_FACTOR);
	mlx_set_mouse_pos(cub->mlx, cub->scn_midx, cub->scn_midy);
}

static void	on_update_keypressed(t_cub *cub)
{
	int	kp[9];

	if (mlx_is_key_down(cub->mlx, MLX_KEY_ESCAPE))
	{
		on_close(cub);
		return ;
	}
	kp[8] = mlx_is_key_down(cub->mlx, MLX_KEY_LEFT_SHIFT);
	kp[0] = mlx_is_key_down(cub->mlx, MLX_KEY_W);
	kp[1] = mlx_is_key_down(cub->mlx, MLX_KEY_S);
	kp[2] = mlx_is_key_down(cub->mlx, MLX_KEY_A);
	kp[3] = mlx_is_key_down(cub->mlx, MLX_KEY_D);
	kp[4] = mlx_is_key_down(cub->mlx, MLX_KEY_LEFT);
	kp[5] = mlx_is_key_down(cub->mlx, MLX_KEY_RIGHT);
	kp[6] = mlx_is_key_down(cub->mlx, MLX_KEY_UP);
	kp[7] = mlx_is_key_down(cub->mlx, MLX_KEY_DOWN);
	if (kp[0] || kp[1] || *(size_t *)(kp + 2))
		cub_player_move(cub, (kp[0] * (kp[0] + kp[8]) - kp[1] * (kp[1] + kp[8]))
			* 100 * cub->mlx->delta_time,
			(kp[3] * (kp[3] + kp[8]) - kp[2] * (kp[2] + kp[8]))
			* 100 * cub->mlx->delta_time);
	if (*((size_t *)(kp + 4)))
		cub_player_rotate(cub, (kp[5] - kp[4]) * 1.0f * cub->mlx->delta_time);
	if (*((size_t *)(kp + 6)))
		cub_player_zoom(cub, (kp[7] - kp[6]) * 0.8f * cub->mlx->delta_time);
}

void	on_update(t_cub *cub)
{
	commit_all_obj_actions(cub);
	on_update_keypressed(cub);
	if (cub->renderer.requires_update)
	{
		if (DEBUG)
			ft_deltatime_usec_note(NULL);
		update_minimap(cub);
		order_draw_call(cub->draw_threads, 0, 3);
		if (DEBUG)
			ft_deltatime_usec_note("this == bananas");
		cub->renderer.requires_update = 0;
	}
}
