/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/18 22:20:15 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	error_color(char *err_str, char ***split_color)
{
	strtab_clear(split_color);
	ft_eprintf("Error: Failed to parse this color string from file : %s\n",
		err_str);
	return (0);
}

static int	color_split(char *col_str, int *ret_col)
{
	char		**color;
	int			rgb[3];

	*ret_col = 0;
	color = ft_split_set(col_str + 1, ", ");
	if (strtab_len(color) != 3)
		return (error_color(col_str, &color));
	rgb[0] = ft_atoi(color[0]);
	rgb[1] = ft_atoi(color[1]);
	rgb[2] = ft_atoi(color[2]);
	if (strtab_len(color) != 3 || (rgb[0] < 0 || rgb[0] > 255)
		|| (rgb[1] < 0 || rgb[1] > 255) || (rgb[2] < 0 || rgb[2] > 255))
		return (error_color(col_str, &color));
	*ret_col = str_to_color(rgb[0], rgb[1], rgb[2], 0xff);
	strtab_clear(&color);
	return (*ret_col);
}

t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex_str)
{
	char	*t;

	printf("______ HERE GET_BY_ID__[%d]___name{%s}\n", id, tex_str);
	if (id < 0 || id > 3)
		return (NULL);
	if (!cub->tex.walls[id])
	{
		while (*(++tex_str) && ft_isspace(*tex_str))
			continue ;
		t = tex_str;	
		while (*tex_str && !ft_isspace(*tex_str))
			tex_str++ ;
		*tex_str = '\0';
		cub->tex.walls[id] = mlx_load_png(t);
		if (!cub->tex.walls[id])
			return (report_mlx_tex_load_failed(t));
		cub->tex_id++;
	}
	else
	{
		ft_eprintf("Error\n\t- Trying to load texture id %d twice.\n", id);
		return (NULL);
	}	
	return (cub);
}

static int	error_clr(char *err, t_map *map)
{
	strtab_clear(&map->raw);
	strtab_clear(&map->txtr);
	if (err && err[0])
		return (error(err, map));
	return (-1);
}

int	tex_parse(t_cub *cub, t_map *map)
{
	int		nb;
	int		id;

	nb = 0;
	while (nb < 6 && map->raw[nb])
	{
		id = ft_in_set(map->raw[nb][0], (const char *)"WNESCFAB");
		printf("id for tag %d : %c  \n", id,map->raw[nb][0]);
		// printf("id for tag %d \n", map->raw[nb][1]);
		if (id < 0 || map->raw[nb][1] != ' ')
			return (error_clr("Invalid config found!\n", map));
		else if (id > 5)
		{
			if (get_tex_by_ref(cub, id, map->raw[nb]))
				nb = 5;
			else
				return (error_clr(NULL, map));
		}
		else if (id < 4 && !get_tex_by_id(cub, id, map->raw[nb]))
			return (error_clr(NULL, map));
		if (id == 4 || id == 5)
		{
			if (!color_split(map->raw[nb], cub->tex.color + (id - 4)))
				return (-1);
		}	
		printf("tex_parse nb = %d \n", nb);
		nb++;
	}
	if (cub->tex_id != 3)
		return (error_clr("Missing textures. \
			At least one wall texture was not loaded\n", map));
	return (0);
}
