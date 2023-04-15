/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/15 02:35:37 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	color_split(char **txtr, int id)
{
	char		**color;
	uint32_t	col;

	if (!(id == 4 || id == 5))
		return (0);
	col = 0;
	color = ft_split(txtr[1], ',');
	col = str_to_color(ft_atoi(color[0]), ft_atoi(color[1]), ft_atoi(color[2]), 0xff);
	strtab_clear(&color);
	return (col);
}

static t_map	*t_o_cell(t_map *m, int pos_x, int pos_y)
{
	m->pos_x = pos_x;
	m->pos_y = pos_y;
	m->flg_chk = 0;
//	if (m->pos_x != 0 && m->pos_y != 0 && m->pos_x < m->width && m->pos_y < m->height)
	if ((0 < m->pos_x && m->pos_x < (m->width - 1))
		&& (0 < m->pos_y && m->pos_y < (m->height - 1)))
	{
		if (ft_in_set(m->tab[m->pos_y - 1][m->pos_x],
			(const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
		if (ft_in_set(m->tab[m->pos_y + 1][m->pos_x],
			(const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
		if (ft_in_set(m->tab[m->pos_y][m->pos_x - 1],
			(const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
		if (ft_in_set(m->tab[m->pos_y][m->pos_x + 1],
			(const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
/*
		if (m->pos_x - 1)
			if (ft_in_set(&m->tab[m->pos_x - 1][m->pos_y],
				(const char *)MAP_CHARS) == -1)
				m->flg_chk = 1;
		if (m->pos_x + 1)
			if (ft_in_set(&m->tab[m->pos_x + 1][m->pos_y],
				(const char *)MAP_CHARS) == -1)
				m->flg_chk = 1;
		if (m->pos_y - 1)
			if (ft_in_set(&m->tab[m->pos_x][m->pos_y - 1],
				(const char *)MAP_CHARS) == -1)
				m->flg_chk = 1;
		if (m->pos_y + 1)
			if (ft_in_set(&m->tab[m->pos_x][m->pos_y + 1],
				(const char *)MAP_CHARS) == -1)
				m->flg_chk = 1;
*/
	}
	else
	{
		printf("DEBUG WARNING : Found 0 on edge of map !\n");
		m->flg_chk = 1;
	}
	return (m);
}

static t_map	*t_hero_cell(t_map *m, int m_x, int m_y)
{
	int	o_cells;

	o_cells = ft_in_set(m->tab[m_y][m_x], (const char *)MAP_CHARS);
	if (o_cells > 5)
		m->flg_chk = 1;
	else
	{
		m->hero_side = o_cells;
		m->hero_x = m->pos_x;
		m->hero_y = m->pos_y;
//		m->flg_chk = 2;
	}
	return (m);
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
//			printf("wall chk : (%d, %d)\n", m->pos_x, m->pos_y);
			o_cells = ft_in_set((m->tab[m->pos_y][m->pos_x]), \
					(const char *)MAP_CHARS);
			if (o_cells == 0)
				m = t_o_cell(m, m->pos_x, m->pos_y);
			if (o_cells > 1)
				m = t_hero_cell(m, m->pos_x, m->pos_y);
			m->pos_x++;
		}
		m->pos_y++;
	}
	if (m->hero_x <= 0 || m->hero_y <= 0)
		m->flg_chk = 1;
//	if (m->hero_x != -1)
//	{
//		m->pos_x = m->hero_x;
//		m->pos_y = m->hero_y;
//	}
	return (m);
}
