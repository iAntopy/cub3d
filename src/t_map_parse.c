/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:39:58 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/26 21:55:45 by iamongeo         ###   ########.fr       */
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
static t_cub	*map_frame(t_map *map, t_cub *cub)
{
	char	**m;
	int		i;
	int		q;

	i = 0;
	q = strtab_len(map->raw) - map->height;
	m = (char **)malloc(sizeof(char *) * map->height + 1);
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
	cub = wall_check(cub, &cub->map);
	return (cub);
}

//// map_len
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
	else
		return (strtab_len(map->raw));
	return (0);
}

int	map_checker(t_cub *cub, t_map *map, char *file)
{
	int	map_len;
	int	map_offset;

	if (ft_strfcmp(".cub", file, 4))
		return (error("Wrong file extention.", map));
	map_len = read_whole_file(map, file);
	if (tex_parse(cub, map) < 0)
		return (-1);
	map_offset = (cub->box.xnum - cub->box.pnum) + cub->box.pset;
	map->m = map->raw + map_offset;
	map->height = transcribe(map, map_offset);
	printf("\n$$$ MAP_RAW (%d)  TXTR [%d] ", map_len, map_offset);
	printf(" MAP_HEIGHT [%d] $$$\n\n", map->height);
	if (!map_frame(map, cub) || !mx_struct(map, cub))
		return (-1);
	printf("A ptr : %p =? %p\n", map->mx[0][0], &cub->pset[2]);
	if (build_grid_coords_map(map) < 0 || build_collision_map(map) < 0)
		return (-1);
	print_collision_map(map);
	return (0);
}

/* WAS LIKE ... Ver.2
	map_checker:
		1	strfcmp .cub - chk file extention 
		2 	read_whole_file - stock it all. = map->raw
		3	tex_parse - chk map char by ref	= ( [6] == ref. xwalls fork.
				ft_in_set (charset) - cmp char <> ref
				get_tex_by_id (<4) -  attrib path from ref
						spaceless , txtr[id] = load_png (path)
				color_split (id 4/5) - bitshift_color process
		4	transcribe
		5	map_frame - gabarit!
		6	wall_chk - floor / edge 
				ft_in_set - floor / sibling	: t_o_cell
				hero_cell - pos / dir		: t_hero_cell	
*/

/*
	MOST OF IT	:	- read_whole_file 		
				+ find holy ref.
		
				+ alt behavior for 'A' = xwalls[4]  *** (get_tex_by_ref) >>t_ref_utils
				+ ref for xwalls[4] to be build from preset
					- tex_parse 
				+ chk order [0.1.2.3] => [W,N,E,S]
				+ find ref by pos
				+ fill load_png with found attrib
	NEXT 	:
			:	Enable	MULTI XWALLS.  		-- OK -- choose (4 on 5) map3
			:			MAP_RAW	xwalls[4]			dual choice (A - B)
			:				ex: "A acbd"
			:	Setting MAP_CHARTS
			:			MAP_ floor_tex 	REF:('0'-'9')
			:			MAP_ xwalls		REF:('A'-'Z')
			:			MAP_ player		REF:('@') no_direction(N) default!
			:			MAP_ object		REF:('meta-chars')
			:
*/
