/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:39:58 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/18 22:09:35 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static char	*spc_chk(t_map *map, int j)
{
	char	*line;
	int		i;

	line = map->tab[j];
	i = -1;
	while (++i < map->width)
		if (!ft_strchr(MAP_CHARS, line[i]))
			line[i] = '1';
	return (line);
}

static	int	transcribe(t_map *map)
{
	char	**tmp;
	int		len;
	int		i;

	tmp = map->raw + 6;
	i = 0;
	while (tmp[i])
	{
		len = int_strlen(tmp[i]);
		if (len > map->width)
			map->width = len;
		i++;
	}
	map->height = i;
	map->total_cells = (map->height * map->width);
	return (map->height);
}

static t_map	*map_frame(t_map *map)
{
	char	**m;
	int		i;

	m = map->raw + 6;
	i = 0;
	while (i < map->height)
	{
		map->tab[i] = (char *)ft_calloc(sizeof(char *), (map->width + 1));
		ft_strlcpy(map->tab[i], m[i], map->width + 1);
		map->tab[i] = spc_chk(map, i);
		i++;
	}
	strtab_clear(&map->raw);
	map = wall_check(map);
	printf("wall_chk returned with flg_chk == 1\n");
	if (map->flg_chk == 1)
		return (NULL);
	return (map);
}

static int	read_whole_file(t_map *map, char *filepath)
{
	char	buffer[CUBMAP_BUFMAX + 1];
	int		fd;
	ssize_t	nc;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (error("Could not open file", map));
	nc = read(fd, buffer, CUBMAP_BUFMAX);
	buffer[nc] = '\0';
	if (nc < 0 || nc == CUBMAP_BUFMAX)
	{
		close(fd);
		return (error("Could not read file or buffer maxout", map));
	}
	map->raw = ft_split(buffer, '\n');
	if (!map->raw)
		return (report_malloc_error());
	flush_empty_lines(map->raw);
	close(fd);
	if (strtab_len(map->raw) < 6)
		return (error("Missing info in config file.", map));
	return (0);
}

int	map_checker(t_cub *cub, t_map *map, char *file)
{
	if (ft_strfcmp(".cub", file, 4))
		return (error("Wrong file extention.", map));
	if (read_whole_file(map, file) < 0)
		return (-1);
	cub->tex_id = -1;
	if (tex_parse(cub, map) < 0)
		return (-1);
	if (transcribe(map) < 3)
		return (error("Map in file is too short", map));
	map->tab = (char **)ft_calloc(sizeof(char *), (map->height + 1));
	if (!map->tab || !map_frame(map) || build_grid_coords_map(map) < 0
		|| build_collision_map(map) < 0)
		return (-1);
	print_collision_map(map);
	printf("map (width, height) : (%d, %d)\n", map->width, map->height);
	return (0);
}
