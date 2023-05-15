/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 18:58:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/14 19:27:10 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// void    mlx_draw_square(mlx_image_t *img, int pos[2], int side, uint32_t col)
        // {
        //     int         i;
        //     uint32_t    *pxls;
        //     pxls = (uint32_t *)img->pixels + pos[0] + pos[1] * img->width;
        //     i = -1;
        //     while (++i < side)
        //         __mlx_fill_pixels((uint8_t *)(pxls + (i * img->width)), side * sizeof(uint32_t), col);
        // }

        // void    mlx_draw_square(mlx_image_t *img, int pos[2], int side, uint32_t col)
        // {
        //     int         i;
        //     uint32_t    *pxls;
        //     pxls = (uint32_t *)img->pixels + pos[0] + pos[1] * img->width;
        //     i = -1;
        //     while (++i < side)
        //         __mlx_fill_pixels((uint8_t *)(pxls + (i * img->width)), side * sizeof(uint32_t), col);
// }
///



/// set player pos to center map 

/// set map_char to map_pixel

////    set map_min_max 


void    minimap_set_pos(t_cub *cub, t_map *m)
{
    
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