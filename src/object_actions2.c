/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_actions2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 16:08:21 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/06 16:43:28 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define PORTAL_TRIGGER_DIST_SQ 400.0f
#define NPC_TWICH_TIMER 100

// target is resulting target coord as vect2
static void	find_proper_npc_dest(t_cub *cub, t_oinst *npc)
{
	int	is_in_wall;
	int	cell[2];

	is_in_wall = 1;
	while (is_in_wall)
	{
		npc->ori = ft_random() * M_TAU;
		npc->target[0] = npc->px + cosf(npc->ori) * NPC_TWICH_TIMER;
		npc->target[1] = npc->py + sinf(npc->ori) * NPC_TWICH_TIMER;
		get_cell(npc->target[0], npc->target[1], cell, cell + 1);
		is_in_wall = is_wall(&cub->map, cell[0], cell[1]);
	}
}

int	__obj_action_player(t_oinst *obj, t_cub *cub)
{
	if (obj == cub->hero.ply_obj)
		return (-1);
	if (obj->isactive && obj->counter > NPC_TWICH_TIMER)
	{
		obj->isactive = 0;
		obj->counter = 0;
	}
	else if (!obj->isactive && obj->counter > NPC_TWICH_TIMER)
	{
		find_proper_npc_dest(cub, obj);
		obj->isactive = 1;
		obj->counter = 0;
	}
	else if (obj->isactive)
		obj_move_rel(cub, obj, 1.0f, 0.0f);
	++obj->counter;
	return (0);
}

int	__obj_action_portal(t_oinst *obj, t_cub *cub)
{
	float	dt[2];
	float	dist;
	t_oinst	*other;

	if (!obj->isactive || !obj->relative)
		return (-1);
	if (obj->counter > 0)
		return (obj->counter--);
	other = cub->objs.instances;
	while (other)
	{
		if (!(obj_get_type(other) == OBJ_PLAYER
				|| obj_get_type(other) == OBJ_FIREBALL) && next_obj(&other))
			continue ;
		find_vector_delta(&obj->px, &other->px, dt);
		dist = dt[0] * dt[0] + dt[1] * dt[1];
		if (dist < PORTAL_TRIGGER_DIST_SQ)
		{
			obj_set_position(cub, other, ((t_oinst *)obj->relative)->px,
				((t_oinst *)obj->relative)->py);
			((t_oinst *)obj->relative)->counter = 30;
			break ;
		}
		other = other->next;
	}
	return (0);
}

static void	__obj_lever_check_if_pressed(t_oinst *obj, t_cub *cub)
{
	t_oinst	*prtl;
	t_oinst	*other;

	prtl = (t_oinst *)obj->relative;
	other = cub->objs.instances;
	while (other)
	{
		if (obj_get_type(other) == OBJ_PLAYER
			&& other->cx == obj->cx && other->cy == obj->cy)
		{
			activate_portal(prtl, 1);
			obj->isactive = 1;
			obj->special_gset.xwalls[0] = obj->gset->xwalls[1];
			break ;
		}
		other = other->next;
	}
}

int	__obj_action_lever(t_oinst *obj, t_cub *cub)
{
	t_oinst	*prtl;

	if (obj->isactive)
	{
		prtl = (t_oinst *)obj->relative;
		if (!prtl)
			return (-1);
		if (obj->counter > 600)
		{
			activate_portal((t_oinst *)obj->relative, 0);
			obj->isactive = 0;
			obj->special_gset.xwalls[0] = obj->gset->xwalls[0];
			obj->counter = 0;
		}
		++obj->counter;
	}
	if (obj->relative)
		__obj_lever_check_if_pressed(obj, cub);
	return (0);
}
