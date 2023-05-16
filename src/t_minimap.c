/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 18:58:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/15 21:06:01 by gehebert         ###   ########.fr       */
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



// /// set player pos to center map 
// void mlx_draw_player(t_cub * cub, t_map *m)
// {
        
// 	mlx_set_color_in_rows(cub->renderer.mmap_layer, 0, 15, 0xffffffff);
// 	pos[0] = 50;
// 	pos[1] = 50;
// 	mlx_draw_square(cub.renderer.mmap_layer, pos, 50, 0xffffffff);
// }

/// set map_char to map_pixel

////    set map_min_max 


void    minimap_set_pos(t_cub *cub)
{
        int	pos[2];
        int mm_x;
        int mm_y;
        int mmaxx;
        int mmaxy;

        mm_x = cub->hero.cell_x; //m->pos_x + 5;
        mm_y = cub->hero.cell_y; 
        mmaxx = mm_x;
        mmaxy = mm_y;
        if  (mm_x < 5)
                mmax_x++;
        if  (mm_y < 5)
                mmax_y++;
        // if  (mm_x > 5) // map_height
        //         mmax_x++;
        // if  (mm_y < 5)       // map->wicht
        //         mmax_y++;
        // mlx_draw_player(cub , cub->map.m);
        
        mlx_set_color_in_rows(cub->renderer.mmap_layer, 0, 15, 0xffffffff);
	pos[0] = 50;
	pos[1] = 50;
	mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xffffffff);
    
}


