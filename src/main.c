/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:07:26 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/12 19:12:41 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	cub_clear(t_cub *cub, int exit_status)
{
	t_hero	*h;

	printf("CUB CLEAR AT EXIT\n");
	renderer_clear(cub);
	floorcaster_clear(cub);
	printf("renderer_cleared\n");
	if (cub->mlx)
		mlx_terminate(cub->mlx);
	printf("mlx_terminated\n");
	h = &cub->hero;
	printf("clearing all mtx matrices\n");
	mtx_clear_list(10, h->theta_offsets, h->ray_thetas,
		h->rays[0], h->rays[1], h->coll_walls, h->coll_sides,
		h->collisions, h->fisheye_correctors,
		h->distances, h->tex_infos);
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
	if (event.key == MLX_KEY_ESCAPE && event.action == MLX_PRESS)
		on_close(param);
	else if (event.key == MLX_KEY_W && event.action == MLX_PRESS)
		cub_player_move(cub, 10, 0);
	else if (event.key == MLX_KEY_S && event.action == MLX_PRESS)
		cub_player_move(cub, -10, 0);
	else if (event.key == MLX_KEY_A && event.action == MLX_PRESS)
		cub_player_move(cub, 0, -10);
	else if (event.key == MLX_KEY_D && event.action == MLX_PRESS)
		cub_player_move(cub, 0, 10);
}

void	on_scroll(double deltax, double deltay, void *param)
{
	t_cub	*cub;

	(void)deltax;
	cub = (t_cub *)param;
	if (deltax)
		cub_player_zoom(cub, deltax / 10);
	else
		cub_player_zoom(cub, deltay / 10);
}

void	on_cursor_move(double xpos, double ypos, void *param)
{
	t_cub	*cub;
	double	dx;
	//	double	dy;

		cub = (t_cub *)param;
		(void)cub;
		(void)ypos;
		dx = xpos - cub->scn_midx;
	//	dy = ypos - cub->scn_midy;
	//	cub->hero.ori += dx * ROT_FACTOR;
	//	printf("Cursor moved : pos (x, y) : (%lf, %lf), delta (dx, dy) : (%lf, %lf), ori : %f\n", xpos, ypos, dx, dy,
	//		cub->hero.ori);
		cub_player_rotate(cub, dx * ROT_FACTOR);
	//	update_rays(cub);
	//	render_scene(cub);
		// CAN'T TOUCH THIS
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
	if (get_is_wall(&cub->map, x, y))
		return (printf("ERROR hero can't be placed in wall."));
	cub->hero.cell_x = x;
	cub->hero.cell_y = y;
	cub->hero.px = x * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.py = y * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.ori = M_PI - side * cub->inv_two_pi;
//	cub->hero.ori_factor = fabsf(cub->hero.ori * cub->inv_two_pi);
	cub->skymap_tex_offset = (int)(cub->hero.ori * cub->skymap_radial_width);
	printf("SET PLAYER POS : ori : %f, sky_tex_offset : %d\n", cub->hero.ori, cub->skymap_tex_offset);
	return (0);
}

int	main(int argc, char **argv)
{
	t_cub		cub;
	// float		*hero_cell_coord;
	
	ft_memclear(&cub, sizeof(cub));
	if (argc != 2)
		return (EXIT_FAILURE);
	cub_init_core_data(&cub);
	//	if (load_map(&cub, argv[1]) < 0)
	//		return (cub_clear(&cub, EXIT_FAILURE));

		// if (set_player_cell_pos(&cub, 1, 5, 0.0f) != 0)
		// 	return (cub_clear(&cub, EXIT_FAILURE));
		// hero_cell_coord = get_grid_coords(&cub.map, cub.hero.cell_x, cub.hero.cell_y);
		// printf("hero cell indexes : (%d, %d), hero cell coord : (%.3f, %.3f), hero pos : (%.2f, %.2f), hero orientation : %.5f\n",
		// 	cub.hero.cell_x, cub.hero.cell_y, hero_cell_coord[0], hero_cell_coord[1],
		// 	cub.hero.px, cub.hero.py, cub.hero.ori);

	// FONCTION DE PARSING VIENT ICI !!
	// INIT INPUT	
	if (map_checker(&cub, init_map(&cub.map), argv[1]) != 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	if (set_player_cell_pos(&cub, cub.map.pos_x, cub.map.pos_y, cub.map.hero_side) != 0)
	 	return (cub_clear(&cub, EXIT_FAILURE));

//	return (cub_clear(&cub, EXIT_SUCCESS));
	printf("DEBUG: MAP_CHK END:: TEX _START!\n");
	// if (parsing_func_de_fou_debile(&cub, argc, argv) < 0)
	//	return (EXIT_FAILURE);

	// Window Width, Height, title, is_resizable. (option possible pour la window : full screen mode)
	cub.mlx = mlx_init(SCN_WIDTH, SCN_HEIGHT, "(cub)^3.D", 0);
	if (!cub.mlx)
	{
		printf("MLX init failed \n");
		return (cub_clear(&cub, EXIT_FAILURE));	
	}

	mlx_focus(cub.mlx);
//	cub.tex.walls[0] = mlx_load_png("tex/w_side.png");
	// cub.tex.walls[0] = mlx_load_png(cub.tex.tex_n[0]);	
	printf("OYE OYE! Try init Walls ::%p:: \n",cub.tex.walls[0]);		
	if ((cub.tex.walls[0]))
		printf("Try init Walls W[%d] H[%d] \n", cub.tex.walls[0]->width, cub.tex.walls[0]->height);
	else 
		printf("Try init Walls FAILS \n");	
//	 return (0);


	cub.tex.floor = mlx_load_png("tex/floor.png");
	if (!cub.tex.floor)
		return (cub_clear(&cub, EXIT_FAILURE));
	printf("Floor texture loaded SUCCESSFULLY !\n");
	
	cub.tex.skymap = mlx_load_png("tex/sky_star.png");
	if (!cub.tex.skymap)
	{
		printf("loading skymap FAILED !! ptr : %p\n", cub.tex.skymap);
		return (cub_clear(&cub, EXIT_FAILURE));
	}
	printf("Skymap texture loaded SUCCESSFULLY !\n");


	//// ESSENTIAL DATA FOR SKYMAP !! ////////
	cub.skymap_radial_width = cub.tex.skymap->width * cub.inv_two_pi;// skymap.width / 2pi



	printf("Init mlx SUCCESSFUL !\n");
	printf("Try init renderer\n");
	if (init_renderer(&cub) < 0 || init_floorcaster(&cub) < 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	printf("Init renderer SUCCESSFUL !\n");
	
	if (init_raycaster(&cub) < 0)
		return (cub_clear(&cub, EXIT_FAILURE));

//	ft_deltatime_usec_note(NULL);
	// if (raycast_all_vectors(&cub) < 0)
		// return (cub_clear(&cub, EXIT_FAILURE));

//	ft_deltatime_usec_note("Raycaster results are in. What say you ?!\n");
	
	// return (0);
	// return (cub_clear(&cub, EXIT_SUCCESS));
	

		/// mlx_texture_to_image
		// 	cub.tex.walls[0] = mlx_load_png("tex/w_side.png");
		// 	if (!(cub.tex.walls[0]))
		// 		error("B. You are trying but no png to tex.\n", map);
		// 	// 	set img to be display
		// 	cub.imgz = mlx_texture_to_image(cub.mlx, cub.tex.walls[0]);
		// 	if (!cub.imgz)
		// 		error("C. You are trying to open img but no img.\n", map);
		// /// image_to_window

		// ///test_img_to_window
		// 	cub.color = mlx_new_image(cub.mlx, 128, 128);
		// 	mlx_image_to_window(cub.mlx, cub.color, 10, 10);
	
	// INIT CURSOR SETTINGS
	mlx_set_mouse_pos(cub.mlx, cub.scn_midx, cub.scn_midy);
	mlx_set_cursor_mode(cub.mlx, MLX_MOUSE_HIDDEN);

	
	// INIT HOOKS
	mlx_cursor_hook(cub.mlx, &on_cursor_move, &cub);
	mlx_key_hook(cub.mlx, &cub_key_handler, &cub);
	mlx_scroll_hook(cub.mlx, &on_scroll, &cub);
	mlx_close_hook(cub.mlx, &on_close, &cub);
	

	printf("Starting mlx loop\n");
	mlx_loop(cub.mlx);
	printf("mlx loop stopped !\n");
	return (cub_clear(&cub, EXIT_SUCCESS));
}
