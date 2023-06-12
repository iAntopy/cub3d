/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawnpoint_funcs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 19:22:15 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 20:24:19 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	set_playable_obj(t_cub *cub, t_oinst *player)
{
	if (!cub || !player)
		return (report_err("set_playable_object : missing inputs."));
	cub->hero.ply_obj = player;
	init_raycaster(cub);
	init_rdata_consts(cub, &cub->hero.rcast,
		cub->hero.rcast.rdata, cub->hero.rcast.pdata);
	update_rays(cub);
	cub->renderer.requires_update = 1;
	return (0);
}

int	spawn_new_player(t_oinst *spawnp, int is_playable)
{
	t_cub	*cub;
	t_oinst	*player;
	int		id;
	float	pos[2];

	if (!spawnp || obj_get_type(spawnp) != OBJ_SPAWNPOINT)
		return (-1);
	cub = (t_cub *)spawnp->relative;
	if (cub->nb_players >= MAX_PLAYERS)
		return (-1);
	pos[0] = spawnp->px;
	pos[1] = spawnp->py;
	id = create_player_instance(cub, pos, spawnp->alleg, spawnp);
	if (id < 0)
		return (-1);
	player = get_obj(cub, id);
	cub->player_ids[cub->nb_players++] = id;
	if (is_playable)
		set_playable_obj(cub, player);
	return (id);
}

// Set player position to its currently set spawnpoint.
int	respawn_player(t_oinst *player)
{
	t_cub	*cub;
	t_oinst	*spawnp;

	if (!player || obj_get_type(player) != OBJ_PLAYER)
		return (report_err("Trying to respawn none-player type."));
	spawnp = (t_oinst *)player->spawnpoint;
	cub = (t_cub *)spawnp->relative;
	obj_set_position(cub, player, spawnp->px, spawnp->py);
	if (player == cub->hero.ply_obj)
		update_rays(cub);
	cub->renderer.requires_update = 1;
	return (0);
}
