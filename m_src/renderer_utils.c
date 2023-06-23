/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 21:11:02 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/12 15:35:38 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	cub_put_pixel(mlx_image_t *img, int x, int y, int col)
{
	((uint32_t *)img->pixels)[x + y * img->width] = col;
}
