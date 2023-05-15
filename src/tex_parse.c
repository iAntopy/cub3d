/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/15 00:17:53 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_matrx	*pset_maker(t_cub *cub, char **raw, int queue, int len)
{
	int		fill;
	int		id;
	char	*ref;

	id = 0;
	fill = -1;
	ref = raw[queue];
	if (ft_in_set((const char)raw[queue][0], (const char *)MAP_UCHR) > -1)
	{
		printf("PSET MAKER !!\n");
		while (fill++ < 3)
		{
			id = ft_in_set((const char)ref[fill + 2], MAP_LCHR);
			// printf("MAKER: recett{%c} index[%d]\n", ref[fill + 2],
				// id);
			if (id != -1)
				cub->pset[len].xwalls[fill] = cub->box.xform[id];
			//  printf(" cub->pset.Xwals<<%p>> ::\n", cub->pset[len].xwalls[fill]); 
		}
	}
	return (cub->pset);
}

t_box	*xwalls_builder(t_cub *cub, char **raw)
{
	int		queue;
	int		len;
	char	*ref;

	queue = (cub->box.xnum  + cub->box.meta);
	len = 0;
	while (len < cub->box.pset)
	{
		ref = raw[queue];
		
		printf("PSET:: CHAR>>(%c)", ref[0]);
		printf(" :: <<%d of %d>> \n", len + 1, cub->box.pset);		
		cub->pset = pset_maker(cub, raw, queue, len);
		len++;
		queue++;
	}
	return (&cub->box);
}

static int	error_clr(char *err, t_map *map)
{
	strtab_clear(&map->raw);
	if (err && err[0])
		return (error(err, map));
	return (-1);
}

int	tex_parse(t_cub *cub, t_map *map)
{
	int	id;
	int	nb;
	
	cub->box.n_dual = 0;
	cub->box.pset = 0;
	nb = cub->box.xnum;
	cub = e_list_txtr(cub, &cub->box, map);
		
	int	nb;
	nb = cub->box.xnum;
	////		///		///
	while (map->raw[nb] && map->raw[nb][0] != ' ')
	{
		id = ft_in_set(map->raw[nb][0], (const char *)MAP_UCHR);
		if (id < 0 || map->raw[nb][1] != ' ')
			break;
		printf("PSET[%d]:: PNUM[%d]  REF = %c \n", cub->box.pset, cub->box.pnum, map->raw[nb][0]);
		cub->box.pset++;
		nb++;
	}
	cub->box.tot = cub->box.xnum + cub->box.meta + cub->box.pset;
	cub->pset = (t_matrx *)malloc(sizeof(t_matrx) * cub->box.pset);
	if (!cub->pset)
		return (-1);
	if (!xwalls_builder(cub, map->raw))
		return (error_clr(NULL, map));
	return (0);
}

