/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_activators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 22:11:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/02 23:52:47 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// Pass either OBJ_ACTIVATE or OBJ_DEACTIVATE as mode.
int	activate_portal(t_oinst *obj, unsigned int new_state)
{
	mlx_texture_t	*tex;
	t_oinst			*link;

	if (obj->type->type_enum != OBJ_PORTAL)
		return (report_obj_err(obj, "ACTIVATION FAILED : not a portal obj.\n"));
	else if (new_state >= 2)
		return (report_obj_err(obj, "ACTIVATION FAILED : invalid state.\n"));
	link = (t_oinst *)obj->relative;
	if (obj->isactive && new_state == 0)
	{
		printf("DEACTIVATING PORTAL id %d\n", obj->_id);
		obj->isactive = 0;
		obj->tex_idx = 0;
		tex = obj->gset->xwalls[0];
		obj->type->height = obj->type->width * (tex->height / tex->width);
		activate_portal(link, 0);
	}
	else if (!obj->isactive && new_state == 1)
	{
		if (!link)
			return (ft_eprintf("Cannot activate portal without a link\n"), -1);
		printf("ACTIVATING PORTAL id %d, allegiance : %d\n", obj->_id, obj->allegiance);
		obj->isactive = 1;
		obj->tex_idx = obj->allegiance;
		tex = obj->gset->xwalls[obj->allegiance];
		obj->type->height = obj->type->width * (tex->height / tex->width);
		activate_portal(link, 1);
	}
	else
		return (-1);
	return (0);
}

int	activate_fireball(t_oinst *obj, int new_state, t_oinst *target)
{
	if (obj->type->type_enum != OBJ_FIREBALL)
		return (report_err("Trying to activate firepit but obj is not firepit."));
	if (new_state && target && target->type->type_enum == OBJ_PLAYER)
		target->relative = target;
	obj->isactive = new_state;
	return (0);
}

int	activate_firepit(t_oinst *obj, int new_state, t_oinst *target)
{
	if (obj->type->type_enum != OBJ_FIREPIT)
		return (report_err("Trying to activate firepit but obj is not firepit."));
	if (new_state && target && target->type->type_enum == OBJ_PLAYER)
		target->relative = target;
	obj->isactive = new_state;
	return (0);
}

int	set_playable_obj(t_cub *cub, t_oinst *player)
{
	if (!cub || !player)//|| player->type->type_enum != OBJ_PLAYER)
		return (report_err("set_playable_object : missing inputs."));
	cub->hero.ply_obj = player;

	init_raycaster(cub);
	init_rdata_consts(cub, &cub->hero.rcast,
		cub->hero.rcast.rdata, cub->hero.rcast.prtl_proj);
	update_rays(cub);
	cub->renderer.requires_update = 1;
	return (0);
}

int	spawn_new_player(t_oinst *spawnp, int is_playable)
{
	t_cub	*cub;
	t_oinst	*player;
	int		id;
	float		pos[2];

	printf("Trying to spawning new player. spawnp : %p\n", spawnp);
	printf("spawnp->type->type_enum : %d\n", spawnp->type->type_enum);
	if (!spawnp || spawnp->type->type_enum != OBJ_SPAWNPOINT)
		return (-1);
	printf("Spawning new player\n");
	cub = (t_cub *)spawnp->relative;
	if (cub->nb_players >= MAX_PLAYERS)
		return (-1);
	printf("player type_enum : %d\n", cub->objs.player.type_enum);
	pos[0] = spawnp->px;
	pos[1] = spawnp->py;
	id = create_player_instance(cub, pos, spawnp->allegiance, spawnp);
	if (id < 0)
		return (-1);
	player = get_obj(cub, id);
	cub->player_ids[cub->nb_players++] = id;
	if (is_playable)
	{
		set_playable_obj(cub, player);
		printf("setting new playable character obj id : %d (%d) at pos : (%.2f, %.2f)\n", id,
			cub->hero.ply_obj->_id, cub->hero.ply_obj->px, cub->hero.ply_obj->py);
	}
	return (id);
}

// Set player position to its currently set spawnpoint.
int	respawn_player(t_oinst *player)
{
	t_cub	*cub;
	t_oinst	*spawnp;
	
	if (!player || player->type->type_enum != OBJ_PLAYER)
		return (report_err("Trying to respawn none-player type."));
	spawnp = (t_oinst *)player->spawnpoint;
	cub = (t_cub *)spawnp->relative;
	obj_set_position(cub, player, spawnp->px, spawnp->py);
//	player->px = spawnp->px;
//	player->py = spawnp->py;
//	player->cx = spawnp->cx;
//	player->cy = spawnp->cx;
	if (player == cub->hero.ply_obj)
	{
		printf("RESPAWN PLAYABLE CHARACTER %p\n", cub->hero.ply_obj);
		update_rays(cub);
	}
	cub->renderer.requires_update = 1;
	return (0);
}
