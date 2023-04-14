/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/12 22:02:34 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex)
{
	printf("______ HERE GET_BY_ID__[%d]___name{%s}\n", id, &tex[0]);
	if (id == 0)
		cub->tex.walls[W_SIDE] = mlx_load_png(&tex[0]);
	else if (id == 1)
		cub->tex.walls[N_SIDE] = mlx_load_png(&tex[0]);
	else if (id == 2)
		cub->tex.walls[E_SIDE] = mlx_load_png(&tex[0]);
	else if (id == 3)
		cub->tex.walls[S_SIDE] = mlx_load_png(&tex[0]);
	return (cub);
}

static int	setup_wall_textures(t_cub *cub)
{
	char			**src;
	mlx_texture_t	**dst;

	src = cub->tex.tex_n;
	dst = cub->tex.walls;
	if (src[W_SIDE] && src[N_SIDE] && src[E_SIDE] && src[S_SIDE])
	{
		printf("All textures available :  \n- %s- %s- %s- %s\n", src[W_SIDE],
			src[N_SIDE], src[E_SIDE], src[S_SIDE]);
		dst[W_SIDE] = mlx_load_png(src[W_SIDE]);
		dst[N_SIDE] = mlx_load_png(src[N_SIDE]);
		dst[E_SIDE] = mlx_load_png(src[E_SIDE]);
		dst[S_SIDE] = mlx_load_png(src[S_SIDE]);
	}
	return (cub->tex.walls[W_SIDE] && cub->tex.walls[N_SIDE]
		&& cub->tex.walls[E_SIDE] && cub->tex.walls[S_SIDE]);
}

static int	error_clear(char *err, t_map *map, char ***txtr)
{
	if (txtr)
		strtab_clear(txtr);
	return (error(err, map));
}

static char	**t_get_liner(t_map *map, int fd, char **txtr)
{
	char	*line;

	line = get_next_line(fd);
	while (*line == '\n' || ft_strlen(line) < 2)
	{
		map->lines_to_map++;
		free(line);
		line = get_next_line(fd);
	}
	if (line)
	{
		map->lines_to_map++;
		txtr = ft_split(line, ' ');
		if (txtr[1][ft_strlen(txtr[1]) - 1] == '\n')
			txtr[1][ft_strlen(txtr[1]) - 1] = '\0';
		if (ft_strlen(txtr[0]) > 2)
			return (error_clear("7, Texture Ref error !\n", map, &txtr));
		if (!txtr[1])
			return (error_clear("8, Texture NameS error !\n", map, &txtr));
		free(line);
	}
	return (txtr);
}

int	tex_parse(t_cub *cub, t_map *map, int fd)
{
	char	**txtr;
	int		nb;
	int		id;

	nb = 0;
	while (nb < 6)
	{
		txtr = t_get_liner(map, fd, txtr);
		id = ft_in_set((const char *)txtr[0], (const char *)"WNESCF");
		if (id < 0)
			return (error_clear("9, Texture unmatching!\n", map, &txtr));
		else if (id < 4)
			cub->tex.tex_n[id] = txtr[1];
		else
			cub->tex.color = color_split(txtr, id);
		nb++;
	}
	return (setup_wall_textures(cub));
}
