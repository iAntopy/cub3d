/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_extend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 23:18:50 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/11 22:12:45 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void 	pset_flush(t_cub *cub)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while(cub->pset[j].xwalls[i] && j < cub->box.pset)
	{
		i = 0;
		if (cub->pset[j].xwalls[i])
		{
			while(i < 4)
			{
				printf(":: FLUSH__:PSET[%d]:: xwalls[%d]:: <<%p>> \n", j, i, cub->pset[j].xwalls[i]);
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

void 	dual_flush(t_cub *cub)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while(cub->dual[j].xwalls[i] && j < cub->box.n_dual)
	{
		i = 0;
		if (cub->dual[j].xwalls[i])
		{
			while(i < 2)
			{
				printf(":: FLUSH__:DUAL[%d]:: xwalls[%d]:: <<%p>> \n", j, i, cub->dual[j].xwalls[i]);
				free(cub->dual[j].xwalls[i]);
				cub->dual[j].xwalls[i] = NULL;
				i++;
			}
		}
		j++;
	}
	printf(":: END__:DUAL:: \n");
	free(cub->dual);
	cub->dual = NULL;	
}

void 	objx_flush(t_cub *cub)
{	
	if (cub->box.objx)
		printf(":: INTO__:OBJX:: \n");
	int i;

	i = 0;
	while(cub->box.objx[i] && i < cub->box.meta)
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

void 	instance_killer(t_cub *cub)
	{
		int i;
		int j;

		i = 0;
		j = 0;
		printf(":: PRE-FREE__ ::OBJS [%d]::instance gset[%p]:: \n", i,
			&cub->objs.instances->gset[i]);
		while(&cub->objs.instances->gset[i] && i < cub->box.meta)
		{
			j = 0;
			while (cub->objs.instances->gset[i].xwalls[j])
			{
				printf(":: POST-FREE__ ::OBJS [%d]::instance gset[%d].xwalls[%p]::\n", i, j,
					&cub->objs.instances->gset[i].xwalls[j]);
				if (cub->objs.instances->gset[i].xwalls[j])
					cub->objs.instances->gset[i].xwalls[j] = NULL;
				j++;
			}
			// free(*cub->objs.instances->gset[i].xwalls);
			*cub->objs.instances->gset[i].xwalls = NULL;
			// free(&cub->objs.instances->gset[i]);
			// *cub->objs.instances->gset[i] = NULL;
			i++;
		}
		// free(cub->objs.instances->gset);
		cub->objs.instances->gset = NULL;
}

// mapx = ***ptr_pos, **line_len, *nb_line
	// void	clr_mapx(t_cub *cub)
	// {
	// 	int i;
	// 	int j;

	// 	j = -1;
	// 	while (j++ < cub->map.height)
	// 	{
	// 		i = -1;
	// 		while (i++ < cub->map.width)// && cub->map.mx[j][i])
	// 		{
	// 			if (&cub->map.mx[j][i])
	// 			{
	// 				printf("MapX META X (y:%d, x:%d)>>  ptr:%p\n", j, i,  &cub->map.mx[j][i]);
	// 				free(cub->map.mx[j][i]);
	// 				cub->map.mx[j][i] = NULL;
	// 			}
	// 		}
	// 		free(cub->map.mx[j]);
	// 		cub->map.mx[j] = NULL;

	// 	}
	// 	free(cub->map.mx);
	// 	cub->map.mx = NULL;
	// 	printf("MapX META X END!!!\n");
// }

void	clr_legend_strct(t_cub *cub)
{
	printf("\t:: MID EXIT :: \n\n");
	while (&cub->box.xform[cub->box.xnum] && cub->box.xnum-- > 0)
	{
		if (cub->box.xform[cub->box.xnum])
		{
			mlx_delete_texture(cub->box.xform[cub->box.xnum]);
			cub->box.xform[cub->box.xnum] = NULL;
		}
	}
	free(cub->box.xform);
	cub->box.xform = NULL;

	if (cub->pset[0].xwalls[0])
		pset_flush(cub);
	if (cub->dual[0].xwalls[0])
		dual_flush(cub);
	if (*cub->box.objx)
		objx_flush(cub);
	// if (&cub->objs.instances->gset[0])
		// 	instance_killer(cub);
	// clr_mapx(cub);
	// *strtab_clear(cub->map.mx);
	strtab_clear(&cub->map.raw);
	strtab_clear(&cub->map.m);
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