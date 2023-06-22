/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:07:26 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/21 20:47:32 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	cub_clear(t_cub *cub, int exit_status)
{
	printf("CUB CLEAR AT EXIT\n");
	stop_draw_threads(cub->draw_threads);
	ft_free_p((void **)&cub->map.collision_map);
	strtab_clear((char ***)&cub->map.grid_coords);
	raycaster_clear(&cub->hero.rcast, EXIT_SUCCESS);
	renderer_clear(cub, 0);
	clear_floorcaster(cub);
	clear_obj_framework(cub);
	if (cub->mlx)
		mlx_terminate(cub->mlx);
	clr_legend_strct(cub);
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
	cub->renderer.bframe[0] = 0;
	cub->renderer.bframe[1] = 0;
	cub->renderer.bframe[2] = SCN_WIDTH;
	cub->renderer.bframe[3] = SCN_HEIGHT;
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
	mlx_key_hook(cub->mlx,
		(void (*)(struct mlx_key_data, void *))on_keypress, cub);
	mlx_loop_hook(cub->mlx, (void (*)(void *))on_update, cub);
	mlx_scroll_hook(cub->mlx, on_scroll, cub);
	mlx_close_hook(cub->mlx, on_close, cub);
}

int	main(int argc, char **argv)
{
	t_cub	cub;

	if (argc != 2)
		return (EXIT_FAILURE);
	ft_memclear(&cub, sizeof(cub));
	cub_init_core_data(&cub);
	if (map_checker(&cub, &cub.map, argv[1]) != 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	cub.mlx = mlx_init(SCN_WIDTH, SCN_HEIGHT, "(cub)^3.D", 0);
	if (!cub.mlx)
		return (cub_clear(&cub, report_mlx_init_error()));
	printf("MLX42 context initialized successfully !\n");
	if (init_renderer(&cub) < 0 || init_floorcaster(&cub) < 0
		|| init_raycaster(&cub) < 0 || init_skycaster(&cub) < 0
		|| init_draw_threads(&cub, cub.draw_threads) < 0)
		return (cub_clear(&cub, EXIT_FAILURE));
	cub_setup_mlx_hooks_and_settings(&cub);
	printf("Party time babyyyyy !\n");
	mlx_loop(cub.mlx);
	if (mlx_errno)
		return (cub_clear(&cub, report_err_strerror("mlx loop exit error")));
	return (cub_clear(&cub, EXIT_SUCCESS));
}
