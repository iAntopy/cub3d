/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_extend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghebert <ghebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 23:18:50 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/20 15:35:46 by ghebert          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "cub3d.h"

void	pset_flush(t_cub *cub)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (cub->pset[j].xwalls[i] && j < cub->box.pset)
	{
		i = 0;
		if (cub->pset[j].xwalls[i])
		{
			while (i < 4)
			{
				printf(":: FLUSH__:PSET[%d]:: xwalls[%d]:: <<%p>> \n", j, i,
					cub->pset[j].xwalls[i]);
				cub->pset[j].xwalls[i] = NULL;
				i++;
			}
		}
		j++;
	}
	printf(":: END__:PSET\n");
	free(cub->pset);
	cub->pset = NULL;
}

void	dual_flush(t_cub *cub)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (cub->dual[j].xwalls[i] && j < cub->box.n_dual)
	{
		i = 0;
		if (cub->dual[j].xwalls[i])
		{
			while (i < 2)
			{
				free(cub->dual[j].xwalls[i]);
				cub->dual[j].xwalls[i] = NULL;
				i++;
			}
		}
		else
			break ;
		j++;
	}
	printf(":: END__:DUAL:: \n");
	free(cub->dual);
	cub->dual = NULL;
}

void	objx_flush(t_cub *cub)
{
	int	i;

	if (cub->box.objx)
		printf(":: INTO__:OBJX:: \n");
	i = 0;
	while (cub->box.objx[i] && i < cub->box.meta)
	{
		if (cub->box.objx[i])
		{
			printf(":: FLUSH__:OBJX[%d]:: <<%p>> \n", i, &cub->box.objx[i]);
			free(cub->box.objx[i]);
			cub->box.objx[i] = NULL;
			i++;
		}
	}
	printf(":: END__:OBJX:: [%d] \n", i);
	free(cub->box.objx);
	cub->box.objx = NULL;
}

int	clr_legend_strct(t_cub *cub)
{
	int	xf;

	xf = 0;
	cub->box.xnum -= (cub->box.n_dual);
	while (cub->box.xform[cub->box.xnum] )
	{
		if (!*cub->box.xform)
		{
			mlx_delete_texture(cub->box.xform[xf]);
			cub->box.xform[xf] = NULL;
		}
		xf++;
	}
	if (cub->pset)
		pset_flush(cub);
	else
		return (0);
	if (cub->dual)
		dual_flush(cub);
	if (cub->box.objx)
		objx_flush(cub);
	strtab_clear(&cub->map.raw);
	strtab_clear(&cub->map.m);
	return (0);
}

t_cub	*mapx_alt_pos(t_map *m, t_cub *cub, int p_box)
{
	m->pos_y = -1;
	while (++m->pos_y < m->height)
	{
		m->pos_x = -1;
		while (++m->pos_x < m->width)
		{
			p_box = ft_in_set((m->m[m->pos_y][m->pos_x]), MCHR);
			if (p_box > -1 || p_box == ((int)ft_strlen(cub->box.chrs) - 1))
			{
				if (ft_in_set((m->m[m->pos_y][m->pos_x - 1]), NCHR) != -1)
					m->mx[m->pos_y][m->pos_x] = m->mx[m->pos_y][m->pos_x - 1];
				else if (ft_in_set((m->m[m->pos_y + 1][m->pos_x]), NCHR) != -1)
					m->mx[m->pos_y][m->pos_x] = m->mx[m->pos_y + 1][m->pos_x];
				else if (ft_in_set((m->m[m->pos_y - 1][m->pos_x]), NCHR) != -1)
					m->mx[m->pos_y][m->pos_x] = m->mx[m->pos_y - 1][m->pos_x];
				else if (ft_in_set((m->m[m->pos_y][m->pos_x + 1]), NCHR) != -1)
					m->mx[m->pos_y][m->pos_x] = m->mx[m->pos_y][m->pos_x + 1];
			}
		}
	}
	return (cub);
}
