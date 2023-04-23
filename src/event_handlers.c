/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:22:30 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/23 13:06:22 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	on_close(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	printf("CLOSE REQUEST RECEIVED !\n");
	mlx_close_window(cub->mlx);
}

void	on_keypress(mlx_key_data_t event, void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	if (event.action != MLX_PRESS)
		return ;
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

void	on_scroll(double deltax, double deltay, void *param)
{
	t_cub	*cub;

	(void)deltax;
	cub = (t_cub *)param;
	if (deltax)
		cub_player_zoom(cub, deltax * 0.1f);
	else
		cub_player_zoom(cub, deltay * 0.1f);
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

void	on_update(t_cub *cub)
{
	float	d_walk;
	float	d_strafe;
	int		shift_pressed;

	d_walk = 0;
	d_strafe = 0;
	shift_pressed = mlx_is_key_down(cub->mlx, MLX_KEY_LEFT_SHIFT) * 100;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_W))
		d_walk += (100 + shift_pressed) * cub->mlx->delta_time;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_S))
		d_walk -= (100 + shift_pressed) * cub->mlx->delta_time;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_A))
		d_strafe -= (100 + shift_pressed) * cub->mlx->delta_time;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_D))
		d_strafe += (100 + shift_pressed) * cub->mlx->delta_time;
	if (d_walk || d_strafe)
		cub_player_move(cub, d_walk, d_strafe);
	if (cub->renderer.requires_update)
	{
		ft_deltatime_usec_note(NULL);
		order_draw_call(cub->draw_threads);
//		render_walls(cub, cub->hero.rcast.rdata);
		ft_deltatime_usec_note("this shit == bananas");
		cub->renderer.requires_update = 0;
	}
}
