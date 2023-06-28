/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_actions3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 16:08:21 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/27 18:54:39 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define PORTAL_TRIGGER_DIST_SQ 400.0f

static int	__teleport_check(t_cub *cub, t_oinst *obj, t_oinst *oth, float *dt)
{
	float	dist;

	find_vector_delta(&obj->px, &oth->px, dt);
	dist = dt[0] * dt[0] + dt[1] * dt[1];
	if (oth->isteleportable && dist < PORTAL_TRIGGER_DIST_SQ)
	{
		oth->isteleportable = 0;
		oth->last_teleport_prtl = obj->relative;
		obj_set_position(cub, oth, ((t_oinst *)obj->relative)->px,
			((t_oinst *)obj->relative)->py);
		return (1);
	}
	else if (oth->last_teleport_prtl == obj && dist > PORTAL_TRIGGER_DIST_SQ)
	{
		oth->isteleportable = 1;
		oth->last_teleport_prtl = NULL;
	}
	return (0);
}

int	__obj_action_portal(t_oinst *obj, t_cub *cub)
{
	float	dt[2];
	t_oinst	*other;

	if (!obj->isactive || !obj->relative)
		return (-1);
	other = cub->objs.instances;
	while (other)
	{
		if (!(obj_get_type(other) == OBJ_PLAYER
				|| obj_get_type(other) == OBJ_FIREBALL
				|| obj_get_type(other) == OBJ_FIREPIT) && next_obj(&other))
			continue ;
		__teleport_check(cub, obj, other, dt);
		other = other->next;
	}
	return (0);
}
