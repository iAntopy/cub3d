/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_instanciators2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 19:13:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/27 15:28:48 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// pos is in world coord, NOT cell coord.
int	create_fireball_instance(t_cub *cub, float *pos, int alleg, t_oinst *link)
{
	t_oinst	*new_obj;

	if (!cub->objs.fireball.type_enum)
		init_fireball_model(&cub->objs);
	if (!ft_calloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	new_obj->type = &cub->objs.fireball;
	new_obj->_id = get_new_obj_id();
	new_obj->alleg = alleg;
	new_obj->tex_idx = alleg;
	obj_set_position(cub, new_obj, pos[0], pos[1]);
	new_obj->dx = pos[2];
	new_obj->dy = pos[3];
	new_obj->relative = NULL;
	new_obj->action = __obj_action_fireball;
	if (link)
	{
		new_obj->relative = link;
		new_obj->isactive = 1;
	}
	new_obj->gset = new_obj->type->gsets[0];
	new_obj->next = cub->objs.instances;
	new_obj->isteleportable = 1;
	cub->objs.instances = new_obj;
	return (new_obj->_id);
}

//pos is in world coord, NOT cell coord.
int	create_firepit_instance(t_cub *cub, float *pos, int alleg, t_oinst *link)
{
	t_oinst	*new_obj;

	if (!cub->objs.firepit.type_enum)
		init_firepit_model(&cub->objs);
	if (!ft_calloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	new_obj->type = &cub->objs.firepit;
	new_obj->_id = get_new_obj_id();
	new_obj->alleg = alleg;
	new_obj->tex_idx = alleg;
	obj_set_position(cub, new_obj, pos[0], pos[1]);
	new_obj->dx = pos[2];
	new_obj->dy = pos[3];
	new_obj->relative = NULL;
	new_obj->action = __obj_action_firepit;
	if (link)
	{
		new_obj->relative = link;
		new_obj->isactive = 1;
	}
	new_obj->gset = new_obj->type->gsets[0];
	new_obj->next = cub->objs.instances;
	new_obj->isteleportable = 1;
	cub->objs.instances = new_obj;
	return (new_obj->_id);
}
