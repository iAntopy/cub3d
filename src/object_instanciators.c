/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_instanciators.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 20:45:55 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 21:18:29 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// pos is in world coord, NOT cell coord.
int	create_player_instance(t_cub *cub, float *pos, int allegiance,\
 t_hero *link)
{
	t_oinst	*new_obj;

	if (!link)
		return (report_err("Creating a player instance must provide ptr to t_hero struct.\n"));
	new_obj = NULL;
	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	
	printf("create_player_instance : Creating player at (%f, %f)\n",
		pos[0], pos[1]);
	new_obj->type = &cub->objs.player;
	new_obj->_id = get_new_obj_id();
	new_obj->allegiance = allegiance;
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->cx = (int)(pos[0] * cub->inv_cw);
	new_obj->cy = (int)(pos[1] * cub->inv_cw);
	new_obj->action = __obj_action_player;
	new_obj->rel_type_enum = 0;
	new_obj->relative = link;
	new_obj->isactive = 1;
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single Player instance created at pos (%f, %f)\n", pos[0], pos[1]);
	return (new_obj->_id);	
}

int	create_lever_instance(t_cub *cub, float *pos, int allegiance, t_oinst *link)
{
	t_oinst	*new_obj;
	int		cell[2];

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	cell[0] = (int)(pos[0] * cub->inv_cw);
	cell[1] = (int)(pos[1] * cub->inv_cw);
	new_obj->type = &cub->objs.lever;
	printf("Crreating lever instance. new_obj->type->type_enum = %d vs OBJ_LEVER (%d)\n", new_obj->type->type_enum,
		OBJ_LEVER);
	new_obj->_id = get_new_obj_id();
	new_obj->tex_idx = 0;
	new_obj->allegiance = allegiance;
	new_obj->cx = cell[0];
	new_obj->cy = cell[1];
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->relative = link;
	new_obj->isactive = 0;	
	new_obj->action = __obj_action_lever;
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	if (cub->map.mx[cell[1]][cell[0]])
		new_obj->special_gset.xwalls[1] = cub->map.mx[cell[1]][cell[0]]->xwalls[1];
	else
		new_obj->special_gset.xwalls[1] = cub->dual[0].xwalls[1];
	new_obj->special_gset.xwalls[0] = new_obj->type->gset->xwalls[0];
	cub->map.mx[cell[1]][cell[0]] = &new_obj->special_gset;
	return (new_obj->_id);
}

// pos is in world coord, NOT cell coord.
int	create_portal_instance(t_cub *cub, float *pos, int allegiance, t_oinst *link)
{
	t_oinst	*new_obj;

	if (link && link->type->type_enum != OBJ_PORTAL)
		return (report_err("Creating a portal instance trying to link to non portal obj.\n"));
	new_obj = NULL;
	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	
	printf("create_portal_instance : Creating portal at (%f, %f) with link : %p\n",
		pos[0], pos[1], link);
	new_obj->type = &cub->objs.portal;
	new_obj->_id = get_new_obj_id();
	new_obj->allegiance = allegiance;
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->action = __obj_action_portal;
	new_obj->rel_type_enum = 0;
	new_obj->relative = new_obj;
	new_obj->isactive = 0;
	if (link && link->type->type_enum == OBJ_PORTAL)
	{
		printf("linking portal %d (%p) to portal %d (%p)\n", new_obj->_id, new_obj, link->_id, link);
		new_obj->rel_type_enum = OBJ_PORTAL;
		new_obj->relative = link;
		link->relative = new_obj;
	}
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single Portal instance created at pos (%f, %f)\n", pos[0], pos[1]);
	return (new_obj->_id);	
}

// pos is in world coord, NOT cell coord.
int	create_fireball_instance(t_cub *cub, float *pos, int allegiance, t_hero *link)
{
	t_oinst	*new_obj;

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());

	new_obj->type = &cub->objs.fireball;
	new_obj->_id = get_new_obj_id();
	new_obj->allegiance = allegiance;
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->dx = pos[2];
	new_obj->dy = pos[3];
	new_obj->relative = NULL;
	new_obj->action = __obj_action_fireball;
	if (link)
	{
//		printf("linking fireball %d (%p) to portal %d (%p)\n", new_obj->_id, new_obj, link->_id, link);
		new_obj->relative = link;
		new_obj->isactive = 1;
	}
	else
		new_obj->isactive = 0;
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single fireball instance created at pos (%f, %f)\n", pos[0], pos[1]);
	return (new_obj->_id);	
}

//pos is in world coord, NOT cell coord.
int	create_firepit_instance(t_cub *cub, float *pos, int allegiance, t_hero *link)
{
	t_oinst	*new_obj;

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	new_obj->type = &cub->objs.firepit;
	new_obj->_id = get_new_obj_id();
	new_obj->allegiance = allegiance;
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->relative = NULL;
	new_obj->isactive = 0;
	new_obj->action = __obj_action_firepit;
	if (link)
	{
//		printf("linking firepit %d (%p) to portal %d (%p)\n", new_obj->_id, new_obj, link->_id, link);
		new_obj->relative = link;
		new_obj->isactive = 1;
	}
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single firepit instance created at pos (%f, %f)\n", pos[0], pos[1]);
	return (new_obj->_id);	
}