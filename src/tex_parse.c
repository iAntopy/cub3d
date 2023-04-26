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

// t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex_str)
	// {
	// 	char	*t;
	// 	// mlx_texture_t	*xform;

	// 	printf("______ HERE GET_BY_ID__[%d]___name{%s}\n", id, tex_str);
		
	// 	cub->tex_id++;
	// 	if (!cub->mx->xwalls[id])
	// 	{
	// 		while (*(++tex_str) && ft_isspace(*tex_str))
	// 			continue ;
	// 		t =  tex_str;
	// 		// t =  ft_substr(tex_str,0, ft_strlen(tex_str) - 2);
	// 		while (*tex_str && !ft_isspace(*tex_str))
	// 			tex_str++ ;
	// 		*tex_str = '\0';
	// 		printf("tex_by_id __ t** = {%s} \n", t);
	// 		cub->mx->xwalls[id] = mlx_load_png(t);
	// 		if (cub->mx->xwalls[id])
	// 			return (report_mlx_tex_load_failed(t));
	// 	}
	// 		// if (!cub->tex.walls[id])
	// 		// {
	// 		// 	while (*(++tex_str) && ft_isspace(*tex_str))
	// 		// 		continue ;
	// 		// 	t = tex_str;	
	// 		// 	while (*tex_str && !ft_isspace(*tex_str))
	// 		// 		tex_str++ ;
	// 		// 	*tex_str = '\0';
	// 		// 	cub->tex.walls[id] = mlx_load_png(t);
	// 		// 	if (!cub->tex.walls[id])
	// 		// 		return (report_mlx_tex_load_failed(t));
	// 		// 	cub->tex_id++;
	// 		// }
	// 	else
	// 	{
	// 		ft_eprintf("Error\n\t- Trying to load texture id %d twice.\n", id);
	// 		return (NULL);
	// 	}	
	// 	return (cub);
// }

/* Malloc str:id(txtr_ref)  str:path(txtr_path)*/
t_box *xwalls_builder(t_cub *cub, char **raw, t_matrx *pset)
{
    int     fill;
    int     len;
    int 	id;
	int 	queue;
	char	*ref;
    
    id = 0;
    len = 0;
	fill = -1;
	(void)ref;
	queue = cub->box.xnum - cub->box.pnum;
    printf("XWLLS PRESET {%s} max len = %d \n", raw[queue], cub->box.pset);
    while (len < cub->box.pset)
    {
        printf("START RAW NB[%d]=> REF %c \n", len, raw[queue][0]);
		if (len < cub->box.pset && ft_in_set((const char )raw[queue][0], (const char *)MAP_UCHR) > -1)
		{
			// printf("#### BUILDER  MULTI:  REF(%c) recett{%s} <<ID %d>> \n", ref[0], ref, id);
			fill = -1;
			while(fill++ < 3)
			{
				ref = raw[queue];
				id = ft_in_set((const char)ref[fill + 2], MAP_LCHR);
				printf("BUILDER  MULTI:  REF(%c) recett{%c} <<ID %d>> \n", ref[0], ref[fill + 2], id);
				if (id != -1)
				{
					pset[queue].xwalls[fill] =  cub->box.xform[id];
				}
			}
			queue++;
    	}
		else if (cub->box.pnum && (ft_in_set((const char)raw[queue][0], (const char *)MAP_NCHR) > -1))
		{
			printf("BUILDER UNIQ : REF(%c) \n", raw[queue][0]);
			pset[queue].xwalls[0] = cub->box.xform[queue];
			pset[queue].xwalls[1] = NULL;
			++queue;
		}
		len++;
	}
    return (&cub->box);
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
	t_matrx *pset;
	int		nb;
	int		id;

	cub =  e_list_txtr(cub, &cub->box, map);
	nb = cub->box.xnum;
	cub->box.pset = cub->box.pnum;
	while (map->raw[nb] && map->raw[nb][0] != ' ')
	{
		id = ft_in_set(map->raw[nb][0], (const char *)MAP_UCHR);
		if (id < 0 || map->raw[nb][1] != ' ')
			return (error_clr("Invalid config found!\n", map));
		cub->box.pset++;
		nb++;
	}
	pset = (t_matrx *)malloc(sizeof(t_matrx) * cub->box.pset);
	if (!pset)
        return (-1);
	if (!xwalls_builder(cub, map->raw, pset))
		return (error_clr(NULL, map));
	return (0);
}
		// printf("raw->xnum = %d \n", cub->box->xnum);
		// printf("id for tag %s \n", map->raw[nb]);

/* 	Start with 	: VER.3
		: 	map->raw	-->	first lecture total len of the file
		:		NCHR	-->	floor txtr legend	
		:		LCHR	-->	wall  txtr legend
		:		UCHR	-->	wall  txtr preset
	Need to set	:
		:		xnum	how many legend to build  (lower and num)
		:		pset	how many preset to malloc	(uppercase)
	Frame builder
		:		FRAME one	: legend
		:		xform = malloc(sizeof(mlx_texture_t *) * xnum)
		:		FRAME two	: preset
		:		xwalls = malloc(sizeof(void *) * 4)
*/

