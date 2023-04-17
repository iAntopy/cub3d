/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:39:58 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/16 23:33:17 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_map	*init_map(t_map *map)
{
	map->pos_x = 1;
	map->pos_y = 1;
	map->hero_side = -1;
	map->hero_x = 0;
	map->hero_y = 0;
	map->width = 0;
	map->height = 0;
	map->total_cells = 0;
	map->file = NULL;
	map->tab = NULL;
	map->flg_chk = 0;
	map->lines_to_map = 0;
	return (map);
}

static char	*spc_chk(t_map *map, int j)
{
	char	*line;
	int		i;

	line = map->tab[j];
	i = -1;
	while (++i < map->width)
		if (!ft_strchr(MAP_CHARS, line[i]))
			line[i] = ' ';
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
	printf("DEBUG: map->h = %d:\n", map->height);
	printf("DEBUG: map->w = %d:\n", map->width);
	printf("DEBUG: map->total_cells = %d:\n", map->total_cells);
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
	if (map->flg_chk == 1)
	{
		printf("DEBUG WARNING : wall chk failed \n");
		return (NULL);
	}
	return (map);
}

int	read_whole_file(t_map *map, char *filepath)
{
	char	buffer[100001];
	int		fd;
	ssize_t	nc;
	char	**t;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (error("Could not open file", map));
	nc = read(fd, buffer, 100000);
	buffer[nc] = '\0';
	printf("ca va : nc = %zd\n", nc);
	if (nc < 0)
	{
		close(fd);
		return (error("Could not read file or buffer maxout", map));
	}
	map->raw = ft_split(buffer, '\n');
	strtab_print(map->raw);
	t = map->raw;
	while (*t)
	{
		if (is_empty_line(*t))
		{
			ft_free_p((void **)t);
			ft_memmove(t, t + 1, sizeof(char *) * (1 + strtab_len(t + 1)));
		}
		t++;
	}
	strtab_print(map->raw);
	close(fd);
	return (0);
}

int	map_checker(t_cub *cub, t_map *map, char *file)
{
	if (ft_strfcmp(".cub", file, 4))
		return (error("Wrong file extention.", map));
	if (read_whole_file(map, file) < 0)
		return (-1);
	cub->tex_id = -1;
	printf("tex_parse\n");
	if (tex_parse(cub, map) < 0)
		return (-1);
	printf("transcribe\n");
	if (transcribe(map) < 3)
		return (error("Map in file is too short", map));
	map->tab = (char **)ft_calloc(sizeof(char *), (map->height + 1));
	if (!map->tab || !map_frame(map) || build_grid_coords_map(map) < 0
		|| build_collision_map(map) < 0)
		return (-1);
	print_collision_map(map);
	return (0);
}
