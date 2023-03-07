/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:39:58 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/06 09:30:21 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include "../include/textur.h"


// init map struct
t_map	*init_map(t_map *map)
{
    map = malloc(sizeof(t_map));
	
	map->pos_x = 1;		// start pos
	map->pos_y = 1;		// start pos
	map->width = 0;		// longest row
	map->height = 0;	// how many row
	map->cases = 0;		// ref nbr cases total
    map->file = NULL;   // char* path
    map->tab = NULL;    // char** ... map!
	
	return (map);
}

// mapping space void into the map
/// need error chk imprv
static	char *spc_chk(t_map *map, int j)
{	
	int i;
	int h;
	const char *set;

	h = 0;
	set = "01WNES";
	i =	ft_in_set((char const*)map->tab[j], set);
	while (h < i)
	{
		// write(1, &map->tab[j][h], 1);
		h++;
	// 		// error("6. Map contains unrecognized characters", map);
	}
	write(1, "\n", 1);
	return (map->tab[j]);
}

//pre-count fonc. set width max
static	t_map	*transcribe(t_map *map, char *file)
{
	int		fd;
	char *tmp;

	fd = open(file, O_RDONLY);
	tmp = get_next_line(fd);
	map->width = int_strlen(tmp);
	map->height = 0;
	while (tmp != NULL)
	{
		tmp = get_next_line(fd);
		map->height++;
		if (int_strlen(tmp) > map->width)
			map->width = int_strlen(tmp);
	}
	close(fd);
	///
	map->cases = (map->height * map->width);
	printf("DEBUG: map->h = %d:\n", map->height);
	printf("DEBUG: map->w = %d:\n", map->width);
	printf("DEBUG: map->cases = %d:\n", map->cases);
	//	// tex_parse();
	
	return (map);
}

//	set foam into char_less map_part
static t_map	*map_frame(t_map *map, int fd)
{
	char	*temp;
	int 	nb;  // actual line 

	nb = 0;
	while (nb < map->height)
	{
		temp = get_next_line(fd);
		if (!temp)
			break;
		map->tab[nb] = (char *)ft_calloc(sizeof(char *), (map->width + 1));	
		if (map->width == int_strlen(temp))
		{
			
			map->tab[nb] = ft_strncpy_i(map->tab[nb], temp, map->width, 0);
			map->tab[nb] = 	spc_chk(map, nb);
		}
		else if(map->width != int_strlen(temp))
			map->tab[nb] = ft_strncpy_i(map->tab[nb], temp, map->width, 0);
		else					
			error("3. There is an error in your map, Please verify\n", map);
		free(temp);
		nb++;
	}
	free(temp);
	return (map);
}

// open , pre-count , calloc , padding ,  needed:(tex_parse)
t_cub	map_checker(t_cub cub, t_map *map, char *file)
{
	int		fd;
	//
		fd = open(file, O_DIRECTORY);
		if (fd > 0)
			error("1. You are trying to open a dir.\n", map);
		fd = open(file, O_RDONLY);
		if (fd < 0 || ft_strfcmp(".cub", file, 4))
			error("2. Wrong file name or extention\n", map);
	
	/// should manage texture	part of file
	map = tex_parse(cub, map, fd);
	printf("DEBUG:Z. AFTER txtr_PARSE\n");	

	map = transcribe(map, file); // pre-count
	map->tab = (char **)ft_calloc(sizeof(char *), (map->height + 1));
	printf("DEBUG:Z. AFTER map_PARSE\n");	
	map = map_frame(map, fd);
	printf("DEBUG:Z. AFTER map_frame\n");		
	close(fd);
	// 	// wall_check(map, game);
	return (cub);
}
