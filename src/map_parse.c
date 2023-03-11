/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:39:58 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/09 22:41:09 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
//#include "texture.h"


// init map struct
t_map	*init_map(t_map *map)
{
	map->pos_x = 1;		// start pos
	map->pos_y = 1;		// start pos
	map->hero_int = -1;	// hero_int refex
	map->width = 0;		// longest row
	map->height = 0;	// how many row
	map->total_cells = 0;		// ref nbr cases total
	map->file = NULL;   // char* path
	map->tab = NULL;    // char** ... map!
	map->flg_chk = 0;   // continu as  long as it stay at ZERO;
	map->lines_to_map = 0;	// nb of lines in file before map description
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

int	is_empty_line(char *line)
{
	if (!line)
		return (0);
	while (*line && ft_isspace(*line))
		line++;
	return (*line == '\0');	
}

// if nb_lines == 0, skips only empty lines. Otherwise skip nb_lines.
char	*skip_file_lines(t_map *map, int fd, int nb_lines)
{
	char	*tmp;
	int	i;

	i = 0;
	tmp = get_next_line(fd);
	while ((!nb_lines && is_empty_line(tmp)) || (tmp && i < nb_lines))
	{
		map->lines_to_map++;
		i++;
		free(tmp);
		tmp = get_next_line(fd);
	}
	return (tmp);
}

//pre-count fonc. set width max
static	int	transcribe(t_map *map, int fd)//char *file)
{
	char *tmp;

	tmp = skip_file_lines(map, fd, 0);
	map->width = int_strlen(tmp);
	map->height = 0;
	while (tmp != NULL)
	{
		map->height++;
		if (int_strlen(tmp) > map->width)
			map->width = int_strlen(tmp);
		free(tmp);
		tmp = get_next_line(fd);
	}
	map->total_cells = (map->height * map->width);
	printf("DEBUG: map->h = %d:\n", map->height);
	printf("DEBUG: map->w = %d:\n", map->width);
	printf("DEBUG: map->total_cells = %d:\n", map->total_cells);
	return (map->height);
}

//	set foam into char_less map_part
static t_map	*map_frame(t_map *map, int fd)
{
	t_map 	*dup;
	char	*temp;
	int 	nb;// actual line 

	temp = skip_file_lines(map, fd, map->lines_to_map);
	nb = 0;
	printf("*** map frame entered\n");
	while (temp && (nb < map->height))
	{
		printf("map frame %d : %s", nb, temp);
		map->tab[nb] = (char *)ft_calloc(sizeof(char *), (map->width + 1));	
		if (map->width == int_strlen(temp))
		{
			map->tab[nb] = ft_strncpy_i(map->tab[nb], temp, map->width, 0);
			map->tab[nb] = spc_chk(map, nb);
		}
		else
			map->tab[nb] = ft_strncpy_i(map->tab[nb], temp, map->width, 0);
		free(temp);
		nb++;
		temp = get_next_line(fd);
	}
	if (temp)
		free(temp);
	dup = map;
	map = wall_check(dup);
	if (map->flg_chk == 1)
		return (NULL);
	return (map);
}

// open , pre-count , calloc , padding ,  needed:(tex_parse)
int	map_checker(t_cub *cub, t_map *map, char *file)
{
	int		fd;

	/// OPEN FILE
		if (ft_strfcmp(".cub", file, 4))
			return (error("Wrong file extention.", map));
		fd = open(file, O_RDONLY);
		if (fd < 0)
			return (error("Could not open file", map));
	/// TEX_PARSE
		if (tex_parse(cub, map, fd) < 0)
		return (-1);
		printf("*** tex_parse DONE\n");
	///	TRANSCRIBE
		if (transcribe(map, fd) < 3)// pre-count
			return (error("Map in file is too short", map));
		close(fd);
		printf("transcribe DONE\n");
	/// RE-OPEN CALLOC
		fd = open(file, O_RDONLY);
		map->tab = (char **)ft_calloc(sizeof(char *), (map->height + 1));
		printf("tab calloc DONE\n");
	// MULTI_CHECK CONDITION MAP & MAP_FRAME + BUILD ...
	if (!map->tab || !map_frame(map, fd) || !printf("map_frame DONE !\n") || build_grid_coords_map(map) < 0
		|| !printf("build_grid_coords DONE\n") || build_collision_map(map) < 0)
		return (-1);
	
	printf("collision map DONE \n\n");
	printf("Parsed map :\n");
	// print_map(map);
	printf("Collision map :\n");
	print_collision_map(map);
	printf("DEBUG:Z. AFTER map_frame NEXT wall_chk\n");		
	close(fd);
	
	return (0);
}
