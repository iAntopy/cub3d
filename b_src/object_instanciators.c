/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_instanciators.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 20:45:55 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/12 15:34:59 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// pos is in world coord, NOT cell coord.
int	create_player_instance(t_cub *cub, float *pos, int alleg, t_oinst *spawnp)
{
	t_oinst	*new_obj;

	if (!spawnp)
		return (report_err("create_player must provide spawnpoint obj ptr.\n"));
	if (!cub->objs.player.type_enum)
		init_player_model(&cub->objs);
	new_obj = NULL;
	if (!ft_calloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	new_obj->type = &cub->objs.player;
	new_obj->_id = get_new_obj_id();
	new_obj->alleg = alleg;
	new_obj->spawnpoint = spawnp;
	obj_set_position(cub, new_obj, pos[0], pos[1]);
	obj_set_orientation(cub, new_obj, spawnp->ori);
	new_obj->action = __obj_action_player;
	new_obj->relative = spawnp;
	new_obj->gset = new_obj->type->gsets[new_obj->alleg];
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	return (new_obj->_id);
}

// pos is in world coord, NOT cell coord.
int	create_spawnp_instance(t_cub *cub, float *pos, int alleg)
{
	t_oinst	*new_obj;

	if (!cub->objs.spawnp.type_enum)
		init_spawnpoint_model(&cub->objs);
	new_obj = NULL;
	if (!ft_calloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	new_obj->type = &cub->objs.spawnp;
	new_obj->_id = get_new_obj_id();
	new_obj->alleg = alleg;
	obj_set_position(cub, new_obj, pos[0], pos[1]);
	new_obj->action = __obj_action_spawnpoint;
	new_obj->relative = cub;
	new_obj->isactive = 1;
	new_obj->gset = NULL;
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	return (new_obj->_id);
}

int	create_lever_instance(t_cub *cub, float *pos, int alleg, t_oinst *link)
{
	t_oinst	*new_obj;
	int		cl[2];

	if (!ft_calloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	new_obj->type = &cub->objs.lever;
	new_obj->_id = get_new_obj_id();
	new_obj->alleg = alleg;
	obj_set_position(cub, new_obj, pos[0], pos[1]);
	cl[0] = new_obj->cx;
	cl[1] = new_obj->cy;
	new_obj->relative = link;
	new_obj->action = __obj_action_lever;
	new_obj->gset = new_obj->type->gsets[0];
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	if (cub->map.mx[cl[1]][cl[0]])
		new_obj->special_gset.xwalls[1] = cub->map.mx[cl[1]][cl[0]]->xwalls[1];
	else
		new_obj->special_gset.xwalls[1] = cub->dual[0].xwalls[1];
	new_obj->special_gset.xwalls[0] = new_obj->type->gsets[0]->xwalls[0];
	cub->map.mx[cl[1]][cl[0]] = &new_obj->special_gset;
	return (new_obj->_id);
}

// pos is in world coord, NOT cell coord.
int	create_portal_instance(t_cub *cub, float *pos, int alleg, t_oinst *link)
{
	t_oinst	*new_obj;

	if (link && obj_get_type(link) != OBJ_PORTAL)
		return (report_err("create_portal trying to link to non portal.\n"));
	new_obj = NULL;
	if (!ft_calloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());
	new_obj->type = &cub->objs.portal;
	new_obj->_id = get_new_obj_id();
	new_obj->alleg = alleg;
	obj_set_position(cub, new_obj, pos[0], pos[1]);
	new_obj->action = __obj_action_portal;
	new_obj->relative = new_obj;
	if (link)
		link_portal_instances(new_obj, link);
	new_obj->gset = new_obj->type->gsets[0];
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	return (new_obj->_id);
}
