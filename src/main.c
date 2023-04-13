/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:07:26 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/12 22:06:43 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	cub_clear(t_cub *cub, int exit_status)
{
	printf("CUB CLEAR AT EXIT\n");
	raycaster_clear(&cub->hero.rcast, EXIT_SUCCESS);
	renderer_clear(cub);
	printf("renderer_cleared\n");
	if (cub->mlx)
		mlx_terminate(cub->mlx);
	printf("mlx_terminated\n");
	printf("exit with status : %d\n", exit_status);
	return (exit_status);
}

void	on_close(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	printf("CLOSE REQUEST RECEIVED !\n");
	mlx_close_window(cub->mlx);
}

void	cub_key_handler(mlx_key_data_t event, void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	if (event.action != MLX_PRESS)
		return ;
	if (event.key == MLX_KEY_ESCAPE)
		on_close(param);
//	else if (event.key == MLX_KEY_W)
//		cub_player_move(cub, 10, 0);
//	else if (event.key == MLX_KEY_S)
//		cub_player_move(cub, -10, 0);
//	else if (event.key == MLX_KEY_A)
//		cub_player_move(cub, 0, -10);
//	else if (event.key == MLX_KEY_D)
//		cub_player_move(cub, 0, 10);
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
	t_cub	*cub;
	float	dx;

	(void)ypos;
	cub = (t_cub *)param;
	dx = (float)(xpos - cub->scn_midx);
	cub_player_rotate(cub, dx * ROT_FACTOR);
	mlx_set_mouse_pos(cub->mlx, cub->scn_midx, cub->scn_midy);
}

// On pourra intégrer ces lignes là dans le parsing ou qqc du genre, mais on va en avoir besoin. 
int	cub_init_core_data(t_cub *cub)
{
	cub->scn_midx = SCN_WIDTH / 2;
	cub->scn_midy = SCN_HEIGHT / 2;
	cub->inv_cw = 1.0f / (float)CELL_WIDTH;
	cub->inv_sw = 1.0f / (float)SCN_WIDTH;
	cub->inv_two_pi = 0.5f / M_PI ;
	printf("MAIN : inverse CELL_WIDTH : %.10f\n", cub->inv_cw);

	return (0);
}

int	set_player_cell_pos(t_cub *cub, int x, int y, int side)
{
	printf("Plater (x, y) : (%d, %d)\n", x, y);
	if (get_is_wall(&cub->map, x, y))
		return (printf("ERROR hero can't be placed in wall."));
	cub->hero.cell_x = x;
	cub->hero.cell_y = y;
	cub->hero.px = x * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.py = y * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.ori = M_PI - side * cub->inv_two_pi;
//	cub->hero.ori_factor = fabsf(cub->hero.ori * cub->inv_two_pi);
//	cub->skymap_tex_offset = (int)(cub->hero.ori * cub->skymap_radial_width);
//	printf("SET PLAYER POS : ori : %f, sky_tex_offset : %d\n", cub->hero.ori, cub->skymap_tex_offset);
	return (0);
}

void	on_update(void *param)
{
	t_cub	*cub;
	float	d_walk;
	float	d_strafe;

	cub = (t_cub *)param;
	d_walk = 0;
	d_strafe = 0;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_W))
		d_walk += 1;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_S))
		d_walk -= 1;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_A))
		d_strafe -= 1;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_D))
		d_strafe += 1;
	if (d_walk || d_strafe)
		cub_player_move(cub, d_walk, d_strafe);
	if (cub->renderer.requires_update)
		render_scene(cub);
}

int	main(int argc, char **argv)
{
	t_cub		cub;
	
	if (argc != 2)
		return (EXIT_FAILURE);
	ft_memclear(&cub, sizeof(cub));
	cub_init_core_data(&cub);

	// INIT INPUT	
	if (map_checker(&cub, init_map(&cub.map), argv[1]) != 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	if (set_player_cell_pos(&cub, cub.map.pos_x, cub.map.pos_y, cub.map.hero_side) != 0)
	 	return (cub_clear(&cub, EXIT_FAILURE));

	// Window Width, Height, title, is_resizable. (option possible pour la window : full screen mode)
	cub.mlx = mlx_init(SCN_WIDTH, SCN_HEIGHT, "(cub)^3.D", 0);
	if (!cub.mlx)
		return (cub_clear(&cub, report_mlx_init_error()));	
	mlx_focus(cub.mlx);
	printf("Init mlx SUCCESSFUL !\n");
	
	printf("Try init renderer\n");
	if (init_renderer(&cub) < 0)// || init_floorcaster(&cub) < 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	printf("Init renderer SUCCESSFUL !\n");
	
	if (init_raycaster(&cub) < 0)
		return (cub_clear(&cub, EXIT_FAILURE));

	// INIT CURSOR SETTINGS
	mlx_set_mouse_pos(cub.mlx, cub.scn_midx, cub.scn_midy);
	mlx_set_cursor_mode(cub.mlx, MLX_MOUSE_HIDDEN);

	// INIT HOOKS
	mlx_cursor_hook(cub.mlx, &on_cursor_move, &cub);
	mlx_key_hook(cub.mlx, &cub_key_handler, &cub);
	mlx_loop_hook(cub.mlx, *on_update, &cub);
	mlx_scroll_hook(cub.mlx, &on_scroll, &cub);
	mlx_close_hook(cub.mlx, &on_close, &cub);

//	printf("Starting mlx loop\n");
	mlx_loop(cub.mlx);
	if (mlx_errno)
		printf("mlx loop stopped with ERROR ! : %s\n", mlx_strerror(mlx_errno));
	printf("mlx loop stopped !\n");
	return (cub_clear(&cub, EXIT_SUCCESS));
}
