/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_framework_manager.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 20:47:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/23 16:41:42 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_new_obj_id(void)
{
	static int	counter;

	return (++counter);
}

static void	clear_obj_model(t_omdl *mdl)
{
	t_matrx	*gset;
	int		i;
	int		j;

	if (!mdl || !mdl->gsets[0])
		return ;
	i = -1;
	while (++i < 4)
	{
		gset = mdl->gsets[i];
		if (!gset)
			continue ;
		j = -1;
		while (++j < mdl->nb_texs)
		{
			if (gset->xwalls[j])
			{
				mlx_delete_texture(gset->xwalls[j]);
				gset->xwalls[j] = NULL;
			}
		}
		ft_free_p((void **)&mdl->gsets[i]);
	}
}

// This exists only temporarly while all player textures for all allegiances
// are the same.
static void	clear_player_model(t_omdl *mdl)
{
	t_matrx	*gset;
	int		i;

	if (!mdl || !mdl->gsets[0])
		return ;
	gset = mdl->gsets[0];
	i = -1;
	while (++i < mdl->nb_texs)
	{
		if (gset->xwalls[i])
		{
			mlx_delete_texture(gset->xwalls[i]);
			gset->xwalls[i] = NULL;
		}
	}
	ft_free_p((void **)&mdl->gsets[0]);
	mdl->gsets[1] = NULL;
	mdl->gsets[2] = NULL;
	mdl->gsets[3] = NULL;
}

// Destroy all object instances AND free all object models.
void	clear_obj_framework(t_cub *cub)
{
	delete_all_obj_instances(cub);
	clear_player_model(&cub->objs.player);
	clear_obj_model(&cub->objs.lever);
	clear_obj_model(&cub->objs.portal);
	clear_obj_model(&cub->objs.fireball);
	clear_obj_model(&cub->objs.firepit);
}

// Add new obj to front of list.
// param is an instance creation type specific argument.
//	ex: portal instance can take an instance of 2nd portal as argument.
//	the param portal will now link to the new instance created and the
//	new portal will loop back to param by default. If a third portal is created
//	linking to the 2nd one, it will create a chain (1 -> 2 -> 3 -> 2).
//	By changing the link manually of portal 3 to portal 1, it will create a
//	loop (1 -> 2 -> 3 -> 1). If pairs of portals are created where the first
//	one is without param and 2nd links to the 1st it will create pairs of
//	linking portals (1 <-> 2, 3 <-> 4, ...). A portal without link always
//	show the empty portal texture.
//	The pos variable index 0-1 are x, y position coords. It can also be a larger
//	array in case of fireballs with indexes 2-3 being direction information.
// Allegiance is an e_object_allegiance enum,
// either ALI_NEUTRAL, ALI_TORRENT, ALI_LEGION, ALI_ARMADA.
// type enum and alleg must be placed in an array[2] and passed as arg.
int	create_obj_instance(t_cub *cub, float *pos, int *type_alleg, void *arg)
{
	int	obj_id;
	int	cell[2];

	cell[0] = (int)(pos[0] * cub->inv_cw);
	cell[1] = (int)(pos[1] * cub->inv_cw);
	if (is_wall(&cub->map, cell[0], cell[1]))
		return (report_err("Trying to create object in wall."));
	if (type_alleg[0] == OBJ_PLAYER)
		obj_id = create_player_instance(cub, pos, type_alleg[1], arg);
	else if (type_alleg[0] == OBJ_SPAWNPOINT)
		obj_id = create_spawnp_instance(cub, pos, type_alleg[1]);
	else if (type_alleg[0] == OBJ_PORTAL)
		obj_id = create_portal_instance(cub, pos, type_alleg[1], arg);
	else if (type_alleg[0] == OBJ_LEVER)
		obj_id = create_lever_instance(cub, pos, type_alleg[1], arg);
	else if (type_alleg[0] == OBJ_FIREBALL)
		obj_id = create_fireball_instance(cub, pos, type_alleg[1], arg);
	else if (type_alleg[0] == OBJ_FIREPIT)
		obj_id = create_firepit_instance(cub, pos, type_alleg[1], arg);
	else
		return (report_err("Trying to create object with invalid obj type"));
	return (obj_id);
}
