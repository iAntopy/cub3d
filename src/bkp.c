/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 08:22:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/24 08:22:28 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <dirent.h> 


static t_cub	*map_frame(t_map *map, t_cub *cub)
{

	// char	**m;
	int		i;	// = 0;
	int 	q;	// after txtr ptr
	map->mx = (t_matrx ***)malloc(sizeof(t_matrx **) * map->height);
	map->mx[k] =(t_matrx**)malloc(sizeof(t_matrx *) * map->width);

	i = 0;
	q = strtab_len(map->raw) - map->height;	
	// m = (char **)malloc(sizeof(char *) * map->height + 1);

	while(i < map->height)
	{
		m[i] = (char *)ft_calloc(sizeof(char *), (map->width + 1));
		printf("%s\n", map->raw[q]);
		ft_strlcpy(map->raw[q], m[i], map->width + 1);
		map->raw[i] = spc_chk(map, q);
		m[i] = map->raw[q];
		++q;
		++i;
	}
	if (!wall_check(cub, map))
		printf("wall_chk returned with flg_chk == 1\n");
	cub->map.m = m;
	return (cub);
}
