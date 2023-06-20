/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_extend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 23:18:50 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/20 18:50:17 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
void	pset_flush(t_cub *cub)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (j < cub->box.pset && cub->pset[j].xwalls[i])
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
*/

void	dual_flush(t_cub *cub)
{
	int	i;
	int	j;

	printf("DUAL FLUSH START :: dual ptr %p :: n_dual = %d\n", cub->dual, cub->box.n_dual);
	j = 0;
	i = 0;
	while (j < cub->box.n_dual)
	{
		i = 0;
		printf("dual flush idx j %d, i %d\n", j, i);
		while (i < 2 && cub->dual[j].xwalls[i])
		{
			printf("FLUSH:DUAL[%d]::xwalls[%d]", j, i);
			printf(" <<%p>>\n", cub->dual[j].xwalls[i]);
			mlx_delete_texture(cub->dual[j].xwalls[i]);
			cub->dual[j].xwalls[i] = NULL;
			i++;
		}
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
	while (i < cub->box.meta && cub->box.objx[i])
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

	if (!cub->box.xform)
		return (0);
	xf = 0;
	cub->box.xnum -= (cub->box.n_dual);
	if (cub->box.xform[0])
	{
		//while (cub->box.xform[cub->box.xnum] && xf < cub->box.xnum)// - 1)
		while (xf < cub->box.xnum)// - 1)
		{
			if (cub->box.xform[xf])
			{
				printf("FLUSH:XFORM[%d]", xf);
				printf(" <<%p>>\n", cub->box.xform[xf]);
				mlx_delete_texture(cub->box.xform[xf]);
				cub->box.xform[xf] = NULL;
			}
			//free(cub->box.xform[xf]);
			//cub->box.xform[xf] = NULL;
			xf++;
		}
	}
	ft_free_p((void **)&cub->box.xform);
	// if (cub->box.xform)
	//	free(cub->box.xform);
	// else 
	// 	return (0);
	if (cub->dual)
		dual_flush(cub);
//	if (cub->pset)
//		pset_flush(cub);
	// else 
	// 	return (0);
	if (cub->box.objx)
		objx_flush(cub);
	ft_free_p((void **)&cub->pset);
	strtab_clear(&cub->map.raw);
	strtab_clear(&cub->map.m);
	strtab_clear((char ***)&cub->map.mx);
	free(cub->box.chrs);
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
