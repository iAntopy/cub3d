/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:07:26 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/27 15:41:27 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	cub_clear(t_cub *cub, int exit_status)
{
	int	i;

	ft_printf("CUB CLEANUP AT EXIT\n");
	i = -1;
	while (++i < 4)
		if (cub->tex.walls[i])
			mlx_delete_texture(cub->tex.walls[i]);
	ft_free_p((void **)&cub->map.collision_map);
	strtab_clear(&cub->map.tab);
	strtab_clear((char ***)&cub->map.grid_coords);
	raycaster_clear(&cub->hero.rcast, EXIT_SUCCESS);
	renderer_clear(cub);
	if (cub->mlx)
		mlx_terminate(cub->mlx);
	ft_printf("Good Bye !\n");
	return (exit_status);
}

int	cub_init_core_data(t_cub *cub)
{
	cub->scn_midx = SCN_WIDTH / 2;
	cub->scn_midy = SCN_HEIGHT / 2;
	cub->inv_cw = 1.0f / (float)CELL_WIDTH;
	cub->inv_sw = 1.0f / (float)SCN_WIDTH;
	cub->inv_two_pi = 0.5f / M_PI ;
	return (0);
}

int	set_player_cell_pos(t_cub *cub, int x, int y)
{
	printf("Player (x, y) : (%d, %d)\n", x, y);
	if (is_wall(&cub->map, x, y))
		return (ft_eprintf("Error\n\t - hero can't be placed in wall.\n"));
	cub->hero.cell_x = x;
	cub->hero.cell_y = y;
	cub->hero.px = x * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.py = y * CELL_WIDTH + (CELL_WIDTH / 2.0f);
	cub->hero.ori = M_PI + (M_PI / 2) * cub->map.hero_side;
	cub->track_mouse = 1;
	cub->renderer.requires_update = 1;
	return (0);
}

void	cub_setup_mlx_hooks_and_settings(t_cub *cub)
{
	int	wpos[4];

	mlx_focus(cub->mlx);
	mlx_set_cursor_mode(cub->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_window_pos(cub->mlx, wpos, wpos + 1);
	mlx_set_mouse_pos(cub->mlx, wpos[0] + cub->scn_midx,
		wpos[1] + cub->scn_midx + 1);
	mlx_cursor_hook(cub->mlx, on_cursor_move, cub);
	mlx_key_hook(cub->mlx, on_keypress, cub);
	mlx_loop_hook(cub->mlx, (void (*)(void *))on_update, cub);
	mlx_scroll_hook(cub->mlx, on_scroll, cub);
	mlx_close_hook(cub->mlx, on_close, cub);
}

int	main(int argc, char **argv)
{
	t_cub		cub;

	if (argc != 2)
		return (report_err("Must give one map file argument"),
			EXIT_FAILURE);
	ft_memclear(&cub, sizeof(cub));
	cub_init_core_data(&cub);
	if (map_checker(&cub, &cub.map, argv[1]) != 0
		|| set_player_cell_pos(&cub, cub.map.hero_x, cub.map.hero_y) != 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	cub.mlx = mlx_init(SCN_WIDTH, SCN_HEIGHT, "cub3D", 0);
	if (!cub.mlx)
		return (cub_clear(&cub, report_mlx_init_error()));
	cub_setup_mlx_hooks_and_settings(&cub);
	if (init_renderer(&cub) < 0 || init_raycaster(&cub) < 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	mlx_loop(cub.mlx);
	if (mlx_errno)
		return (cub_clear(&cub,
				report_err_strerror("mlx loop stopped with ERROR ! : %s")));
	return (cub_clear(&cub, EXIT_SUCCESS));
}
