/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/26 21:00:14 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

	// static t_map	*t_o_cell(t_map *m, int pos_x, int pos_y)
	// {
	// 	m->pos_x = pos_x;
	// 	m->pos_y = pos_y;
	// 	m->flg_chk = 0;
	// 	if ((0 < m->pos_x && m->pos_x < (m->width - 1))
	// 		&& (0 < m->pos_y && m->pos_y < (m->height - 1)))
	// 	{
	// 		if (ft_in_set(m->tab[m->pos_y - 1][m->pos_x],
	// 			(const char *)MAP_CHARS) == -1)
	// 			m->flg_chk = 0;
	// 		if (ft_in_set(m->tab[m->pos_y + 1][m->pos_x],
	// 			(const char *)MAP_CHARS) == -1)
	// 			m->flg_chk = 0;
	// 		if (ft_in_set(m->tab[m->pos_y][m->pos_x - 1],
	// 			(const char *)MAP_CHARS) == -1)
	// 			m->flg_chk = 0;
	// 		if (ft_in_set(m->tab[m->pos_y][m->pos_x + 1],
	// 			(const char *)MAP_CHARS) == -1)
	// 			m->flg_chk = 0;
	// 	}
	// 	else
	// 	{
	// 		report_err("Found floor or player on edge of map !\n");
	// 		m->flg_chk = 1;
	// 	}
	// 	return (m);
	// }

	// static t_map	*t_hero_cell(t_map *m, int m_x, int m_y)
		// {
		// 	int	o_cells;

		// 	o_cells = ft_in_set(m->tab[m_y][m_x], (const char *)MAP_CHARS);
		// 	if (o_cells > 5 || m->hero_x > 0 || m->hero_y > 0)
		// 		m->flg_chk = 1;
		// 	else
		// 	{
		// 		m->hero_x = m->pos_x;
		// 		m->hero_y = m->pos_y;
		// 	}
		// 	return (m);
	// }

static int	check_hero_found(t_map *m)
{
	if (m->hero_x <= 0 || m->hero_y <= 0)
	{
		report_err("No player charater found in map.");
		return (1);
	}
	return (0);	
}

t_cub *wall_check(t_cub *cub, t_map *m)
{
	int	o_cells;

	printf("** ---- wall_check starts ---- **\n");
	o_cells = -1;
	m->pos_y = 0;
	printf("HINGH! map : %p\n", m);
	printf("map height : %d\n", m->height);
	printf("map width : %d\n", m->width);
	printf("map pos_y : %d\n", m->pos_y);
	while (m->pos_y < m->height)
	{
		m->pos_x = 0;
		while (m->pos_x < m->width)
		{
			o_cells = ft_in_set((m->m[m->pos_y][m->pos_x]), (const char *)cub->box.chrs);
	//		printf("o_cell : %d, (x, y) = (%d, %d)\n", o_cells, m->pos_x, m->pos_y);
			if (o_cells < 0 && m->m[m->pos_y][m->pos_x] != '\0')
				m->m[m->pos_y][m->pos_x] = 'A';
			
			if ( o_cells == 4)
			{
				m->flg_chk = 2;
				m->hero_x = m->pos_x;
				m->hero_y = m->pos_y;
				m->m[m->pos_y][m->pos_x] = '0';
				m->flg_chk = check_hero_found(m);
			}
			m->pos_x++;
		}
		printf("wow : %s\n", m->m[m->pos_y]);
		m->pos_y++;
	}
//	cub->map = *m;
	printf("wall_chk exit\n");
	return (cub);
}

/*
	map->check
		:	map_frame	>>	calloc each line	>> wall_check >> chk each cell
		:	wall_chk	>>	o_cell /hero_cell /hero_found
		++	ptr. ass.	>>	set each tile with a ptr
		
	
*/