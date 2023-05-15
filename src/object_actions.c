/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:25:58 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/14 22:59:32 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define PORTAL_TRIGGER_DIST 10.0f


int	__obj_action_portal(t_oinst *obj, t_cub *cub)
{
	float	dx;
	float	dy;
	float	dist;
	t_oinst	*link;
	
	if (!obj->isactive || !obj->relative)
		return (-1);
	link = obj->relative;
	dx = obj->px - cub->hero.px;
	dy = obj->py - cub->hero.py;
	dist = sqrtf(dx * dx + dy * dy);
	if (dist < PORTAL_TRIGGER_DIST)
	{
		cub->hero.px = link->px + dx * 1.5f;
		cub->hero.py = link->py + dy * 1.5f;
		return (0);
	}
	else
		return (-1);
}

int	__obj_action_fireball(t_oinst *obj, t_cub *cub)
{
	float	dist;
	t_hero	*player;
	t_oinst	*objs;
	float	delta_prtl[2];
	
	if (!obj->isactive)
		return (-1);
	if (obj->relative)
	{
		player = obj->relative;

		obj->dx = player->px - obj->px;
		obj->dy = player->py - obj->py;
		dist = sqrtf(obj->dx * obj->dx + obj->dy * obj->dy);
		if (dist < 10)
		{
			ft_printf("OUCH MAUDIT CA FAIT MAL!\n");
			delete_oinst_by_id(cub, obj->_id);
			return (0);
		}
		obj->dx /= dist;
		obj->dy /= dist;
		obj->px += obj->dx * obj->type->speed;
		obj->py += obj->dy * obj->type->speed;
		if (is_wall(&cub->map, obj->px * cub->inv_cw, obj->py * cub->inv_cw))
			delete_oinst_by_id(cub, obj->_id);
	}
	else
	{
		obj->px += obj->dx * obj->type->speed;
		obj->py += obj->dy * obj->type->speed;
	}
	objs = cub->objs.instances;
	while (objs)
	{
		if (objs->type->type_enum == OBJ_PORTAL && objs->isactive)
		{
			delta_prtl[0] = objs->px - obj->px;
			delta_prtl[1] = objs->py - obj->py;
			if (sqrtf(delta_prtl[0] * delta_prtl[0] + delta_prtl[1] * delta_prtl[1]) < PORTAL_TRIGGER_DIST)
			{
				obj->px = ((t_oinst *)objs->relative)->px + delta_prtl[0] * 1.5f;
				obj->py = ((t_oinst *)objs->relative)->py + delta_prtl[1] * 1.5f;
				break ;
			}
		}
		objs = objs->next;
	}
	return (0);
}

int	__obj_action_firepit(t_oinst *obj, t_cub *cub)
{
	static int	counter;
	int			pos[4];
	
	if (!obj->isactive)
		return (-1);
	if (obj->relative)
	{
//		printf("delta_time : %zd\n", delta_time);
		if (++counter > FIREPIT_SPAWN_TICKS)
		{
			pos[0] = obj->px;
			pos[1] = obj->py;
			pos[2] = 0;
			pos[3] = 0;
//			printf("SPAWNING FIREBALL\n");
			create_obj_instance(cub, pos, OBJ_FIREBALL, ALI_NEUTRAL, obj->relative);
			counter = 0;
		}
	}

	return (0);
}

int	__obj_action_lever(t_oinst *obj, t_cub *cub)
{
	t_oinst *link;
	int		cx;
	int		cy;

	if (obj->isactive)
		return (-1);
	if (obj->relative)
	{
		cx = (int)obj->px;
		cy = (int)obj->py;
		if (!(cub->hero.cell_x == cx
			&& cub->hero.cell_y == cy))
			return (-1);
		link = (t_oinst *)obj->relative;
		activate_portal(link, 1);
		obj->isactive = 1;
		obj->special_gset.xwalls[0] = obj->type->gset->xwalls[1];
//		dual = cub->map.mx[cy][cx];
//		dual->xwalls[0] = obj->type->gset->xwalls[1];
	}
	return (0);
}

// Attempts to commit an action with every object currently in world.
// Every obj inst is responsible for verifying if its action is possible and valid.
void    commit_all_obj_actions(t_cub *cub)
{
	t_oinst *obj;

	obj = cub->objs.instances;
	while (obj)
	{
		if (obj->action)
			obj->action(obj, cub);

		obj = obj->next;
	}
}
