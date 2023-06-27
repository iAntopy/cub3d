/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/26 20:19:34 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	error_color(t_map *map, char ***split_color)
{
	strtab_clear(split_color);
	return (error("Failed to parse a color argument from file", map));
}

static int	colors_are_all_digits(char **split_col)
{
	int			i;
	const char	*s;

	i = -1;
	while (split_col[++i])
	{
		s = split_col[i] - 1;
		while (*(++s))
			if (!ft_isdigit(*s))
				return (0);
	}
	return (1);
}

static int	color_split(t_map *map, char *col_str, int *ret_col)
{
	char		**color;
	int			rgb[3];

	*ret_col = 0;
	color = ft_split_set(col_str + 1, ", ");
	if (!color || strtab_len(color) != 3 || !colors_are_all_digits(color))
		return (error_color(map, &color));
	rgb[0] = ft_atoi(color[0]);
	rgb[1] = ft_atoi(color[1]);
	rgb[2] = ft_atoi(color[2]);
	if (strtab_len(color) != 3 || (rgb[0] < 0 || rgb[0] > 255)
		|| (rgb[1] < 0 || rgb[1] > 255) || (rgb[2] < 0 || rgb[2] > 255))
		return (error_color(map, &color));
	*ret_col = str_to_color(rgb[0], rgb[1], rgb[2], 0xff);
	strtab_clear(&color);
	return (0);
}

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
//	{
//		ft_eprintf("Error\n\t- Trying to load texture id %d twice.\n", id);
//		return (NULL);
//	}	
	return (cub);
}

static int	error_clr(char *err, t_map *map)
{
	strtab_clear(&map->raw);
	strtab_clear(&map->txtr);
	return (error(err, map));
}

int	tex_parse(t_cub *cub, t_map *map)
{
	int			nb;
	int			id;
	char		header_flgs[7];

	ft_memclear(header_flgs, sizeof(char) * 7);
	nb = 0;
	while (map->raw[nb] && ft_strlen(header_flgs) < 6)
	{
		if (map->raw[nb][0] == '\0')
		{
			nb++;
			continue ;
		}
		id = ft_in_set(map->raw[nb][0], (const char *)"WNESCF");
		if (id < 0 || map->raw[nb][1] != ' ' || header_flgs[id])
			return (error_clr("Invalid config label found!\n", map));
		else if (id < 4 && !get_tex_by_id(cub, id, map->raw[nb]))
			return (error_clr(NULL, map));
		else if (id == 4 || id == 5)
			if (color_split(map, map->raw[nb], cub->tex.color + (id - 4)) < 0)
				return (-1);
		header_flgs[id] = 1;
		nb++;
	}
	return (t_parse_check(map, nb, header_flgs));
}
