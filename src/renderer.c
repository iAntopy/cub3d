/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/30 18:38:07 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	renderer_clear(t_cub *cub)
{
//	printf("renderer clear entered \n");
	if (cub->renderer.bg_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
	if (cub->renderer.sky_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
//	printf("renderer clear bg DONE \n");
	if (cub->renderer.walls_layer)
		mlx_delete_image(cub->mlx, cub->renderer.walls_layer);
//	printf("renderer clear walls DONE \n");
//	if (cub->renderer.proj_layer)
//		mlx_delete_image(cub->mlx, cub->renderer.objs_layer);
//	printf("renderer clear walls DONE \n");
	if (cub->renderer.objs_layer)
		mlx_delete_image(cub->mlx, cub->renderer.objs_layer);
//	printf("renderer clear objs DONE\n");
	if (cub->renderer.mmap_layer)
		mlx_delete_image(cub->mlx, cub->renderer.mmap_layer);
	if (cub->renderer.dbuff)
		ft_free_p((void **)&cub->renderer.dbuff);
	if (cub->renderer.dpbuff)
		ft_free_p((void **)&cub->renderer.dpbuff);
	if (cub->renderer.isproj)
		ft_free_p((void **)&cub->renderer.isproj);
//	printf("renderer clear mmap DONE : SUCCESS \n");
	return (0);
}

uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y)
{
//	if (x < 0 || y < 0)
//		return (0);
//	printf("px: x %d, y %d", x, y);
	return (((uint32_t *)tex->pixels)[x + y * tex->width]);
}

// Called only once at start to initialize mlx image buffers by layer.
int	init_renderer(t_cub *cub)
{
	int j;
	
	cub->renderer.sky_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT >> 1);
	cub->renderer.bg_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.walls_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
//	cub->renderer.proj_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.objs_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.mmap_layer = mlx_new_image(cub->mlx, MMP_WIDTH, MMP_HEIGHT);

	if (!cub->renderer.bg_layer || !cub->renderer.walls_layer
//		|| !cub->renderer.proj_layer
		|| !cub->renderer.objs_layer
		|| !cub->renderer.mmap_layer)
		return (-1);

	cub->map.mmp_mid[0] = MMP_WIDTH >> 1;
	cub->map.mmp_mid[1] = MMP_HEIGHT >> 1;
	cub->map.mmp_ply_off[0] = cub->map.mmp_mid[0] - 5;//
	cub->map.mmp_ply_off[1] = cub->map.mmp_mid[1] - 5;// = MMP_HEIGHT >> 1;
	cub->map.map_to_world_x_rt = CELL_WIDTH * 10.0f / (float)MMP_WIDTH;
	cub->map.world_to_map_x_rt = 1.0f / cub->map.map_to_world_x_rt;//CELL_WIDTH * 10.0f / (float)MMP_WIDTH;
	cub->map.map_to_world_y_rt = CELL_WIDTH * 10.0f / (float)MMP_HEIGHT;
	cub->map.world_to_map_y_rt = 1.0f / cub->map.map_to_world_y_rt;//CELL_WIDTH * 10.0f / (float)MMP_WIDTH;
//	if (!ft_malloc_p(2 * sizeof(float) * SCN_WIDTH * SCN_HEIGHT,
//			(void **)&cub->renderer.dbuff))// 2 rendering depth buffers. 1st: world, 2nd: portal projection.
//		return (-1);
//	cub->renderer.dpbuff = cub->renderer.dbuff + sizeof(float) * SCN_WIDTH * SCN_HEIGHT;//	ref to projection 
											//	depth buff
	if (!ft_calloc_p(sizeof(float) * SCN_WIDTH * SCN_HEIGHT,
			(void **)&cub->renderer.dbuff)
		|| !ft_calloc_p(sizeof(float) * SCN_WIDTH * SCN_HEIGHT,
			(void **)&cub->renderer.dpbuff)
		|| !ft_calloc_p(sizeof(char) * SCN_WIDTH * SCN_HEIGHT,
			(void **)&cub->renderer.isproj))// 2 rendering depth buffers. 1st: world, 2nd: portal projection.
		return (-1);
//	mlx_set_color_in_rows(cub->renderer.bg_layer,
//		0, SCN_HEIGHT >> 1, cub->tex.color[0]);
//	mlx_set_color_in_rows(cub->renderer.bg_layer,
//		SCN_HEIGHT >> 1, SCN_HEIGHT, cub->tex.color[1]);
	mlx_set_color_in_rows(cub->renderer.mmap_layer,
		0, MMP_HEIGHT, 0x7f7a4e3b);
	mlx_image_to_window(cub->mlx, cub->renderer.sky_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.bg_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.walls_layer, 0, 0);
//	mlx_image_to_window(cub->mlx, cub->renderer.proj_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.objs_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.mmap_layer,
		(int)(SCN_WIDTH * 0.03f),
		SCN_HEIGHT - MMP_HEIGHT - (int)(SCN_HEIGHT * 0.03f));
	j = -1;
	while (++j < SCN_HEIGHT)
		cub->buff_offys[j] = j * SCN_WIDTH;
	return (0);
}
