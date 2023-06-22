/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/22 00:08:18 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_matrx	*pset_maker(t_cub *cub, char **raw, int queue, int len)
{
	int		fill;
	int		id;
	char	*ref;

	id = 0;
	fill = -1;
	ref = raw[queue];
	if (ft_in_set((const char)raw[queue][0], (const char *)UCHR) > -1)
	{
		while (++fill < 4)
		{
			id = ft_in_set((const char)ref[fill + 2], LCHR);
			if (id != -1)
				cub->pset[len].xwalls[fill] = cub->box.xform[id];
			else
				return (NULL);
		}
	}
	return (cub->pset);
}

t_box	*xwalls_builder(t_cub *cub, char **raw)
{
	int		queue;
	int		len;
	char	*ref;

	queue = (cub->box.xnum + cub->box.meta - 1);
	len = 0;
	while (len < cub->box.pset)
	{
		ref = raw[queue];
		printf("PSET:: CHAR>>(%c)", ref[0]);
		printf(" :: <<%d of %d>> \n", len + 1, cub->box.pset);
		if (!pset_maker(cub, raw, queue, len))
			return (NULL);
		len++;
		queue++;
	}
	return (&cub->box);
}

int	tex_parse(t_cub *cub, t_map *map)
{
	int	id;
	int	nb;
	int	i;

	i = 0;
	printf("Tex_parse...\n\n");
	cub->box.n_dual = 0;
	cub->box.pset = 0;
	if (!e_list_txtr(cub, &cub->box, map))
		return (-1);
	nb = cub->box.xnum + cub->box.meta - 1;
	while (map->raw[nb] && map->raw[nb][0] != ' ' && i < cub->box.pset)
	{
		id = ft_in_set(map->raw[nb][0], (const char *)UCHR);
		if (id < 0 || map->raw[nb][1] != ' ')
			return (-1);
		nb++;
		i++;
	}
	cub->pset = (t_matrx *)calloc(sizeof(t_matrx), cub->box.pset);
	if (!cub->pset)
		return (-1);
	if (!xwalls_builder(cub, map->raw))
		return (-1);
	return (0);
}
