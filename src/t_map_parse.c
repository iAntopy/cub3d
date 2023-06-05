/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:39:58 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/05 16:12:11 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/// hole filler
static char	*spc_chk(t_cub *cub, t_map *m, int j)
{
	char	*line;
	int		len;
	int		i;

	line = m->raw[j];
	len = (int)ft_strlen(m->raw[j]);
	i = -1;
	while (++i < len)
		if (!ft_strchr(cub->box.chrs, line[i]))
			line[i] = 'A';
	return (line);
}

static	int	transcribe(t_map *map, int map_offset)
{
	char	**tmp;
	int		len;
	int		i;

	tmp = map->raw + map_offset;
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

/// map-part of file  >> wall_check
t_cub	*map_frame(t_map *map, t_cub *cub)
{
	char	**m;
	int		i;
	int		q;

	i = 0;
	q = strtab_len(map->raw) - map->height;
	m = (char **)malloc(sizeof(char *) * (map->height + 1));
	m[map->height] = NULL;
	printf("MAP FRAME : map width : %d\n", map->width);
	while (i < map->height)
	{
		m[i] = (char *)ft_calloc(sizeof(char), (map->width + 5));
		spc_chk(cub, map, q);
		ft_strlcpy(m[i], map->raw[q], ft_strlen(map->raw[q]) + 1);
		printf("%s\n", map->raw[q]);
		++q;
		++i;
	}
	cub->map.m = m;
	if (!wall_check(cub, &cub->map))
		return (NULL);
	return (cub);
}

////  map->raw ++ map_len
int	read_whole_file(t_map *map, char *filepath)
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
	return (strtab_len(map->raw));
}

/// parsing autopsy
int	map_checker(t_cub *cub, t_map *map, char *file)
{
	int	map_offset;	

	printf("Map_chker...\n");
	if (ft_strfcmp(".cub", file, 4))
		return (error("Wrong file extention.", map));
	if (read_whole_file(map, file) < 0 || tex_parse(cub, map) < 0)
		return (-1);
	map_offset = cub->box.xnum + cub->box.meta + cub->box.pset - 1;
	map->m = map->raw + map_offset;
	map->height = transcribe(map, map_offset);
	if (!map_frame(map, cub) || !mapx_builder(map, cub))
		return (-1);
	if (build_grid_coords_map(map) < 0 || build_collision_map(map) < 0)
		return (-1);
	print_collision_map(map);
	clr_legend_strct(cub); // xform
	printf("Collisiont check DONE\n");
	if (instanciate_map_objects(cub) < 0)
		return (-1);
	printf("Instanciate DONE\n");
	return (0);
}
