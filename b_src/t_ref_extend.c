/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_extend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 23:18:50 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/12 17:17:25 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	clr_legend_strct(t_cub *cub)
{
	printf(":: MID EXIT :: \n\n\n");
	while (cub->box.xform[cub->box.xnum])
	{
		if (cub->box.xform[cub->box.xnum])
		{
			mlx_delete_texture(cub->box.xform[cub->box.xnum]);
		}
		free(cub->box.xform[cub->box.xnum]);
		cub->box.xnum--;
	}
	if (*cub->box.xform)
	{
		free(*cub->box.xform);
		printf(":: MID EXIT ::XFORM ::xnum[%d]:: \n", cub->box.xnum);
	}
	if (cub->pset[0].xwalls[0])
		printf(":: MID EXIT :PSET[0]:: xwalls[%p]:: \n",
			cub->pset[0].xwalls[0]);
	if (cub->dual[0].xwalls[0])
		printf(":: MID EXIT ::DUAL[0] ::xwalls[%p]:: \n",
			cub->dual[0].xwalls[0]);
	// if (cub->objs.instances->gset[0])
	// 	printf(":: MID EXIT ::OBJS ::instance gset[%p]::\n",
	// 		cub.objs.instances->gset[0]);
	// strtab_clear(&map->raw);
	// strtab_clear(&map->m);
}

void	p_list_objx(t_objx **objx, int id, int num)
{
	if (num == 0)
		num = id + 1;
	while (id < num)
	{
		printf("LIST_META_obj_id[%d]::NAME:{%c} :: type_[%d] >>>",
			objx[id]->obj_id, objx[id]->name, objx[id]->o_type);
		printf(" RLTV_[%c] < {A:%d}_", objx[id]->relativ, objx[id]->alleg);
		printf("( x[%d], y[%d] )\n", objx[id]->opos[0], objx[id]->opos[1]);
		id++;
	}
}

t_cub	*mapx_alt_pos(t_map *m, t_cub *cub, int p_box)
{
	m->pos_y = -1;
	while (++m->pos_y < m->height)
	{
		m->pos_x = -1;
		while (++m->pos_x < m->width)
		{
			p_box = ft_in_set((m->m[m->pos_y][m->pos_x]), MAP_MCHR);
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
