/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/12 17:55:33 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	renderer_clear(t_cub *cub)
{
	printf("renderer clear entered \n");
	if (cub->renderer.bg_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
	if (cub->renderer.walls_layer)
		mlx_delete_image(cub->mlx, cub->renderer.walls_layer);
	return (0);
}

void	clear_image_buffer(mlx_image_t *img)
{
	ft_memclear(img->pixels, img->width * img->height * sizeof(int));
}

void	__mlx_fill_pixels(uint8_t *pixels, size_t total_bytes, int col)
{
	size_t	*b;
	int		*p;
	size_t	xl;

	p = (int *)&xl;
	*p = col;
	*(p + 1) = col;
	b = (size_t *)pixels;
	while (total_bytes >= sizeof(size_t))
	{
		*(b++) = xl;
		total_bytes -= sizeof(size_t);
	}
	p = (int *)b;
	while (total_bytes >= sizeof(int))
	{
		*(p++) = col;
		total_bytes -= sizeof(int);
	}
}

void	mlx_set_bg_color_traditional(mlx_image_t *img, uint32_t col)
{
	uint32_t	i;
	uint32_t	j;

	i = -1;
	while (++i < img->height)
	{
		j = -1;
		while (++j < img->width)
			mlx_put_pixel(img, j, i, col);
	}
}
/*
void	mlx_set_bg_color(mlx_image_t *img, int col)
{
	__mlx_fill_pixels(img->pixels, img->width * img->height, col);
}
*/

void	mlx_set_color_in_rows(mlx_image_t *img, int start, int end, int col)
{
	__mlx_fill_pixels(img->pixels + (sizeof(uint32_t) * start * img->width),
		sizeof(int) * (end - start) * img->width, col);
}

void	cub_put_pixel(mlx_image_t *img, int x, int y, int col)
{
	((uint32_t *)img->pixels)[x + y * img->width] = col;
}
/*
int	get_texture_pixel(mlx_texture_t *tex, int x, int y)
{
//	printf("get_tex_pix : skymap width : %d, x : %d, y : %d\n", tex->width, x, y);
//	if (x < 0 || y < 0)
//	{
//		dprintf(2, "x or y < 0 : %d, %d\n", x, y);
//		return 0xffffffff;
//	}
	return (((uint32_t *)tex->pixels)[x + y * tex->width]);
}
*/

void	render_walls(t_cub *cub)
{
	int		i;
	int		j;

	t_rdata		*rd;

	int		half_texh;
	int		scn_height;
	int		half_height;
	
	int		scn_start_y;
	int		tex_start_x;
	mlx_texture_t	*tex;
	uint32_t	*pxls;
	float		ratio;
	
	rd = cub->hero.rcast.rdata - 1;

	clear_image_buffer(cub->renderer.walls_layer);

	i = -1;
	while (++i < SCN_WIDTH)
	{
		++rd;
		tex = cub->tex.walls[rd->side];
		half_texh = (tex->height >> 1);
		
		tex_start_x = (int)(rd->tex_ratio * tex->width);
		scn_height = ft_clamp(rd->tex_height, 0, SCN_HEIGHT);
		half_height = (scn_height >> 1);

		ratio = (float)tex->height / (float)rd->tex_height;
		pxls = (uint32_t *)tex->pixels + tex_start_x;

//		printf("scn_fheight : %d, scn_height : %d, tex_start_x : %d\n", scn_fheight, scn_height, tex_start_x);
		scn_start_y = ((SCN_HEIGHT - scn_height) >> 1);// divide by 2. (SCN_HEIGHT / 2 - height / 2)

//////////////	WALLS RENDERING ///////////////////
		j = -1;
		while (++j < scn_height)
			cub_put_pixel(cub->renderer.walls_layer, i, scn_start_y + j,
				pxls[(int)(((j - half_height) * ratio) + half_texh) * tex->width]);

	}
}

// Called anytime a noticeable change is made
void	render_scene(t_cub *cub)
{
//	printf("(((_______( RENDERING THAT SCENE BABYY !! )_________)))\n\n");
	render_walls(cub);
	cub->renderer.requires_update = 0;
}

// Called only once at start
int	init_renderer(t_cub *cub)
{
	printf("init renderer : start. mlx : %p\n", cub->mlx);

	cub->renderer.bg_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.walls_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);

	if (!cub->renderer.bg_layer || !cub->renderer.walls_layer)
		return (-1);
	mlx_set_color_in_rows(cub->renderer.bg_layer, 0, SCN_HEIGHT >> 1, cub->tex.color[0]);
	mlx_set_color_in_rows(cub->renderer.bg_layer, SCN_HEIGHT >> 1, SCN_HEIGHT, cub->tex.color[1]);

	mlx_image_to_window(cub->mlx, cub->renderer.bg_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.walls_layer, 0, 0);

	printf("init renderer : exit \n");
	return (0);
}
