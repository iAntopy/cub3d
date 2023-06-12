/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/08 13:24:58 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	renderer_clear(t_cub *cub, int exit_status)
{
	if (cub->renderer.bg_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
	if (cub->renderer.sky_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
	if (cub->renderer.walls_layer)
		mlx_delete_image(cub->mlx, cub->renderer.walls_layer);
	if (cub->renderer.objs_layer)
		mlx_delete_image(cub->mlx, cub->renderer.objs_layer);
	if (cub->renderer.mmap_layer)
		mlx_delete_image(cub->mlx, cub->renderer.mmap_layer);
	if (cub->renderer.dbuff)
		ft_free_p((void **)&cub->renderer.dbuff);
	if (cub->renderer.dpbuff)
		ft_free_p((void **)&cub->renderer.dpbuff);
	if (cub->renderer.isproj)
		ft_free_p((void **)&cub->renderer.isproj);
	return (exit_status);
}

uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y)
{
	return (((uint32_t *)tex->pixels)[x + y * tex->width]);
}

static int	__setup_base_renderer_and_minimap_data(t_cub *cub)
{
	int	j;

	if (!ft_calloc_p(sizeof(float) * SCN_WIDTH * SCN_HEIGHT,
			(void **)&cub->renderer.dbuff)
		|| !ft_calloc_p(sizeof(float) * SCN_WIDTH * SCN_HEIGHT,
			(void **)&cub->renderer.dpbuff)
		|| !ft_calloc_p(sizeof(char) * SCN_WIDTH * SCN_HEIGHT,
			(void **)&cub->renderer.isproj))
		return (-1);
	cub->map.mmp_mid[0] = MMP_WIDTH >> 1;
	cub->map.mmp_mid[1] = MMP_HEIGHT >> 1;
	cub->map.mmp_ply_off[0] = cub->map.mmp_mid[0] - 5;
	cub->map.mmp_ply_off[1] = cub->map.mmp_mid[1] - 5;
	cub->map.map_wld_x_rt = CELL_WIDTH * 10.0f / (float)MMP_WIDTH;
	cub->map.wld_map_x_rt = 1.0f / cub->map.map_wld_x_rt;
	cub->map.map_wld_y_rt = CELL_WIDTH * 10.0f / (float)MMP_HEIGHT;
	cub->map.wld_map_y_rt = 1.0f / cub->map.map_wld_y_rt;
	j = -1;
	while (++j < SCN_HEIGHT)
		cub->yoffs[j] = j * SCN_WIDTH;
	return (0);
}

// Called only once at start to initialize mlx image buffers by layer.
int	init_renderer(t_cub *cub)
{
	cub->renderer.sky_layer = mlx_new_image(cub->mlx,
			SCN_WIDTH, SCN_HEIGHT >> 1);
	cub->renderer.bg_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.walls_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.objs_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.mmap_layer = mlx_new_image(cub->mlx, MMP_WIDTH, MMP_HEIGHT);
	if (!cub->renderer.bg_layer || !cub->renderer.walls_layer
		|| !cub->renderer.objs_layer
		|| !cub->renderer.mmap_layer)
		return (renderer_clear(cub, report_err_strerror("Renderer init fail")));
	if (__setup_base_renderer_and_minimap_data(cub) < 0)
		return (renderer_clear(cub, report_malloc_error()));
	mlx_set_color_in_rows(cub->renderer.mmap_layer,
		0, MMP_HEIGHT, 0x7f7a4e3b);
	mlx_image_to_window(cub->mlx, cub->renderer.sky_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.bg_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.walls_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.objs_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.mmap_layer,
		(int)(SCN_WIDTH * 0.03f),
		SCN_HEIGHT - MMP_HEIGHT - (int)(SCN_HEIGHT * 0.03f));
	return (0);
}
