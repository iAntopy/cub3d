/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 18:58:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/15 19:51:28 by gehebert         ###   ########.fr       */
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
void mlx_draw_player(t_cub * Cub, t_map *m)
{
        mlx_draw_square(mlx_image_t *img, int pos[2], int side, uint32_t col)
}

/// set map_char to map_pixel

////    set map_min_max 


void    minimap_set_pos(t_cub *cub, t_map *m)
{
        int mminx;
        int mminy;
        int mmaxx;
        int mmaxy;

        mminx = m->pos_x + 5;
        mminy = m->pos_y + 5;
    
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