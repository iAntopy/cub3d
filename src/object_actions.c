/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:25:58 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/29 21:54:56 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

#define PORTAL_TRIGGER_DIST_SQ 100.0f

int	__obj_action_player(t_oinst *obj, t_cub *cub)
{
//	static int	counter;
//	t_hero		*player;
//	float		pos[4];
	(void)obj;
	(void)cub;
//	if (obj->isactive)
//	{
		// if (counter > 1000)
		// {
		// 	player = (t_hero *)obj->relative;
		// 	pos[0] = obj->px;
		// 	pos[1] = obj->py;
		// 	pos[2] = (*player->dirx) * 10.0f ;
		// 	pos[3] = (*player->diry) * 10.0f ;
		// 	create_obj_instance(cub, pos, OBJ_FIREBALL,
		// 		player->ply_obj->allegiance, NULL);
		// 	counter = 0;
		// }
		// ++counter;
//	}
	return (0);
}

int	__obj_action_spawnpoint(t_oinst *obj, t_cub *cub)
{
//	t_oinst		*player;
//	int			i;

	if (!obj->isactive)
		return (-1);
	(void)cub;
//	printf("nb players : %d\n", cub->nb_players);

	/// This updates the players spawn point to the one placed in its current
	/// cell if spawnpoint is active and of same allegience.
	/*
	i = -1;
	while (++i < cub->nb_players)
	{
		player = get_obj(cub, i);
		if (player && player->allegiance == obj->allegiance
			&& player->cx == obj->cx && player->cy == obj->cy)
			player->spawnpoint = obj;
	}
	*/
	return (0);
}

int	__obj_action_portal(t_oinst *obj, t_cub *cub)
{
	float	dx;
	float	dy;
	float	dist;
	t_oinst	*link;
	
	if (!obj->isactive || !obj->relative)
		return (-1);
	link = obj->relative;
	dx = obj->px - cub->hero.ply_obj->px;
	dy = obj->py - cub->hero.ply_obj->py;
	dist = dx * dx + dy * dy;
	if (dist < PORTAL_TRIGGER_DIST_SQ)
	{
		cub->hero.ply_obj->px = link->px + dx * 1.5f;
		cub->hero.ply_obj->py = link->py + dy * 1.5f;
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
		player = (t_hero *)obj->relative;

		obj->dx = player->ply_obj->px - obj->px;
		obj->dy = player->ply_obj->py - obj->py;
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
	return (0);
}

int	__obj_action_firepit(t_oinst *obj, t_cub *cub)
{
	static int	counter;
	float		pos[4];
	
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
//	static int	counter;
//	int		cx;
//	int		cy;

	// if (obj->isactive)
	// {
	// 	if (counter > 400)
	// 	{
	// 		activate_portal((t_oinst *)obj->relative, 0);
	// 		obj->isactive = 0;
	// 		obj->special_gset.xwalls[0] = obj->gset->xwalls[0];
	// 		counter = 0;
	// 	}
	// 	++counter;
	// }
	if (obj->relative)
	{
//		ft_eprintf("lever relative exists\n");
//		cx = (int)obj->px;
//		cy = (int)obj->py;
//		ft_eprintf("lever cx, cy (%d, %d), hero cx, cy (%d, %d)\n", cx, cy, 
//			cub->hero.cell_x, cub->hero.cell_y);
		if (!(cub->hero.ply_obj->cx == obj->cx
			&& cub->hero.ply_obj->cy == obj->cy))
			return (-1);
		ft_eprintf("PRESSED !\n");
		activate_portal((t_oinst *)obj->relative, 1);
		obj->isactive = 1;
		obj->special_gset.xwalls[0] = obj->gset->xwalls[1];
//		dual = cub->map.mx[cy][cx];
//		dual->xwalls[0] = obj->type->gset->xwalls[1];
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
