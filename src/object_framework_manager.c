/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_framework_manager.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 20:47:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/19 00:25:02 by iamongeo         ###   ########.fr       */
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
			if (gset->xwalls[i])
			{
				mlx_delete_texture(gset->xwalls[i]);
				gset->xwalls[i] = NULL;
			}
		}
	}
}

// Destroy all object instances AND free all object models.
void	clear_obj_framework(t_cub *cub)
{
	printf("Clearing objects framework\n");
	delete_all_obj_instances(cub);
	clear_obj_model(&cub->objs.player);
	clear_obj_model(&cub->objs.lever);
	clear_obj_model(&cub->objs.portal);
	clear_obj_model(&cub->objs.fireball);
	clear_obj_model(&cub->objs.firepit);
	// ADD clear_obj_model() calls as nb of init object models grow.
	printf("Clearing objects framework SUCCESS\n");
}

// If more drawable models should exist, initiaize their models here.
// Add more init model func calls as needed.
// int	init_obj_framework(t_cub *cub)
// {
// 	if (!init_portal_model(&cub->objs))
// 		return (-1);
// 	// ADD more models for extra fun.
// 	// ...
// 	return (0);
// }

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
int	create_obj_instance(t_cub *cub, float *pos, int type_enum, int allegiance,\
 void *param)
{
	int	obj_id;
	int	cell[2];

	cell[0] = (int)(pos[0] * cub->inv_cw);
	cell[1] = (int)(pos[1] * cub->inv_cw);
	printf("Creating obj with type : %d. IF is PORTAL = %d\n", type_enum, OBJ_PORTAL);
	if (is_wall(&cub->map, cell[0], cell[1]))
		return (report_err("ERROR : Trying to create object in wall.\n"));
	if (type_enum == OBJ_PLAYER)
		obj_id = create_player_instance(cub, pos, allegiance, param);
	else if (type_enum == OBJ_PORTAL)
	{
		printf("creating portal\n");
		obj_id = create_portal_instance(cub, pos, allegiance, param);
	}
	else if (type_enum == OBJ_LEVER)
	{
		printf("creating lever\n");
		obj_id = create_lever_instance(cub, pos, allegiance, param);
	}
	else if (type_enum == OBJ_FIREBALL)
		obj_id = create_fireball_instance(cub, pos, allegiance, param);
	else if (type_enum == OBJ_FIREPIT)
		obj_id = create_firepit_instance(cub, pos, allegiance, param);
	else
		return (ft_eprintf("Error : Invalid obj type\n"));
	return (obj_id);
}
