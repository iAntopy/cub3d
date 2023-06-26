/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/26 01:02:19 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Border validated separatly from middle cells to prevent the + pattern checks
// to index out of bound cells.
static int	validate_borders(t_map *m)
{
	int	i;

	i = -1;
	while (++i < m->width)
		if (!(m->tab[0][i] == '\0' || ft_strchr(" 1", m->tab[0][i]))
			|| (!(m->tab[m->height - 1][i] == '\0'
			|| ft_strchr(" 1", m->tab[m->height - 1][i]))))
			return (report_err("Map has an open border"));
	i = -1;
	while (++i < m->height)
		if (!(m->tab[i][0] == '\0' || ft_strchr(" 1", m->tab[i][0]))
			|| (!(m->tab[i][m->width - 1] == '\0'
			|| ft_strchr(" 1", m->tab[i][m->width - 1]))))
			return (report_err("Map has an open border"));
	return (0);
}
/*
static t_map	*t_o_cell(t_map *m, int pos_x, int pos_y)
{
	char	c[4];

	strtab_print(m->tab);
	m->pos_x = pos_x;
	m->pos_y = pos_y;
	m->flg_chk = 0;
	if ((0 < m->pos_x && m->pos_x < (m->width - 1))
		&& (0 < m->pos_y && m->pos_y < (m->height - 1)))
	{
		if (ft_in_set(m->tab[m->pos_y - 1][m->pos_x], (const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
		if (ft_in_set(m->tab[m->pos_y + 1][m->pos_x], (const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
		if (ft_in_set(m->tab[m->pos_y][m->pos_x - 1], (const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
		if (ft_in_set(m->tab[m->pos_y][m->pos_x + 1], (const char *)MAP_CHARS) == -1)
			m->flg_chk = 1;
	}
	else
	{
		printf("SHOULD HAVE POP OUT CLEARLY [%d] frm t_o_cell", m->flg_chk);
		printf("m->x[%d] m->y[%d] \n", m->pos_x, m->pos_y);
		m->flg_chk = 1;
		report_err("Found floor or player on edge of map !\n");
	}
	return (m);
}
*/
static int	t_o_cell(t_map *m, int pos_x, int pos_y)
{
	char	c[4];

	m->pos_x = pos_x;
	m->pos_y = pos_y;
	m->flg_chk = 0;
	if (m->pos_x == 0 || m->pos_x == (m->width - 1)
		|| m->pos_y == 0 || m->pos_y == (m->height - 1))
		return (0);
	c[0] = m->tab[m->pos_y - 1][m->pos_x];
	c[1] = m->tab[m->pos_y + 1][m->pos_x];
	c[2] = m->tab[m->pos_y][m->pos_x - 1];
	c[3] = m->tab[m->pos_y][m->pos_x + 1];
	if (c[0] == ' ' || c[0] == '\0'
		|| c[1] == ' ' || c[1] == '\0'
		|| c[2] == ' ' || c[2] == '\0'
		|| c[3] == ' ' || c[3] == '\0')
		return (report_err("Map is not closed !"), 1);
	return (0);
}

static t_map	*t_hero_cell(t_map *m, int m_x, int m_y)
{
	int	o_cells;

	o_cells = ft_in_set(m->tab[m_y][m_x], (const char *)MAP_CHARS);
	if (o_cells > 5 || m->hero_x > 0 || m->hero_y > 0)
	{
		m->flg_chk = 1;
		report_err("Found multiple hero characters in map.");
	}
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
	if (m->flg_chk)
		return (1);
	if (m->hero_x <= 0 || m->hero_y <= 0)
	{
		report_err("No player charater found in map.");
		return (1);
	}
	return (0);
}

static int	map_contains_valid_chars(t_map *m)
{
	int	i;
	int	j;

	i = -1;
	while (++i < m->height)
	{
		j = -1;
		while (++j < m->width)
			if (!(m->tab[i][j] == '\0' || m->tab[i][j] == ' '
				|| ft_strchr((const char *)MAP_CHARS, m->tab[i][j])))
				return (report_err("Map contains at least one invalid character."));
	}
	return (0);
}

t_map	*wall_check(t_map *m)
{
	int	o_cells;

	if (validate_borders(m) < 0 || map_contains_valid_chars(m) < 0)
	{
		m->flg_chk = 1;
		return (m);
	}
	o_cells = -1;
	m->pos_y = 0;
	while (m->pos_y < m->height && m->flg_chk == 0)
	{
		m->pos_x = 0;
		while (m->pos_x < m->width && m->flg_chk == 0)
		{
			o_cells = ft_in_set((m->tab[m->pos_y][m->pos_x]), (const char *)MAP_CHARS);
			if (o_cells == 0 || o_cells > 1)
			{
				//m = t_o_cell(m, m->pos_x, m->pos_y);
				m->flg_chk = t_o_cell(m, m->pos_x, m->pos_y);
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
