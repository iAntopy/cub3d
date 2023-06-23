/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:25:58 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/19 19:13:03 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/// This updates the players spawn point to the one placed in its current
/// cell if spawnpoint is active and of same allegience.
int	__obj_action_spawnpoint(t_oinst *obj, t_cub *cub)
{
	t_oinst		*other;

	if (!obj->isactive)
		return (-1);
	other = cub->objs.instances;
	while (other)
	{
		if (obj_get_type(other) == OBJ_PLAYER
			&& other->alleg == obj->alleg
			&& other->cx == obj->cx && other->cy == obj->cy)
		{
			other->spawnpoint = obj;
			break ;
		}
		other = other->next;
	}
	return (0);
}

int	__fball_hit_target(t_oinst *obj, t_oinst *other, t_oinst **tgt)
{
	static float	closest_dist;
	float			delta[3];

	if (!(*tgt))
		closest_dist = INFINITY;
	find_vector_delta(&obj->px, &other->px, delta);
	delta[2] = delta[0] * delta[0] + delta[1] * delta[1];
	if (delta[2] < 300.0f)
	{
		if (obj_get_type(other) == OBJ_PLAYER)
			respawn_player(other);
		else if (obj_get_type(other) == OBJ_FIREBALL)
			obj_schedule_for_deletion(other);
		return (obj_schedule_for_deletion(obj));
	}
	else if (obj_get_type(other) == OBJ_PLAYER
		&& obj->relative && delta[2] < closest_dist)
	{
		closest_dist = delta[2];
		obj->relative = other;
		*tgt = other;
	}
	return (0);
}

int	__obj_action_fireball(t_oinst *obj, t_cub *cub)
{
	t_oinst	*other;
	t_oinst	*target;

	if (!obj->isactive)
		return (-1);
	if (is_wall(&cub->map, (obj->px + obj->dx * 10.0f) * cub->inv_cw,
			(obj->py + obj->dy * 10.0f) * cub->inv_cw))
		return (obj_schedule_for_deletion(obj));
	other = cub->objs.instances;
	target = NULL;
	while (other)
	{
		if ((!(obj_get_type(other) == OBJ_PLAYER
					|| obj_get_type(other) == OBJ_FIREBALL)
				|| other->alleg == obj->alleg) && next_obj(&other))
			continue ;
		if (__fball_hit_target(obj, other, &target))
			return (1);
		other = other->next;
	}
	if (obj->relative)
		obj_look_at(obj, obj->relative);
	obj_move_abs(cub, obj, obj->dx, obj->dy);
	return (0);
}

int	__obj_action_firepit(t_oinst *obj, t_cub *cub)
{
	float		pos[4];
	int			inst_id;

	if (!obj->isactive)
		return (-1);
	if (++obj->counter > FIREPIT_SPAWN_TICKS)
	{
		pos[0] = obj->px;
		pos[1] = obj->py;
		pos[2] = obj->dx;
		pos[3] = obj->dy;
		inst_id = create_obj_instance(cub, pos,
				obj_type_alleg(OBJ_FIREBALL, obj->alleg), obj->relative);
		activate_fireball(get_obj(cub, inst_id), 1, NULL);
		obj->counter = 0;
	}
	return (0);
}

// Attempts to commit an action with every object currently in world.
// Every obj is responsible for verifying if its action is possible and valid.
void	commit_all_obj_actions(t_cub *cub)
{
	t_oinst	*obj;

	obj = cub->objs.instances;
	while (obj)
	{
		if (obj->action)
			obj->action(obj, cub);
		obj = obj->next;
	}
	delete_all_scheduled_for_deletion(cub);
}
