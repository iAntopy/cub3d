/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghebert <ghebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/20 08:54:55 by ghebert          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "cub3d.h"

t_objx	*init_objx(t_cub *cub, int o_cells, int id)
{
	t_objx	*objx;
	char	*idx;

	objx = (t_objx *)calloc(sizeof(t_objx), 1);
	idx = ft_substr(cub->box.chrs, o_cells, 1);
	objx->obj_id = id;
	objx->name = idx[0];
	free(idx);
	return (objx);
}

int	get_types(t_objx *objx, int head)
{
	if (head != -1)
	{
		if (head < 3)
			objx->o_type = OBJ_LEVER;
		else if (head < 11)
			objx->o_type = OBJ_PORTAL;
		else if (head < 15)
			objx->o_type = OBJ_FIREPIT;
		else if (head < 17)
			objx->o_type = OBJ_FIREBALL;
		else if (head < 21)
			objx->o_type = OBJ_PLAYER;
		else
			objx->o_type = OBJ_FLAG;
	}
	return (objx->o_type);
}

/// get_pos , not get_pos,and adress, and pedigree ... to be sub_div...
t_objx	*get_pos(t_cub *cub, t_map *m, int o_cells, int id)
{
	int		head;
	t_objx	*objx;

	objx = init_objx(cub, o_cells, id);
	head = (ft_in_set(objx->name, (const char *)MCHR));
	if (head != -1)
	{
		objx->o_type = get_types(objx, head);
		objx->opos[0] = m->pos_x;
		objx->opos[1] = m->pos_y;
		objx->relativ = m->raw[o_cells][4];
		objx->alleg = m->raw[o_cells][2] - 48;
		if (objx->o_type == OBJ_PLAYER)
			objx->alleg = ALI_TORRENT;
	}
	if (m->pos_x <= 0 || m->pos_y <= 0)
	{
		report_err("No META char found in map.");
		return (objx);
	}
	return (objx);
}

t_map	*check_hero_found(t_map *m)
{
	if (m->pos_x <= 0 || m->pos_y <= 0)
	{
		report_err("No player charater found in map.");
		m->flg_chk = 1;
		return (m);
	}
	m->flg_chk = 2;
	m->hero_x = m->pos_x;
	m->hero_y = m->pos_y;
	return (m);
}

t_cub	*wall_check(t_cub *cub, t_map *m, t_objx **objx)
{
	int	o_cells;
	int	id;

	id = -1;
	o_cells = -1;
	m->pos_y = -1;
	while (++m->pos_y < m->height)
	{
		m->pos_x = -1;
		while (++m->pos_x < m->width)
		{
			o_cells = ft_in_set((m->m[m->pos_y][m->pos_x]),
					(const char *)cub->box.chrs);
			if (o_cells == (int_strlen(cub->box.chrs) - 1))
				m = check_hero_found(m);
			if (o_cells < 0 && m->m[m->pos_y][m->pos_x] != '\0')
				m->m[m->pos_y][m->pos_x] = 'A';
			else if ((o_cells < cub->box.meta - 1 && o_cells > -1)
				|| o_cells == (int_strlen(cub->box.chrs) - 1))
				if (++id < cub->box.chrs_len - 1)
					objx[id] = get_pos(cub, m, o_cells, id);
		}
	}
	cub->box.objx = objx;
	return (cub);
}
