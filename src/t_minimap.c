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

void    minimap_surround(t_cub *cub, int pos[2], int mmax[2])
{
	// int	pos[2];
	// int mmax[2];
	int ochrs;

	int mm[2];
	char **mraz;
	// t_map *m;

	mraz = cub->map.m;
	// m = &cub->map;
	mmax[0] =  pos[0] - 5;	// pos[0] = cub->hero.cell_x - 5;
	mmax[1] =  pos[1] - 5;	// pos[1] = cub->hero.cell_y - 5; 

	ochrs = ft_in_set((mraz[mmax[1]][mmax[0]]),(const char *)cub->box.chrs);
	
			printf("<<<<>>>minim(x[%d], y[%d] \n",mmax[0], mmax[1]);//, mraz[mmax[0]][mmax[1]]);
			printf("<<<  char( x[%d], y[%d],  \n", pos[0],pos[1]);
			printf("<<<  char {%d}\n", ochrs);
			// printf("<<<  char {%c}\n", cub->box.chrs[ochrs]);
			printf(" char {%s}\n", cub->box.chrs);
	while (mm[1] < 10 && mmax[1] > -5)
	{       
		mm[0] = 0;
		while (mm[0] < 10 && mmax[0] > -5)
		{
			if (mmax[0] < 0 || mmax[1] < 0 || ochrs == -1) // uppercase // mmax pos. rel. -10/+10 map/minimap
				mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0xff66aaff);
			else if (mraz[mmax[0]][mmax[1]] == 64 ) //Boss
				mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0x444444ff);
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
        int mmax[2];
        
        mmax[0] = cub->map.height + 10;
        mmax[1] = cub->map.width + 10;      
        // mlx_draw_player(cub , cub->map.m);
        mlx_set_color_in_rows(cub->renderer.mmap_layer, 0, 10, 0xffffffff);
	pos[0] = cub->hero.cell_x;// * 10;
	pos[1] = cub->hero.cell_y;//* 10;
	mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xff66aaff);
	minimap_surround(cub, pos, mmax);
    
}


