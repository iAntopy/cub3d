/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:07:26 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/31 21:23:30 by iamongeo         ###   ########.fr       */
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

static int	setup_hero(t_cub *cub)
{
//	printf("Player (x, y) : (%d, %d)\n", x, y);
//	if (is_wall(&cub->map, x, y))
//		return (printf("ERROR hero can't be placed in wall."));
	if (!cub->player_ids[0])
		return (report_err("Error : No player found in map\n"));
	cub->hero.ply_obj = get_obj(cub, cub->player_ids[0]);
//	cub->hero.cell_x = x;
//	cub->hero.cell_y = y;
//	cub->hero.px = x * CELL_WIDTH + (CELL_WIDTH / 2.0f);
//	cub->hero.py = y * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.ply_obj->ori = 0;//M_PI + (M_PI / 2) * cub->map.hero_side;
//	cub->hero.allegiance = ALI_NEUTRAL;
	cub->renderer.requires_update = 1;

	return (0);
}

void	cub_setup_mlx_hooks_and_settings(t_cub *cub)
{
	printf("Setting up hooks and focus\n");
	cub->map.width_px = cub->map.width * CELL_WIDTH;
	cub->map.height_px = cub->map.height * CELL_WIDTH;
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
		|| instanciate_map_objects(&cub) < 0
		|| setup_hero(&cub) != 0)
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


	printf("cub->tex.skymap : %p, box sky : %p\n", cub.tex.skymap, cub.box.sky);
	if (init_renderer(&cub) < 0 || init_floorcaster(&cub) < 0
		|| init_raycaster(&cub) < 0 || init_skycaster(&cub) < 0
		|| init_draw_threads(&cub, cub.draw_threads) < 0)
		return (cub_clear(&cub, EXIT_FAILURE));

	set_playable_obj(&cub, cub.hero.ply_obj);
		// || init_obj_framework(&cub) < 0
/*
/////// FOR DEBUG PURPOSES ONLY ! DELETE ME !

	int	pos[2];
//	pos[0] = (cub.map.width - 3) * CELL_WIDTH;
//	pos[1] = CELL_WIDTH * 2;
	
	pos[0] = 20 * CELL_WIDTH;
	pos[1] = 2 * CELL_WIDTH;
	int	portal1_id = create_obj_instance(&cub, pos, OBJ_PORTAL, ALI_LEGION, NULL);
	
	pos[0] = 20 * CELL_WIDTH;
	pos[1] = 7 * CELL_WIDTH;

	create_obj_instance(&cub, pos, OBJ_PORTAL, ALI_LEGION, get_obj(&cub, portal1_id));

	pos[0] = 5 * CELL_WIDTH + 1;
	pos[1] = 2 * CELL_WIDTH + 1;

	create_obj_instance(&cub, pos, OBJ_LEVER, ALI_NEUTRAL, get_obj(&cub, portal1_id));

	pos[0] = 10 * CELL_WIDTH + 32;
	pos[1] = 7 * CELL_WIDTH + 32;

	create_obj_instance(&cub, pos, OBJ_FIREBALL, ALI_NEUTRAL, NULL);
//	if (activate_portal(cub.objs.instances, OBJ_ACTIVATE) < 0)
//		printf("Portal activation FAILURE\n");
//	else
//		printf("Portal activation SUCCESS\n");
//	printf("portal ptr : %p\n", cub.objs.instances);

//	pos[0] = 3 * CELL_WIDTH;
//	pos[1] = 10 * CELL_WIDTH;
//	create_obj_instance(&cub, pos, OBJ_FIREPIT, &cub.hero);

/////// END OF DEBUG SECTION
*/

//	p_list_objx(cub.box.objx, 3, 0);
/*
	if (!cub.objs.portal.gset)
		init_portal_model(&cub.objs);
	if (!cub.objs.lever.gset)
		init_lever_model(&cub.objs);
	if (!cub.objs.fireball.gset)
		init_fireball_model(&cub.objs);
	if (!cub.objs.firepit.gset)
		init_firepit_model(&cub.objs);
*/
//	init_player_model(&cub.objs);

////// DEBUG CODE TO FORCE ALL mapx floor cells to have textures.

	int	i;
	int	j;
	i = -1;
	while (++i < cub.map.height)
	{
		j = -1;
		while (++j < cub.map.width)	
		{
//			printf("map cell gset ptr : %p, ", cub.map.mx[i][j]);
			if (!cub.map.mx[i][j] || !cub.map.mx[i][j]->xwalls[0])
				cub.map.mx[i][j] = &cub.dual[0];
		}
	}

//	float 	pos[2];
//	pos[0] = 13 * CELL_WIDTH + 1;
//	pos[1] = 2 * CELL_WIDTH + 1;
//	create_obj_instance(&cub, pos, OBJ_PLAYER, ALI_NEUTRAL, &cub.hero);

	cub_setup_mlx_hooks_and_settings(&cub);
	printf("Party time babyyyyy !\n");
	mlx_loop(cub.mlx);
	ft_free_p((void **)&cub.box.chrs);
	if (mlx_errno)
		return (cub_clear(&cub,
				report_err_strerror("mlx loop stopped with ERROR ! : %s")));
	return (cub_clear(&cub, EXIT_SUCCESS));
}
