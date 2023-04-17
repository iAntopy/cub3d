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
	/*
	if (id == 0)
		cub->tex.walls[W_SIDE] = mlx_load_png(tex);
	else if (id == 1)
		cub->tex.walls[N_SIDE] = mlx_load_png(tex);
	else if (id == 2)
		cub->tex.walls[E_SIDE] = mlx_load_png(tex);
	else if (id == 3)
		cub->tex.walls[S_SIDE] = mlx_load_png(tex);
	else
		return (NULL);
	*/
	return (cub);
}
/*
static int	setup_wall_textures(t_cub *cub, char ***txtr)
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
		printf("All loaded texture ptrs :  \n- %p- %p- %p- %p\n", 
			dst[W_SIDE], dst[N_SIDE], dst[E_SIDE], dst[S_SIDE]);

	}
	strtab_clear(txtr);
	return (cub->tex.walls[W_SIDE] && cub->tex.walls[N_SIDE]
		&& cub->tex.walls[E_SIDE] && cub->tex.walls[S_SIDE]);
}
*/

static int	error_clear(char *err, t_map *map)
{
	if (map->txtr)
		strtab_clear(&map->txtr);
	return (error(err, map));
}
/*
static char	**t_get_liner(t_map *map, int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line && (*line == '\n' || ft_strlen(line) < 2))
	{
		map->lines_to_map++;
		ft_free_p((void **)&line);
		line = get_next_line(fd);
	}
	if (line)
	{
		map->lines_to_map++;
		if (map->txtr)
		{
			printf("clearing txtr\n");
			strtab_clear(&map->txtr);
		}
		map->txtr = ft_split(line, ' ');
		ft_free_p((void **)&line);
		if (!map->txtr[0] || !map->txtr[1] || ft_strlen(map->txtr[0]) > 2)
		{
			error_clear("Texture ref format error !\n", map);
			return (NULL);
		}
		else if (map->txtr[1][ft_strlen(map->txtr[1]) - 1] == '\n')
			map->txtr[1][ft_strlen(map->txtr[1]) - 1] = '\0';
	}
	if (line)
		ft_eprintf("DEBUG WARNING : returning from get_liner with line none NULL.\n");
	return (map->txtr);
}
*/

int	tex_parse(t_cub *cub, t_map *map)
{
	int		nb;
	int		id;

	nb = 0;
	while (nb < 6)
	{
//		if (!t_get_liner(map, fd))
//			return (-1);
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
