/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_extend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 23:18:50 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/23 19:07:25 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	xform_flush(t_cub *cub)
{
	int	xf;

	printf("xform_flush : start\n");
	if (!cub->box.xform)
		return (0);
	printf("xform_flush : check passed\n");
	xf = 0;
	cub->box.xnum -= (cub->box.n_dual);
	printf("xform_flush : cub->box.xnum : %d\n", cub->box.xnum);
	while (xf < cub->box.xnum && cub->box.xform[xf])
	{
		printf("xform_flush : xnum : %d\n", xf);
		if (cub->box.xform[xf])
		{
			printf("<<>>DBL_CHEK xf:[%d],xnum:[%d]\n", xf, cub->box.xnum);
			mlx_delete_texture(cub->box.xform[xf]);
			cub->box.xform[xf] = NULL;
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
	while (i < cub->box.nb_objx && cub->box.objx[i])
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
	if (cub->box.xform)
	{
		xform_flush(cub);
		free(cub->box.xform);
		cub->box.xform = NULL;
		ft_free_p((void **)&cub->box.xform);
	}
	if (cub->dual)
		dual_flush(cub);
	if (cub->box.objx)
		objx_flush(cub);
	ft_free_p((void **)&cub->pset);
	strtab_clear(&cub->map.raw);
	strtab_clear(&cub->map.m);
	strtab_clear((char ***)&cub->map.mx);
	free(cub->box.chrs);
	printf("CIAO!\n");
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
