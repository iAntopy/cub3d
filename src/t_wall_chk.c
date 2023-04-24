/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/18 20:53:45 by iamongeo         ###   ########.fr       */
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
// 			m->flg_chk = 1;
// 		if (ft_in_set(m->tab[m->pos_y + 1][m->pos_x],
// 			(const char *)MAP_CHARS) == -1)
// 			m->flg_chk = 1;
// 		if (ft_in_set(m->tab[m->pos_y][m->pos_x - 1],
// 			(const char *)MAP_CHARS) == -1)
// 			m->flg_chk = 1;
// 		if (ft_in_set(m->tab[m->pos_y][m->pos_x + 1],
// 			(const char *)MAP_CHARS) == -1)
// 			m->flg_chk = 1;
// 	}
// 	else
// 	{
// 		report_err("Found floor or player on edge of map !\n");
// 		m->flg_chk = 1;
// 	}
// 	return (m);
// }

static t_map	*t_hero_cell(t_map *m, int m_x, int m_y)
{
	int	o_cells;

	o_cells = ft_in_set(m->tab[m_y][m_x], (const char *)MAP_CHARS);
	if (o_cells > 5 || m->hero_x > 0 || m->hero_y > 0)
		m->flg_chk = 1;
	else
	{
		m->hero_side = o_cells - 2;
		m->hero_x = m->pos_x;
		m->hero_y = m->pos_y;
	}
	return (m);
}

static int	check_hero_found(t_map *m)
{
	if (m->hero_x <= 0 || m->hero_y <= 0)
	{
		report_err("No player charater found in map.");
		return (1);
	}
	return (0);	
}

t_map	*wall_check(t_map *m)
{
	int	o_cells;

	o_cells = -1;
	m->pos_y = 0;
	while (m->pos_y < m->height && m->flg_chk == 0)
	{
		m->pos_x = 0;
		while (m->pos_x < m->width && m->flg_chk == 0)
		{
			o_cells = ft_in_set((m->tab[m->pos_y][m->pos_x]), \
					(const char *)MAP_CHARS);
			if (o_cells == 0 || o_cells > 1)
			{
				// m = t_o_cell(m, m->pos_x, m->pos_y);
				if (m->flg_chk == 0 && o_cells > 1)
					m = t_hero_cell(m, m->pos_x, m->pos_y);
			}
			m->pos_x++;
		}
		m->pos_y++;
	}
	m->flg_chk = check_hero_found(m);
	return (m);
}
