/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 21:11:02 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 20:55:47 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

void	mlx_set_color_in_rows(mlx_image_t *img, int start, int end, int col)
{
	__mlx_fill_pixels(img->pixels + (sizeof(uint32_t) * start * img->width),
		sizeof(int) * (end - start) * img->width, col);
}

void	mlx_draw_square(mlx_image_t *img, int pos[2], int side, uint32_t col)
{
	int			i;
	uint32_t	*pxls;

	pxls = (uint32_t *)img->pixels + pos[0] + pos[1] * img->width;
	i = -1;
	while (++i < side)
		__mlx_fill_pixels((uint8_t *)(pxls + (i * img->width)), side * sizeof(uint32_t), col);
}

void	cub_put_pixel(mlx_image_t *img, int x, int y, int col)
{
	((uint32_t *)img->pixels)[x + y * img->width] = col;
}
