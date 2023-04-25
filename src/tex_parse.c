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

/// Now pre_read folder +  Malloc + post_read linked
t_cub  *e_list_txtr(t_cub *cub)
{ 
    t_matrx *mx;
	// mlx_texture_t	**xform;

    mx = malloc(sizeof(t_matrx) * 1);
    if (!mx)
        return (NULL);
    // mx->xnum = e_mtrx_count(mx, full_path);
    // mx->full = malloc(sizeof(char *) * (mx->xnum + 1));
    // mx = e_mtrx_link(mx);
	mx->xnum = 0;
    cub->mx = mx;

    return (cub); 
}

t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex_str)
{
	char	*t;
	// mlx_texture_t	*xform;

	printf("______ HERE GET_BY_ID__[%d]___name{%s}\n", id, tex_str);
	
	cub->tex_id++;
	if (!cub->mx->xwalls[id])
	{
		while (*(++tex_str) && ft_isspace(*tex_str))
			continue ;
		t =  tex_str;
		// t =  ft_substr(tex_str,0, ft_strlen(tex_str) - 2);
		while (*tex_str && !ft_isspace(*tex_str))
			tex_str++ ;
		*tex_str = '\0';
		printf("tex_by_id __ t** = {%s} \n", t);
		cub->mx->xwalls[id] = mlx_load_png(t);
		if (cub->mx->xwalls[id])
			return (report_mlx_tex_load_failed(t));
	}
		// if (!cub->tex.walls[id])
		// {
		// 	while (*(++tex_str) && ft_isspace(*tex_str))
		// 		continue ;
		// 	t = tex_str;	
		// 	while (*tex_str && !ft_isspace(*tex_str))
		// 		tex_str++ ;
		// 	*tex_str = '\0';
		// 	cub->tex.walls[id] = mlx_load_png(t);
		// 	if (!cub->tex.walls[id])
		// 		return (report_mlx_tex_load_failed(t));
		// 	cub->tex_id++;
		// }
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

	nb = 1;
	cub =  e_list_txtr(cub);
	printf("id for tag %d \n", map->raw[nb][0]);
	printf("id for tag %s \n", map->raw[nb]);
	while (map->raw[nb] )
	{
		id = ft_in_set(map->raw[nb][0], (const char *)MAP_LCHR);
		printf(" %d)-  tag to lower:%c  __[%d]id \n", nb, map->raw[nb][0], id);
		if (id < 0 || map->raw[nb][1] != ' ')
		{
			id = ft_in_set(map->raw[nb][0], (const char *)MAP_NCHR);
			if (id < 0 || map->raw[nb][1] != ' ')
			{
				id = ft_in_set(map->raw[nb][0], (const char *)MAP_UCHR);
				if (id < 0 || map->raw[nb][1] != ' ')
					return (error_clr("Invalid config found!\n", map));
				else if (!get_tex_by_ref(cub, id, map->raw[nb]))
					return (error_clr(NULL, map));
					cub->mx->xnum++;
			}
		}
		else if (!get_tex_by_id(cub, id, map->raw[nb]))
			return (error_clr(NULL, map));
		printf("tex_parse __ xnum = %d \n", cub->mx->xnum);
		nb++;
	}
	// if (cub->tex_id != 3)
	// 	return (error_clr("Missing textures. \
	// 		At least one wall texture was not loaded\n", map));
	return (0);
}
