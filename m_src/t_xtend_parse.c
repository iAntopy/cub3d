/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_xtend_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 01:22:01 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/26 21:04:49 by iamongeo         ###   ########.fr       */
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
		while (*t && !ft_isspace(*t))
			t++ ;
		*t = '\0';
		if (!ft_strrchr(tex_str, '.')
			|| ft_strncmp(ft_strrchr(tex_str, '.'), ".png", 4) != 0)
			return (report_err("Filepath with missing/wrong extension"), NULL);
		cub->tex.walls[id] = mlx_load_png(tex_str);
		if (!cub->tex.walls[id])
			return (report_mlx_tex_load_failed(tex_str));
		cub->tex_id++;
	}
	else
		return (report_err("Trying to load a texture twice."), NULL);
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
	flush_map_header_empty_lines(map, nb);
	return (0);
}

// nb_lines is the nb of lines from beginning of file to last header flag
// including empty lines.
void	flush_map_header_empty_lines(t_map *map, int nb_lines)
{
	int		i;
	int		nb_cut;
	size_t	len;

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
