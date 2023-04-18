/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prebuilt_map_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 19:52:56 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/17 15:42:09 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Returns a float ptr to the x world coordinate of the NW corner of the
// (cx, cy) cell coordinate. fptr + 1 is the pointer to the y world coordinate.
float	*get_grid_coords(t_map *map, int cx, int cy)
{
	return (map->grid_coords[cy] + (cx << 1));
}

int	is_wall(t_map *map, int cx, int cy)
{
	return (map->collision_map[cx + cy * map->width]);
}

int	get_is_cell_within_bounds(t_map *map, int cx, int cy)
{
	return ((0 <= cx) && (cx < map->width) && (0 <= cy) && (cy < map->height));
}

// DO NOT include in release
void	print_collision_map(t_map *map)
{
	int	i;
	int	j;

	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (is_wall(map, j, i))
				printf("1");
			else
				printf("0");
		}
		printf("\n");
	}
}

int	build_collision_map(t_map *map)
{
	char	*colls;
	int		i;
	int		j;

	colls = NULL;
	if (!ft_malloc_p(sizeof(char) * map->total_cells, (void **)&colls))
		return (-1);
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
			colls[i * map->width + j] = (map->tab[i][j] == '1'
					|| map->tab[i][j] == '\0');
	}
	map->collision_map = colls;
	return (0);
}

int	build_grid_coords_map(t_map *map)
{
	int		i;
	int		j;
	float	**gcoords;

	gcoords = NULL;
	if (!ft_calloc_p(sizeof(float *) * (map->height + 1), (void **)&gcoords))
		return (-1);
	i = -1;
	while (++i < map->height)
	{
		if (!ft_malloc_p(2 * sizeof(float) * map->width, (void **)&gcoords[i]))
			return (-1);
		j = -1;
		while (++j < map->width)
		{
			gcoords[i][j << 1] = j * CELL_WIDTH;
			gcoords[i][(j << 1) + 1] = i * CELL_WIDTH;
		}
	}
	map->grid_coords = gcoords;
	return (0);
}
