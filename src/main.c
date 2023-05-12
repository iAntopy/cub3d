/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:07:26 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/12 17:30:20 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	cub_clear(t_cub *cub, int exit_status)
{
//	int	i;

	printf("CUB CLEAR AT EXIT\n");
	stop_draw_threads(cub->draw_threads);
//	i = -1;
//	while (++i < 4)
//		if (cub->tex.walls[i])
//			mlx_delete_texture(cub->tex.walls[i]);
	ft_free_p((void **)&cub->map.collision_map);
//	strtab_clear(&cub->map.tab);
	strtab_clear((char ***)&cub->map.grid_coords);
	raycaster_clear(&cub->hero.rcast, EXIT_SUCCESS);
	renderer_clear(cub);
	clear_floorcaster(cub);
	clear_obj_framework(cub);
//	clear_skycaster(cub);
	if (cub->mlx)
		mlx_terminate(cub->mlx);
	printf("exit with status : %d\n", exit_status);
	return (exit_status);
}

int	cub_init_core_data(t_cub *cub)
{
	cub->scn_midx = SCN_WIDTH / 2;
	cub->scn_midy = SCN_HEIGHT / 2;
	cub->inv_cw = 1.0f / (float)CELL_WIDTH;
	cub->inv_sw = 1.0f / (float)SCN_WIDTH;
	cub->inv_two_pi = 0.5f / M_PI;
	cub->near_z = (float)cub->scn_midx / tanf(FOV_HF);
	cub->near_proj_factor = CELL_WIDTH * cub->near_z;
	return (0);
}

int	set_player_cell_pos(t_cub *cub, int x, int y)
{
	printf("Player (x, y) : (%d, %d)\n", x, y);
	if (is_wall(&cub->map, x, y))
		return (printf("ERROR hero can't be placed in wall."));
	cub->hero.cell_x = x;
	cub->hero.cell_y = y;
	cub->hero.px = x * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.py = y * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.ori = 0;//M_PI + (M_PI / 2) * cub->map.hero_side;
	cub->renderer.requires_update = 1;
	cub->map.width_px = cub->map.width * CELL_WIDTH;
	cub->map.height_px = cub->map.height * CELL_WIDTH;
	return (0);
}

void	cub_setup_mlx_hooks_and_settings(t_cub *cub)
{
	printf("Setting up hooks and focus\n");
	mlx_focus(cub->mlx);
	mlx_set_cursor_mode(cub->mlx, MLX_MOUSE_HIDDEN);
	mlx_cursor_hook(cub->mlx, on_cursor_move, cub);
	mlx_key_hook(cub->mlx, (void (*)(struct mlx_key_data, void *))on_keypress, cub);
	mlx_loop_hook(cub->mlx, (void (*)(void *))on_update, cub);
	mlx_scroll_hook(cub->mlx, on_scroll, cub);
	mlx_close_hook(cub->mlx, on_close, cub);
}

int	main(int argc, char **argv)
{
	t_cub		cub;
	
	if (argc != 2)
		return (EXIT_FAILURE);
	ft_memclear(&cub, sizeof(cub));
	cub_init_core_data(&cub);
	if (map_checker(&cub, &cub.map, argv[1]) != 0
		|| set_player_cell_pos(&cub, cub.map.hero_x, cub.map.hero_y) != 0)
	{
		ft_eprintf("WOWOW map checker failed HARD !\n");
		return (cub_clear(&cub, EXIT_FAILURE));
	}
	printf("Initializing MLX42 context.\n");
	cub.mlx = mlx_init(SCN_WIDTH, SCN_HEIGHT, "(cub)^3.D", 0);
	printf("What happened MLX42 context.\n");
	if (!cub.mlx)
		return (cub_clear(&cub, report_mlx_init_error()));
	printf("MLX42 context initialized successfully !\n");

	/// FOR DEBUG PURPOSES ONLY ! DELETE ME !
	cub.pset[0].xwalls[1] = cub.pset[1].xwalls[0];


	printf("cub->tex.skymap : %p, box sky : %p\n", cub.tex.skymap, cub.box.sky);
	if (init_renderer(&cub) < 0 || init_floorcaster(&cub) < 0
		|| init_raycaster(&cub) < 0 || init_skycaster(&cub) < 0)
//		|| init_draw_threads(&cub, cub.draw_threads) < 0)
//		|| init_obj_framework(&cub))
		return (cub_clear(&cub, EXIT_FAILURE));


/////// FOR DEBUG PURPOSES ONLY ! DELETE ME !
/*
	int	pos[2];
	pos[0] = (cub.map.width - 3) * CELL_WIDTH;
	pos[1] = CELL_WIDTH * 2;
	
	int	portal1_id = create_obj_instance(&cub, pos, OBJ_PORTAL, NULL);
	
	pos[0] = 7 * CELL_WIDTH;
	pos[1] = 10 * CELL_WIDTH;

	create_obj_instance(&cub, pos, OBJ_PORTAL, get_oinst_by_id(&cub, portal1_id));
//	if (activate_portal(cub.objs.instances, OBJ_ACTIVATE) < 0)
//		printf("Portal activation FAILURE\n");
//	else
//		printf("Portal activation SUCCESS\n");
	printf("portal ptr : %p\n", cub.objs.instances);

//	mlx_set_color_in_rows(cub.renderer.mmap_layer, 0, 15, 0xffffffff);
	pos[0] = 125;
	pos[1] = 100;
	mlx_draw_square(cub.renderer.mmap_layer, pos, 50, 0xffffffff);
*/
/////// END OF DEBUG SECTION


	cub_setup_mlx_hooks_and_settings(&cub);
	printf("Party time babyyyyy !\n");
	mlx_loop(cub.mlx);
	ft_free_p((void **)&cub.box.chrs);
	if (mlx_errno)
		return (cub_clear(&cub,
				report_err_strerror("mlx loop stopped with ERROR ! : %s")));
	return (cub_clear(&cub, EXIT_SUCCESS));
}
