/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_extend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghebert <ghebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 23:18:50 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/21 09:38:11 by ghebert          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "cub3d.h"

int	xform_flush(t_cub *cub)
{
	int	xf;

	if (!cub->box.xform)
		return (0);
	xf = 0;
	cub->box.xnum -= (cub->box.n_dual);
	while (cub->box.xform[cub->box.xnum] && xf < cub->box.xnum - 1)
	{
		while (xf < cub->box.xnum)
		{
			if (cub->box.xform[xf])
			{
				mlx_delete_texture(cub->box.xform[xf]);
				cub->box.xform[xf] = NULL;
			}
			xf++;
		}
		xf++;
	}
	return (0);
}

void	dual_flush(t_cub *cub)
{
	int	i;
	int	j;

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
	// int	xf;

	// xf = 0;
	// cub->box.xnum -= (cub->box.n_dual);
	// while (cub->box.xform[cub->box.xnum] )
	// {
	// 	xform_flush(cub);
	// 	ft_free_p((void **)&cub->box.xform);
	// }
	if (cub->dual)
		dual_flush(cub);
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
