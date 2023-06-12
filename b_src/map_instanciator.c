/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_instanciator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 06:25:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/12 15:34:59 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_objx	*find_relative(t_cub *cub, t_objx *ob, int nb_meta)
{
	t_objx	*other;
	int		i;

	if (!ob->relativ)
		return (NULL);
	i = -1;
	while (++i < nb_meta)
	{
		other = cub->box.objx[i];
		if (other && other->name == ob->relativ)
			return (other);
	}
	return (NULL);
}

static int	link_all_map_instances(t_objx **ob, int nb_meta)
{
	t_objx	*o;
	int		i;

	i = -1;
	while (++i < nb_meta)
	{
		o = ob[i];
		if (o->o_type == OBJ_PORTAL && o->rel_ref && o->rel_ref->wobj)
			link_portal_instances(o->wobj, o->rel_ref->wobj);
		else if (o->o_type == OBJ_LEVER && o->rel_ref && o->rel_ref->wobj)
			link_lever_to_portal(o->wobj, o->rel_ref->wobj);
		else if (o->o_type == OBJ_FIREBALL)
		{
			if (o->rel_ref && o->rel_ref->wobj)
				link_fireball_to_target(o->wobj, (t_oinst *)o->rel_ref->wobj);
			activate_fireball(o->wobj, 1, NULL);
		}
		else if (o->o_type == OBJ_FIREPIT)
		{
			if (o->rel_ref && o->rel_ref->wobj)
				link_firepit_to_target(o->wobj, (t_oinst *)o->rel_ref->wobj);
			activate_firepit(o->wobj, 1, NULL);
		}
	}
	return (0);
}

static t_oinst	*__inst_by_type(t_cub *cub, t_objx *ob, float *pos, int nb_meta)
{
	int		inst_id;

	if (ob->o_type == OBJ_PLAYER && cub->nb_players < MAX_PLAYERS)
	{
		inst_id = create_obj_instance(cub, pos,
				obj_type_alleg(OBJ_SPAWNPOINT, ob->alleg), cub);
		inst_id = spawn_new_player(get_obj(cub, inst_id), ob->name == '@');
		obj_set_orientation(cub, get_obj(cub, inst_id), objx_get_side_ori(ob));
	}
	else if (ob->o_type == OBJ_FIREBALL || ob->o_type == OBJ_FIREPIT)
	{
		pos[2] = cosf(objx_get_side_ori(ob));
		pos[3] = sinf(objx_get_side_ori(ob));
		if (ob->relativ == '9')
		{
			ob->relativ = '@';
			ob->rel_ref = find_relative(cub, ob, nb_meta);
		}
		inst_id = create_obj_instance(cub, pos,
				obj_type_alleg(ob->o_type, ob->alleg), NULL);
	}
	else
		inst_id = create_obj_instance(cub, pos,
				obj_type_alleg(ob->o_type, ob->alleg), NULL);
	return (get_obj(cub, inst_id));
}

static t_oinst	*instanciate_specific_obj(t_cub *cub, t_objx *ob, int nb_meta)
{
	float	pos[4];

	if (ob->obj_id < 0 || ob->o_type < 1 || NB_OBJ_TYPES < ob->o_type
		|| ob->alleg < 1 || 3 < ob->alleg)
		return (NULL);
	pos[0] = (float)ob->opos[0] * CELL_WIDTH + (CELL_WIDTH >> 1);
	pos[1] = (float)ob->opos[1] * CELL_WIDTH + (CELL_WIDTH >> 1);
	ob->wobj = __inst_by_type(cub, ob, pos, ob->o_type);
	ob->rel_ref = find_relative(cub, ob, nb_meta);
	return (ob->wobj);
}

int	instanciate_map_objects(t_cub *cub)
{
	int	nb_meta;
	int	i;

	if (!cub || !cub->box.objx)
		return (-1);
	nb_meta = cub->box.meta;
	i = -1;
	while (++i < nb_meta)
	{
		instanciate_specific_obj(cub, cub->box.objx[i], nb_meta);
		if (cub->box.objx[i]->name == '@')
			set_playable_obj(cub, cub->box.objx[i]->wobj);
	}
	link_all_map_instances(cub->box.objx, nb_meta);
	return (0);
}
