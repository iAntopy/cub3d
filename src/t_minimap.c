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


// /// set player pos to center map 
void mlx_draw_player(t_cub * cub, t_map *map)
{
    int pos[2];
	pos[0] = cub->hero.cell_x * 10;
	pos[1] = cub->hero.cell_y * 10;
	if (pos[0] < map->width && pos[1] < map->height)
		mlx_draw_square(cub->renderer.mmap_layer, pos, 15, 0x88ff6666);
}

/// set map_char to map_pixel
void	mlx_draw_mmap(t_cub *cub)
{
	int	len[2];
	int mmax[2];
	int id;
	
	id = 0;
	mmax[0] = 10;//cub->map.height;
	mmax[1] = 10;//cub->map.width;      
	
	len[0] = 0;//cub->hero.cell_x * 10;
	len[1] = 0;//cub->hero.cell_y * 10;

	while (len[0] < mmax[0])
	{
		len[1] = 0;
		while (len[1] < mmax[1])
		{
			len[0] *= 15;
			len[1] *= 15;
			if (id % 2)
				mlx_draw_square(cub->renderer.mmap_layer, len, 15, 0x99ffaaff);			else
				mlx_draw_square(cub->renderer.mmap_layer, len, 15, 0xffffffff);

			len[0] /= 15;
			len[1] /= 15;
			len[1]++;
			id++;
		}
		len[0]++;
		id++;
	}
	printf("MiniMAP END (x[%d],y[%d] \n\n",len[0], len[1]);
	// mlx_draw_player(cub, &cub->map);
}

// void    minimap_surround(t_cub *cub, int pos[2], int mmax[2])
		// {
		// 	// int	pos[2];
		// 	// int mmax[2];
		// 	int ochrs;

		// 	int mm[2];
		// 	char **mraz;
		// 	// t_map *m;

		// 	mraz = cub->map.m;
		// 	// m = &cub->map;
		// 	// mmax[0] =  pos[0] - 5;	
		// 	// mmax[1] =  pos[1] - 5;	
		// 	pos[0] = cub->hero.cell_x - 5;
		// 	pos[1] = cub->hero.cell_y - 5; 

			
		// 		// printf("<<<< minim(x[%d], y[%d] \n",mmax[0], mmax[1]);//, mraz[mmax[0]][mmax[1]]);
		// 		// printf("<<<  char( x[%d], y[%d],  \n", pos[0],pos[1]);
		// 		// printf("<<<  char {%d} //", ochrs);
		// 		// printf("<<<  char {%c}\n", cub->box.chrs[ochrs]);
		// 		printf(" MM ++char {%s}\n", cub->box.chrs);
		// 	while (mm[1] < 10 && mmax[1] > -5)
		// 	{       
		// 		mm[0] = 0;
		// 		pos[0] = cub->hero.cell_x - 5;
		// 		printf("\n");
		// 		while (mm[0] < 10 && mmax[0] > -5 && mmax[0] < cub->map.width + 5)
		// 		{
		// 			ochrs = ft_in_set((mraz[pos[1]][pos[0]]),(const char *)cub->box.chrs);
		// 			// printf("<<<%d>>> ((%d, %d))\n", mraz[mmax[0]][mmax[1]], mmax[0], mmax[1]);
		// 			// if (mmax[0] < 0 || mmax[1] < 0 || ochrs == -1) // uppercase // mmax pos. rel. -10/+10 map/minimap
		// 			// {
		// 			// 	// mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0x444444ff);
		// 			// 	// printf("<<<%d>>> ((%d, %d)) Out_Of_Bound\n", mraz[mmax[0]][mmax[1]], mmax[0], mmax[1]);
		// 			// 	printf("MM ((%d, %d)) Out_Of_Bound\n", mm[0], mm[1]);
		// 			// 	printf("POS ((%d, %d)) Out_Of_Bound\n", pos[0], pos[1]);
		// 			// }
		// 			// else if (mraz[mmax[0]][mmax[1]] == 64 ) //Boss
		// 			// {
		// 			// 	// mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0xff11aaff);
		// 			// 	printf("<<<%d>>>PLYR ((%d, %d))\n", mraz[mmax[0]][mmax[1]], mmax[0], mmax[1]);
		// 			// }
		// 			// else if (mraz[mmax[0]][mmax[1]] >= 65 ) //uppercase
		// 			// {

		// 			// 	// mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0x99ffaaff);
		// 			// 	printf("<<<%d>>> UPPERC ((%d, %d))\n", mraz[mmax[0]][mmax[1]], mmax[0], mmax[1]);
		// 			// }
		// 			// else if (mraz[mmax[0]][mmax[1]] >= 48 ) //num_chrs
		// 			// {

		// 			// 	// mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0xffffffff);
		// 			// 	printf("<<<%d>>> NCHR((%d, %d))\n", mraz[mmax[0]][mmax[1]], mmax[0], mmax[1]);
		// 			// }
		// 			// else //if (mraz[mmax[0]][mmax[1]] < 48 ) //uppercase
		// 			printf("%c", cub->box.chrs[ochrs]);
		// 			mmax[0]++;
		// 			mm[0]++;
		// 			pos[0]++;
		// 		}
		// 		mmax[1]++;
		// 		mm[1]++;
		// 		pos[1]++;
		// 	}
		// 				// mlx_draw_square(cub->renderer.mmap_layer, mmax, 10, 0xff66aaff);

		// 	// mmax_x = cub->map.height + 5;
		// 	// mmax_y = cub->map.width - 5;      
		// 	// mlx_draw_player(cub , cub->map.m);
		//         // mlx_set_color_in_rows(cub->renderer.mmap_layer, 0, 10, 0xffffffff);
		// 	// pos[0] = cub->hero.cell_x * 10;
		// 	// pos[1] = cub->hero.cell_y * 10;
		// 	// mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xff66aaff);
				
// }


////    set map_min_max 
void    minimap_set_pos(t_cub *cub)
{
        int	pos[2];
        // int mmax[2];
        
        // mmax[0] = cub->map.height + 10;
        // mmax[1] = cub->map.width + 10;      
        // mlx_draw_player(cub , cub->map.m);
        // mlx_set_color_in_rows(cub->renderer.mmap_layer, 0, 10, 0xffffffff);
	pos[0] = 30;//cub->hero.cell_x * 15;
	pos[1] = 30;//cub->hero.cell_y * 15;

	mlx_draw_mmap(cub);
	mlx_draw_square(cub->renderer.mmap_layer, pos, 15, 0x88000000);//black semi alpha
	// mlx_draw_player(cub, m);
	// minimap_surround(cub, pos, mmax);
    
}


