/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 21:55:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/29 22:33:36 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	pixel_is_inbound(mlx_image_t *img, int x, int y)
{
	return ((x >= 0) && (y >=0)
		&& ((uint32_t)x < img->width) && ((uint32_t)y < img->height));
}

static void	swap_limits(int *lims[2], int ds[2])
{
	int	*temp;

	temp = lims[0];
	lims[0] = lims[1];
	lims[1] = temp;
	ds[0] = -ds[0]; 
	ds[1] = -ds[1];
}

static void	draw_line_low(mlx_image_t *img, int *lims[2], int ds[2], int col)
{
	int	D;
	int	yi;
	int	dy_min_dx;
	int	x;
	int	y;

	x = lims[0][0] - 1;
	y = lims[0][1];
	yi = 1 - (2 * (ds[1] < 0));
	ds[1] *= yi;
	D = (2 * ds[1]) - ds[0];
	dy_min_dx = ds[1] - ds[0];
	while (++x < lims[1][0])
	{
		if (pixel_is_inbound(img, x, y))			// TODO : MOVE CLIPPING OUTSIDE DRAW CALL.
			cub_put_pixel(img, x, y, col);
		if (D > 0)
		{
			y += yi;
			D += 2 * dy_min_dx;
		}
		else
			D += 2 * ds[1];
	}
}

static void	draw_line_high(mlx_image_t *img, int *lims[2], int ds[2], int col)
{
	int	D;
	int	xi;
	int	dx_min_dy;
	int	x;
	int	y;

	y = lims[0][1] - 1;
	x = lims[0][0];
	xi = 1 - (2 * (ds[0] < 0));
	ds[0] *= xi;
	D = (2 * ds[0]) - ds[1];
	dx_min_dy = ds[0] - ds[1];
	while (++y < lims[1][1])
	{
		if (pixel_is_inbound(img, x, y))
			cub_put_pixel(img, x, y, col);
//			mlx_buff_put_pixel(mlx->off_buff, x, y, col);
		if (D > 0)
		{
			x += xi;
			D += 2 * dx_min_dy;
		}
		else
			D += 2 * ds[0];
	}
}

void	mlx_draw_line(mlx_image_t *img, int start[2], int end[2], int color)
{
	int	deltas[2];
	int	*limits[2];

	deltas[0] = end[0] - start[0];
	deltas[1] = end[1] - start[1];
	limits[0] = start;
	limits[1] = end;
	if (abs(deltas[1]) < abs(deltas[0]))
	{
//		printf("draw line dx > dy, swap : %d\n", (start[0] > end[0]));
		if (start[0] > end[0])
		{
			swap_limits(limits, deltas);
//			printf("SWAP!\n");
		}
//		printf("draw line low : from (%i, %i) to delta (%i, %i), color %d\n", *limits[0], *limits[1], deltas[0], deltas[1], color);
		draw_line_low(img, limits, deltas, color);
	}
	else
	{
//		printf("draw line dy > dx, swap : %d\n", (start[1] > end[1]));
		if (start[1] > end[1])
		{
			swap_limits(limits, deltas);
//			printf("SWAP!\n");
		}
		draw_line_high(img, limits, deltas, color);
	}
}
