/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_activators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 22:11:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/20 15:02:40 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// Pass either OBJ_ACTIVATE or OBJ_DEACTIVATE as mode.
int	activate_portal(t_oinst *obj, unsigned int new_state)
{
	mlx_texture_t	*tex;

	if (obj->type->type_enum != OBJ_PORTAL)
		return (report_obj_err(obj, "ACTIVATION FAILED : not a portal obj.\n"));
	else if (new_state >= 2)
		return (report_obj_err(obj, "ACTIVATION FAILED : invalid state.\n"));
	if (obj->isactive && new_state == 0)
	{
		printf("DEACTIVATING PORTAL id %d\n", obj->_id);
		obj->isactive = 0;
		obj->tex_idx = 0;
		tex = obj->gset->xwalls[0];
		obj->type->height = obj->type->width * (tex->height / tex->width);
	}
	else if (!obj->isactive && new_state == 1)
	{
		if (!obj->relative)
			return (ft_eprintf("Cannot activate portal without a link\n"), -1);
		printf("ACTIVATING PORTAL id %d, allegiance : %d\n", obj->_id, obj->allegiance);
		obj->isactive = 1;
		obj->tex_idx = obj->allegiance;
		tex = obj->gset->xwalls[obj->allegiance];
		obj->type->height = obj->type->width * (tex->height / tex->width);
	}
	else
		return (-1);
	return (0);
}

// int	set_playable_obj(t_cub *cub, t_oinst *player)
// {
	
// }

// int	spawn_new_player(t_oinst *spawnp, int is_playable)
// {
// 	t_cub	*cub;
// 	t_oinst	*player;
// 	int		id;
// 	int		pos[2];

// 	if (!spawnp || spawnp->type->type_enum != OBJ_SPAWNPOINT)
// 		return (-1);
// 	cub = (t_cub *)spawnp->relative;
// 	pos[0] = spawnp->px;
// 	pos[1] = spawnp->py;
// 	id = create_player_instance(cub, pos, spawnp->allegiance, spawnp);
// 	if (is_playable)
// 		set_playable_obj(cub, player);
// 	return (0);
// }