/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_activators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 22:11:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/23 16:40:07 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Pass either OBJ_ACTIVATE or OBJ_DEACTIVATE as mode.
int	activate_portal(t_oinst *obj, unsigned int new_state)
{
	mlx_texture_t	*tex;
	t_oinst			*link;

	if (obj_get_type(obj) != OBJ_PORTAL)
		return (report_obj_err(obj, "ACTIVATION FAILED : not a portal obj.\n"));
	else if (new_state >= 2)
		return (report_obj_err(obj, "ACTIVATION FAILED : invalid state.\n"));
	link = (t_oinst *)obj->relative;
	if (obj->isactive && new_state == 0)
	{
		obj->isactive = 0;
		obj->tex_idx = 0;
		activate_portal(link, 0);
	}
	else if (!obj->isactive && new_state == 1)
	{
		if (!link)
			return (report_err("Cannot activate portal without a link"), -1);
		obj->isactive = 1;
		obj->tex_idx = obj->alleg;
		activate_portal(link, 1);
	}
	tex = obj->gset->xwalls[obj->tex_idx];
	obj->type->height = obj_get_width(obj) * (tex->height / tex->width);
	return (-obj_get_isactive(obj));
}

int	activate_fireball(t_oinst *obj, int new_state, t_oinst *target)
{
	if (obj_get_type(obj) != OBJ_FIREBALL)
		return (report_err("Try activate fireball but is not fireball."));
	if (new_state && target && obj_get_type(target) == OBJ_PLAYER)
		target->relative = target;
	obj->isactive = new_state;
	return (0);
}

int	activate_firepit(t_oinst *obj, int new_state, t_oinst *target)
{
	if (obj_get_type(obj) != OBJ_FIREPIT)
		return (report_err("Try activate firepit but is not firepit."));
	if (new_state && target && obj_get_type(target) == OBJ_PLAYER)
		target->relative = target;
	obj->isactive = new_state;
	return (0);
}
