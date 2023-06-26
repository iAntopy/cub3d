/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/26 01:03:12 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


static int	error_clr(char *err, t_map *map)
{
	if (map->raw)
		strtab_clear(&map->raw);
	if (map->txtr)
		strtab_clear(&map->txtr);
	// printf(" Error:Code[%s]\n", err);
	if (err && err[0])
		return (error(err, map));
	return (-1);
}

static int	error_color(t_map *map, char ***split_color)
{
		// flush_split_color(split_color);	
		strtab_clear(split_color);
		return (error_clr("Failed to parse: 'error_color'.", map));
}


static int	colors_are_all_digits(char **split_col)
{
	int		i;
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

	// *ret_col = 0;
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

static int	error_clr(char *err, t_map *map)
{
	strtab_clear(&map->raw);
	strtab_clear(&map->txtr);
	return (error(err, map));
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
	int	i;
//	int	j;
	int	nb_cut;
	size_t	len;

	printf("map raw [nb_lines] : %s\n", map->raw[nb_lines]);
	while (map->raw[nb_lines] && is_empty_line(map->raw[nb_lines]))
		nb_lines++;
	nb_cut = 0;
	i = -1;
	while (map->raw[++i] && (i + nb_cut) < nb_lines)
	{
//		j = 0;
//		while (map->raw[i][j] && is_empty_line(map->raw[i]))//ft_isspace(map->raw[i][j]))
//			j++;
		if (!is_empty_line(map->raw[i]))//map->raw[i][j])
			continue ;
		len = strtab_len(map->raw + i + 1);
		free(map->raw[i]);
		ft_memmove(map->raw + i, map->raw + i + 1,
			sizeof(char *) * (len + 1));
		nb_cut++;
		i--;
	}
}

int	tex_parse(t_cub *cub, t_map *map)
{
	int		nb;
	int		id;
	char		header_flgs[7];

	ft_memclear(header_flgs, sizeof(char) * 7);
	nb = 0;
	//while (nb < 6 && map->raw[nb])
	while (map->raw[nb] && ft_strlen(header_flgs) < 6)
	{
		if (map->raw[nb][0] == '\0')
		{
			nb++;
			continue ;
		}
		id = ft_in_set(map->raw[nb][0], (const char *)"WNESCF");
		printf("id for tag %c : %d\n", map->raw[nb][0], id);
		if (id < 0 || map->raw[nb][1] != ' ' || header_flgs[id])
			return (error_clr("Invalid config label found!\n", map));
		else if (id < 4 && !get_tex_by_id(cub, id, map->raw[nb]))
			return (error_clr(map->raw[nb], map));
		else if (id == 4 || id == 5)
			if (color_split(map, map->raw[nb], cub->tex.color + (id - 4)) < 0)
				return (-1);
		header_flgs[id] = 1;
		nb++;
	}
//	if (cub->tex_id != 3)
//		return (error_clr("Missing textures.
//			At least one wall texture was not loaded\n", map));
	//if (!all_header_flags_present(header_flgs))
	if (ft_strlen(header_flgs) < 6)
		return (error_clr("At least one config flag missing", map));
	strtab_print(map->raw);
	printf("lines to end of header %d\n", nb);
	flush_map_header_empty_lines(map, nb);
	strtab_print(map->raw);
	return (0);
}
