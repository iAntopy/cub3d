/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 18:58:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/19 01:44:52 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"


void	minimap_check(t_cub *cub, t_map *m)
{
//	int 	stat[2];
	int		pos[2];
	int		o_cells;
	
	o_cells = -1;
//	stat[0] = 0;
//	stat[1] = 0;
	
	m->pos_y = 0;
	
	while (m->pos_y < m->height &&  m->pos_y * 10 < 150) 
	{
		m->pos_x = 0;
		while (m->pos_x < m->width &&  m->pos_x * 10 < 150) 
		{
			o_cells = ft_in_set((m->m[m->pos_y][m->pos_x]),
					(const char *)cub->box.chrs);
			
			if (o_cells < 0 && m->m[m->pos_y][m->pos_x] != '\0') // wall
			{
				pos[0] = m->pos_x *10;
				pos[1] = m->pos_y *10;
				mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0x88000000);
				//black semi alpha
				// printf("\tMINIMAP_POS_CHK (X[%d]::Y[%d]>> {%C}) \n", pos[0], pos[1], o_cells);
			}
			else if (o_cells == (int_strlen(cub->box.chrs) - 1)) // plyr
			{
				pos[0] = m->pos_x *10;
				pos[1] = m->pos_y *10;		
				mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xffaa8888);	
				// printf("\tMINIMAP_CHK STAT_X[%d]::STAT_Y[%d]: \n", stat[0], stat[1]);
				printf("\tMINIMAP_POS_CHK (X[%d]::Y[%d]>> {%C}) \n", pos[0], pos[1], cub->box.chrs[o_cells]);
				// mlx_draw_square(cub->renderer.mmap_layer, pos, 15, 0x88ff66ff);
			}
			else if ((o_cells >= cub->box.meta - 1) && (o_cells < cub->box.meta + cub->box.pset )) // floor
			{
				pos[0] = m->pos_x * 10;
				pos[1] = m->pos_y * 10;
				mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xffffffff);
			}
			else if (o_cells < cub->box.meta ) 
			{
				pos[0] = m->pos_x * 10;
				pos[1] = m->pos_y * 10;
				mlx_draw_square(cub->renderer.mmap_layer, pos, 10, 0xffff66ff);
				printf("\tMINIMAP_POS_CHK (X[%d]::Y[%d]>> {%C}) \n", pos[0], pos[1], cub->box.chrs[o_cells]);
			}
			m->pos_x++;
		}
		m->pos_y++;
//		stat[0] = m->pos_x;
//		stat[1] = m->pos_y;
	}
		// printf("\tMINIMAP_CHK STAT_X[%d]::STAT_Y[%d]: \n", stat[0], stat[1]);
}

// /// set player pos to center map 
// void mlx_draw_player(t_cub * cub, t_map *map)
// 	{
// 	    int pos[2];
// 		pos[0] = cub->hero.cell_x * 15;
// 		pos[1] = cub->hero.cell_y * 15;
// 		if (pos[0] < map->width && pos[1] < map->height)
// 			mlx_draw_square(cub->renderer.mmap_layer, pos, 15, 0x88ff66ff);
// }

/// set map_char to map_pixel
// void	mlx_draw_mmap(t_cub *cub)
// {
// 	int	len[2];
	// 	int mmax[2];
	// 	int id;
		
	// 	id = 0;
	// 	mmax[0] = 10;//cub->map.height;
	// 	mmax[1] = 10;//cub->map.width;      
		
	// 	len[0] = 0;//cub->hero.cell_x * 10;
	// 	len[1] = 0;//cub->hero.cell_y * 10;

	// 	while (len[0] < mmax[0])
	// 	{
	// 		len[1] = 0;
	// 		while (len[1] < mmax[1])
	// 		{
	// 			len[0] *= 15;
	// 			len[1] *= 15;
	// 			if (id % 2)
	// 				mlx_draw_square(cub->renderer.mmap_layer, len, 15, 0x99ffaaff);	
	// 			else
	// 				mlx_draw_square(cub->renderer.mmap_layer, len, 15, 0xffffffff);

	// 			len[0] /= 15;
	// 			len[1] /= 15;
	// 			len[1]++;
	// 			id++;
	// 		}
	// 		len[0]++;
	// 		id++;
	// 	}
	// 	printf("MiniMAP END (x[%d],y[%d] \n\n",len[0], len[1]);
	// 	// mlx_draw_player(cub, &cub->map);
// }

/// set map_char to map_pixel
// void	mlx_update_mmap(t_cub *cub, t_map *m)
		// {
		// 	int	len[2];
		// 	int mmax[2];
		// 	int id;
			
		// 	id = 0;
		// 	mmax[0] = 10;//cub->map.height;
		// 	mmax[1] = 10;//cub->map.width;      
			
		// 	len[0] = 0;//cub->hero.cell_x * 10;
		// 	len[1] = 0;//cub->hero.cell_y * 10;

		// 	while (len[0] < mmax[0])
		// 	{
		// 		len[1] = 0;
		// 		while (len[1] < mmax[1])
		// 		{
		// 			len[0] *= 15;
		// 			len[1] *= 15;
		// 			if (id % 2)
		// 				mlx_draw_square(cub->renderer.mmap_layer, len, 15, 0x99ffaaff);	
		// 			else
		// 				mlx_draw_square(cub->renderer.mmap_layer, len, 15, 0xffffffff);

		// 			len[0] /= 15;
		// 			len[1] /= 15;
		// 			len[1]++;
		// 			id++;
		// 		}
		// 		len[0]++;
		// 		id++;
		// 	}
		// 	printf("MiniMAP END (x[%d],y[%d] \n\n",len[0], len[1]);
		// 	mlx_draw_player(cub, &cub->map);
// }
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
    //     int	pos[2];
      
	// pos[0] = 30;//cub->hero.cell_x * 15;
	// pos[1] = 30;//cub->hero.cell_y * 15;

	// mlx_draw_mmap(cub);
	// mlx_update_mmap(t_cub *cub, t_map *m);
	// mlx_draw_square(cub->renderer.mmap_layer, pos, 15, 0x88000000);//black semi alpha
	// minimap_surround(cub, pos, mmax);
    minimap_check(cub, &cub->map);
	// mlx_draw_player(cub, &cub->map);
}


