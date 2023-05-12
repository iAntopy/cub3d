/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/11 20:36:25 by gehebert         ###   ########.fr       */
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
	else if ((ft_in_set((const char)raw[queue][0],
			(const char *)MAP_NCHR) > -1))
	{
		cub->pset[len].xwalls[0] = cub->box.xform[queue];
		cub->pset[len].xwalls[1] = NULL;
	}
	/* printf("MAKER: UNIQ:: ptr<<%p>> ::\n", cub->box.xform[queue]);*/
	return (cub->pset);
}

t_box	*xwalls_builder(t_cub *cub, char **raw)
{
	int		queue;
	int		len;
	char	*ref;

	// queue = cub->box.xnum - cub->box.pnum;
	queue = (cub->box.xnum  + cub->box.meta);
	len = 0;
	while (len < cub->box.pset)
	{
		ref = raw[queue];
		
		printf("PSET:: CHAR>>(%c)", ref[0]);
		printf(" :: <<%d of %d>> \n", len + 1, cub->box.pset);
		// printf("START RAW NB[%d]=> REF %c \n", len, raw[queue][0]);
		
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
	
	printf("Tex_parse...\n\n");
	cub->box.dual = 0;
	cub->box.pset = 0;
	
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
	////		///		///	
	// cub->box.pset = cub->box.pnum;
	cub->box.tot = cub->box.xnum + cub->box.meta + cub->box.pset;
	
		printf("\n<<META[%d]::XNUM[%d]", cub->box.meta, cub->box.xnum);
		printf("::PSET[%d]::TOT[%d]>> \n\n", cub->box.pset, cub->box.tot);

	cub->pset = (t_matrx *)malloc(sizeof(t_matrx) * cub->box.pset);
	if (!cub->pset)
		return (-1);
	if (!xwalls_builder(cub, map->raw))
		return (error_clr(NULL, map));
		
	return (0);
}

/* 
	Start with 	: VER.3
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
