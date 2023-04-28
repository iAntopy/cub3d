/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/28 10:36:22 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	renderer_clear(t_cub *cub)
{
	printf("renderer clear entered \n");
	if (cub->renderer.bg_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
	if (cub->renderer.walls_layer)
		mlx_delete_image(cub->mlx, cub->renderer.walls_layer);
	if (cub->renderer.objs_layer)
		mlx_delete_image(cub->mlx, cub->renderer.objs_layer);
	if (cub->renderer.mmap_layer)
		mlx_delete_image(cub->mlx, cub->renderer.mmap_layer);
	return (0);
}

// Called only once at start to initialize mlx image buffers by layer.
int	init_renderer(t_cub *cub)
{
	cub->renderer.bg_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.walls_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.objs_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.mmap_layer = mlx_new_image(cub->mlx, MMP_WIDTH, MMP_HEIGHT);
	if (!cub->renderer.bg_layer || !cub->renderer.walls_layer
		|| !cub->renderer.mmap_layer)
		return (-1);
	mlx_set_color_in_rows(cub->renderer.bg_layer,
		0, SCN_HEIGHT >> 1, cub->tex.color[0]);
	mlx_set_color_in_rows(cub->renderer.bg_layer,
		SCN_HEIGHT >> 1, SCN_HEIGHT, cub->tex.color[1]);
	mlx_set_color_in_rows(cub->renderer.mmap_layer,
		0, MMP_HEIGHT, 0x7f7a4e3b);
	mlx_image_to_window(cub->mlx, cub->renderer.bg_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.walls_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.objs_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.mmap_layer,
		(int)(SCN_WIDTH * 0.03f),
		SCN_HEIGHT - MMP_HEIGHT - (int)(SCN_HEIGHT * 0.03f));
	return (0);
}
