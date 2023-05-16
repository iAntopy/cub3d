/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 18:58:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/16 00:08:35 by gehebert         ###   ########.fr       */
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

void    minimap_surround(t_cub *cub, int pos[2])
{
	// int	pos[2];
	int mm[2];
	int mmax[2];
	char **mraz;
	mraz = cub->map.raw;
	mmax[0] =  pos[0] - 5;
	mmax[1] =  pos[1] - 5;
	// pos[0] = cub->hero.cell_x - 5;
	// pos[1] = cub->hero.cell_y - 5; 
	
	
			printf("<<<<>>>minim(x[%d], y[%d] \n",mmax[0], mmax[1]);//, mraz[mmax[0]][mmax[1]]);
			printf("<<<  char( x[%d], y[%d],  \n", pos[0],pos[1]);
			printf("<<<  char {%c}\n",  mraz[pos[0]][pos[1]]);
	while (mm[1] < 10 && mmax[1] > -5)
	{       
		mm[0] = 0;
		while (mm[0] < 10 && mmax[0] > -5)
		{
			if (mmax[0] < 0 || mmax[1] < 0) // uppercase // mmax pos. rel. -10/+10 map/minimap
				mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0xff66aaff);
			else if (mraz[mmax[0]][mmax[1]] >= 65 ) //uppercase
				mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0xff11aaff);
			else if (mraz[mmax[0]][mmax[1]] >= 48 ) //uppercase
				mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0x99ffaaff);
			else
				mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0xffffffff);
			mmax[0]++;
			mm[0]++;
		}
		mmax[1]++;
		mm[1]++;
	}

	// mmax_x = cub->map.height + 5;
	// mmax_y = cub->map.width - 5;      
	// mlx_draw_player(cub , cub->map.m);
        // mlx_set_color_in_rows(cub->renderer.mmap_layer, 0, 10, 0xffffffff);
	// pos[0] = cub->hero.cell_x * 10;
	// pos[1] = cub->hero.cell_y * 10;
	// mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xff66aaff);
        
}


////    set map_min_max 
void    minimap_set_pos(t_cub *cub)
{
        int	pos[2];
        // int mm_x;
        // int mm_y;
        int mmax_x;
        int mmax_y;

        mmax_x = cub->map.height + 5;
        mmax_y = cub->map.width - 5;      
        // mlx_draw_player(cub , cub->map.m);
        mlx_set_color_in_rows(cub->renderer.mmap_layer, 0, 10, 0xffffffff);
	pos[0] = cub->hero.cell_x * 10;
	pos[1] = cub->hero.cell_x * 10;
	minimap_surround(cub, pos);
	// mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xff66aaff);
    
}


