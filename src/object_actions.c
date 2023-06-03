/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:25:58 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/02 23:52:05 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

#define PORTAL_TRIGGER_DIST_SQ 400.0f

int	__obj_action_player(t_oinst *obj, t_cub *cub)
{
	int		is_in_wall;

	if (obj == cub->hero.ply_obj)
		return (-1);
	if (obj->isactive && obj->counter > 50)
	{
		obj->isactive = 0;
		obj->counter = 0;
	}
	else if (!obj->isactive && obj->counter > 50)
	{
		is_in_wall = 1;
		while (is_in_wall)
		{
			obj->ori = ft_random() * M_TAU;
			obj->target[0] = obj->px + cosf(obj->ori) * 100.0f;
			obj->target[1] = obj->py + sinf(obj->ori) * 100.0f;
			is_in_wall = is_wall(&cub->map, (int)(obj->target[0] * cub->inv_cw),
				(int)(obj->target[1] * cub->inv_cw));
		}
		obj->isactive = 1;
		obj->counter = 0;
	}
	else if (obj->isactive)
		obj_move_rel(cub, obj, 2.0f, 0.0f);
	++obj->counter;
	return (0);
}

int	__obj_action_spawnpoint(t_oinst *obj, t_cub *cub)
{
	t_oinst		*other;
//	int			i;

	if (!obj->isactive)
		return (-1);
//	printf("nb players : %d\n", cub->nb_players);
	/// This updates the players spawn point to the one placed in its current
	/// cell if spawnpoint is active and of same allegience.
	other = cub->objs.instances;
	while (other)
	{
		if (other->type->type_enum == OBJ_PLAYER
//			&& other->allegiance == obj->allegiance
			&& other->cx == obj->cx && other->cy == obj->cy)
		{
			printf("WOWOW !!!! player %d changed spawn point !\n\n", other->_id);
			other->spawnpoint = obj;
			break ;
		}
		other = other->next;
	}
	return (0);
}

int	__obj_action_portal(t_oinst *obj, t_cub *cub)
{
	float	dx;
	float	dy;
	float	dist;
	t_oinst	*link;
	t_oinst	*other;
	
	if (!obj->isactive || !obj->relative)
		return (-1);
	link = obj->relative;

	if (obj->counter > 0)
	{
		obj->counter--;
		return (0);
	}
	other = cub->objs.instances;
	while (other)
	{
		if (!(other->type->type_enum == OBJ_PLAYER
			|| other->type->type_enum == OBJ_FIREBALL))
		{
			other = other->next;
			continue ;
		}
		dx = obj->px - other->px;
		dy = obj->py - other->py;
		dist = dx * dx + dy * dy;
		if (dist < PORTAL_TRIGGER_DIST_SQ)
		{
			obj_set_position(cub, other, link->px,link->py);
			link->counter = 30;
			break ;
		}
		other = other->next;
	}
	return (0);
}

int	__fireball_check_hit(t_cub *cub, t_oinst *obj)
{
	float	closest_plyr;
	t_oinst	*other;
	t_oinst	*target;
	float	delta[2];
	float	dist;	

	if (is_wall(&cub->map, (obj->px + obj->dx * 10.0f) * cub->inv_cw,
		(obj->py + obj->dy * 10.0f) * cub->inv_cw))
		return (delete_oinst_by_id(cub, obj->_id));
	other = cub->objs.instances;
	closest_plyr = INFINITY;
	while (other)
	{
		if (other->type->type_enum != OBJ_PLAYER
			|| other->allegiance == obj->allegiance)
		{
			other = other->next;
			continue ;
		}
		delta[0] = other->px - obj->px;
		delta[1] = other->py - obj->py;
		dist = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
//		printf("fireball dist : %f\n", dist);
		if (dist < 20)
		{
			respawn_player(other);
			return (delete_oinst_by_id(cub, obj->_id));
		}
		else if (obj->relative && dist < closest_plyr)
		{
			obj->dx = delta[0] / dist;
			obj->dy = delta[1] / dist;
			closest_plyr = dist;
			target = other;
		}
		other = other->next;
	}
	if (obj->relative && target != obj->relative)
	{
		link_fireball_to_target(obj, target);
//			obj->px += obj->dx * obj->type->speed;
//			obj->py += obj->dy * obj->type->speed;
	}
//	else
//		obj_move_abs(cub, obj, obj->dx, obj->dy);
	return (0);
}

int	__obj_action_fireball(t_oinst *obj, t_cub *cub)
{
//	float	dist;
//	t_oinst	*other;
//	float	delta_prtl[2];
	
//	printf("FIREBAAAAAAAAALLLL\n");
	if (!obj->isactive)
		return (-1);
	if (__fireball_check_hit(cub, obj) == 0)
		obj_move_abs(cub, obj, obj->dx, obj->dy);
/*
	if (obj->relative)
	{
//		player = (t_hero *)obj->relative;
		other = (t_oinst *)obj->relative;

		obj->dx = other->px - obj->px;
		obj->dy = other->py - obj->py;
		dist = sqrtf(obj->dx * obj->dx + obj->dy * obj->dy);
		if (dist < 10)
		{
//			ft_printf("OUCH MAUDIT CA FAIT MAL!\n");
			respawn_player((t_oinst *)obj->relative);
			delete_oinst_by_id(cub, obj->_id);
			return (0);
		}
		obj->dx /= dist;
		obj->dy /= dist;
		obj->px += obj->dx * obj->type->speed;
		obj->py += obj->dy * obj->type->speed;
	}
	else
	{
		other = cub->objs.instances;
		while (other)
//		{
			
//		}
		obj->px += obj->dx * obj->type->speed;
		obj->py += obj->dy * obj->type->speed;
	}
*/
	/*
	objs = cub->objs.instances;
	while (objs)
	{
		if (objs->type->type_enum == OBJ_PORTAL && objs->isactive)
		{
			delta_prtl[0] = objs->px - obj->px;
			delta_prtl[1] = objs->py - obj->py;
			if (delta_prtl[0] * delta_prtl[0] + delta_prtl[1] * delta_prtl[1]
				< PORTAL_TRIGGER_DIST_SQ)
			{
				obj->px = ((t_oinst *)objs->relative)->px + delta_prtl[0] * 1.5f;
				obj->py = ((t_oinst *)objs->relative)->py + delta_prtl[1] * 1.5f;
				break ;
			}
		}
		objs = objs->next;
	}
	*/
	return (0);
}

int	__obj_action_firepit(t_oinst *obj, t_cub *cub)
{
	float		pos[4];
	int			inst_id;
	
	if (!obj->isactive)
		return (-1);
//	printf("firepit active\n");
	if (++obj->counter > FIREPIT_SPAWN_TICKS)
	{
		pos[0] = obj->px;
		pos[1] = obj->py;
		pos[2] = obj->dx;
		pos[3] = obj->dy;
		printf("SPAWNING FIREBALL : pos (%f, %f), dir (%f, %f)\n", pos[0], pos[1], pos[2], pos[3]);
		inst_id = create_obj_instance(cub, pos, OBJ_FIREBALL,
			obj->allegiance, obj->relative);
		activate_fireball(get_obj(cub, inst_id), 1, NULL);
		obj->counter = 0;
	}
	return (0);
}

int	__obj_action_lever(t_oinst *obj, t_cub *cub)
{
	t_oinst	*prtl;
	t_oinst	*other;

//	int		cx;
//	int		cy;

	if (obj->isactive)
	{
		prtl = (t_oinst *)obj->relative;
		if (obj->counter > 300)
		{
			activate_portal((t_oinst *)obj->relative, 0);
			obj->isactive = 0;
			obj->special_gset.xwalls[0] = obj->gset->xwalls[0];
			obj->counter = 0;
		}
		++obj->counter;
	}
	if (obj->relative)
	{
//		ft_eprintf("lever relative exists\n");
//		cx = (int)obj->px;
//		cy = (int)obj->py;
//		ft_eprintf("lever cx, cy (%d, %d), hero cx, cy (%d, %d)\n", cx, cy, 
//			cub->hero.cell_x, cub->hero.cell_y);
		prtl = (t_oinst *)obj->relative;
		other = cub->objs.instances;
		while (other)
		{
			if (other->type->type_enum == OBJ_PLAYER
				&& other->cx == obj->cx && other->cy == obj->cy)
			{
				activate_portal(prtl, 1);
				obj->isactive = 1;
				obj->special_gset.xwalls[0] = obj->gset->xwalls[1];
				break ;
			}
			other = other->next;
		}
/*

		if (!(cub->hero.ply_obj->cx == obj->cx
			&& cub->hero.ply_obj->cy == obj->cy))
			return (-1);
		ft_eprintf("PRESSED !\n");
		activate_portal(prtl, 1);
		obj->isactive = 1;
//		dual = cub->map.mx[cy][cx];
//		dual->xwalls[0] = obj->type->gset->xwalls[1];
*/
	}
//	else
//		printf("Lever has no relative\n");
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
