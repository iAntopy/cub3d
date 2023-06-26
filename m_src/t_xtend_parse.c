/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_xtend_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 01:22:01 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/26 02:42:14 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//files that haven't match requirment limits
t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex_str)
{
	char	*t;

	if (id < 0 || id > 3)
		return (NULL);
	if (!cub->tex.walls[id])
	{
		while (*(++tex_str) && ft_isspace(*tex_str))
			continue ;
		t = tex_str;
		while (*tex_str && !ft_isspace(*tex_str))
			tex_str++ ;
		*tex_str = '\0';
		cub->tex.walls[id] = mlx_load_png(t);
		if (!cub->tex.walls[id])
			return (report_mlx_tex_load_failed(t));
		cub->tex_id++;
	}
	else
	{
		ft_eprintf("Error\n\t- Trying to load texture id %d twice.\n", id);
		return (NULL);
	}	
	return (cub);
}

t_map	*t_hero_cell(t_map *m, int m_x, int m_y)
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

int	t_parse_check(t_map *map, int nb, char *header_flgs)
{
	if (ft_strlen(header_flgs) < 6)
		return (error_clr("At least one config flag missing", map));
	strtab_print(map->raw);
	printf("lines to end of header %d\n", nb);
	flush_map_header_empty_lines(map, nb);
	strtab_print(map->raw);
	return (0);
}

int	is_empty_line(char *line)
{
	int	j;

	j = 0;
	while (line[j] && ft_isspace(line[j]))
		j++;
	return (!line[j]);
}

// nb_lines is the nb of lines from beginning of file to last header flag
// including empty lines.
void	flush_map_header_empty_lines(t_map *map, int nb_lines)
{
	int		i;
	int		nb_cut;
	size_t	len;

	printf("map raw [nb_lines] : %s\n", map->raw[nb_lines]);
	while (map->raw[nb_lines] && is_empty_line(map->raw[nb_lines]))
		nb_lines++;
	nb_cut = 0;
	i = -1;
	while (map->raw[++i] && (i + nb_cut) < nb_lines)
	{
		if (!is_empty_line(map->raw[i]))
			continue ;
		len = strtab_len(map->raw + i + 1);
		free(map->raw[i]);
		ft_memmove(map->raw + i, map->raw + i + 1,
			sizeof(char *) * (len + 1));
		nb_cut++;
		i--;
	}
}
