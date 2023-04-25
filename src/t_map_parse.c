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
	printf("*** line= %c \n",  *map->raw[0]);
	flush_empty_lines(map->raw);
	close(fd);
	if (strtab_len(map->raw) < 6)
	 	return (error("Missing info in config file.", map));
	else 
		return(strtab_len(map->raw));
	return (0);
}

int	map_checker(t_cub *cub, t_map *map, char *file)
{
	int map_len;

	if (ft_strfcmp(".cub", file, 4))
		return (error("Wrong file extention.", map));
	if ((map_len = read_whole_file(map, file)) == 0)
		return (-1);
	cub->tex_id = -1;
	if (tex_parse(cub, map) < 0)
		return (-1);
	if (transcribe(map) < 3)
		return (error("Map in file is too short", map));
	printf("map_raw len  : (%d)\n", map_len - map->height);
	map->tab = (char **)ft_calloc(sizeof(char *), (map->height + 1));
	if (!map->tab || !map_frame(map) || build_grid_coords_map(map) < 0
		|| build_collision_map(map) < 0)
		return (-1);
	print_collision_map(map);
	printf("map (width, height) : (%d, %d)\n", map->width, map->height);
	return (0);
}

/* WAS LIKE ...
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

/// main >> e_list_txtr >> e_mtrx_count >> get_folder_namr
	///		e_list_txtr >> e_mtrx_link >> t_name_set >> xwalls_builder
///	map_checker >> tex_parse >> get_tex_by_ref >> get_ref_str
	///		map_frame >> wall_chk >> (o_cell removed)

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
