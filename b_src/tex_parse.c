/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/21 22:45:58 by gehebert         ###   ########.fr       */
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
		cub->pset = pset_maker(cub, raw, queue, len);
		if (!cub->pset)
			return (NULL);
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
	int	i;

	i = 0;
	printf("Tex_parse...\n\n");
	cub->box.n_dual = 0;
	cub->box.pset = 0;
	if (!e_list_txtr(cub, &cub->box, map))
		return (-1);
	nb = cub->box.xnum + cub->box.meta - 1;
	while (map->raw[nb] && map->raw[nb][0] != ' ' && i++ < cub->box.pset)
	{
		id = ft_in_set(map->raw[nb][0], (const char *)UCHR);
		if (id < 0 || map->raw[nb][1] != ' ')
			return (-1);
		nb++;
	}
	cub->pset = (t_matrx *)calloc(sizeof(t_matrx), cub->box.pset);
	if (!cub->pset)
		return (-1);
	if (!xwalls_builder(cub, map->raw))
		return (error_clr(NULL, map));
	return (0);
}
