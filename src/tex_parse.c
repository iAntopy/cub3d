/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/16 21:55:52 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex)
{
	printf("______ HERE GET_BY_ID__[%d]___name{%s}\n", id, tex);
	if (id < 0 || id > 3)
		return (NULL);
	if (!cub->tex.walls[id])
	{
		cub->tex.walls[id] = mlx_load_png(tex);
		cub->tex_id++;
	}
	if (!cub->tex.walls[id])
	{
		printf("DEBUG WARNING : tex %s id %d failed to load !\n", tex, id);
		return (NULL);
	}
	return (cub);
}

static int	error_clear(char *err, t_map *map)
{
	if (map->txtr)
		strtab_clear(&map->txtr);
	return (error(err, map));
}

int	tex_parse(t_cub *cub, t_map *map)
{
	int		nb;
	int		id;

	nb = 0;
	while (nb < 6)
	{
		map->txtr = ft_split(map->raw[nb], ' ');
		id = ft_in_set(map->txtr[0][0], (const char *)"WNESCF");
		if (id < 0)
			return (error_clear("Invalid config label found!\n", map));
		else if (id < 4 && !get_tex_by_id(cub, id, map->txtr[1]))
			return (error_clear("Texture load error!\n", map));
		else if (id == 4 || id == 5)
			cub->tex.color[id - 4] = color_split(map, id);
		strtab_clear(&map->txtr);
		nb++;
	}
	if (cub->tex_id != 3)
		return (-1);
	return (0);
}
