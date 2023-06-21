/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_obj_model.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 09:30:18 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/14 20:27:53 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_omdl	*init_player_model(t_objs *objs)
{
	mlx_texture_t	*tex;

	if (objs->player.type_enum)
		return (NULL);
	objs->player.model_name = "player";
	objs->player.type_enum = OBJ_PLAYER;
	objs->player.is_drawable = 1;
	objs->player.is_oriented = 1;
	objs->player.is_solid = 1;
	objs->player.nb_texs = 8;
	objs->player.offy = 20;
	objs->player.gsets[0] = gset_builder("tex/gset_player/",
			objs->player.nb_texs);
	if (!objs->player.gsets[0])
		return (NULL);
	objs->player.gsets[1] = objs->player.gsets[0];
	objs->player.gsets[2] = objs->player.gsets[0];
	objs->player.gsets[3] = objs->player.gsets[0];
	tex = objs->portal.gsets[0]->xwalls[0];
	objs->player.width = 40;
	objs->player.half_w = objs->player.width >> 1;
	objs->player.height = (int)(objs->player.width * (tex->height
				/ (float)tex->width));
	objs->player.half_h = objs->player.height >> 1;
	return (&objs->player);
}

t_omdl	*init_spawnpoint_model(t_objs *objs)
{
	if (objs->spawnp.type_enum)
		return (NULL);
	objs->spawnp.model_name = "spawnpoint";
	objs->spawnp.type_enum = OBJ_SPAWNPOINT;
	printf("INIT OBJ_SPAWNPOINT type struct with spawnp.type_enum = %d\n",
		objs->spawnp.type_enum);
	objs->spawnp.is_drawable = 0;
	objs->spawnp.is_oriented = 0;
	objs->spawnp.nb_texs = 0;
	objs->spawnp.offy = 0;
	objs->spawnp.width = CELL_WIDTH;
	objs->spawnp.half_w = objs->spawnp.width >> 1;
	objs->spawnp.height = CELL_WIDTH;
	objs->spawnp.half_h = objs->spawnp.height >> 1;
	printf("player enum at init spawnpoint end: %d\n", objs->player.type_enum);
	return (&objs->spawnp);
}

t_omdl	*init_portal_model(t_objs *objs)
{
	mlx_texture_t	*tex;

	if (objs->portal.type_enum)
		return (NULL);
	objs->portal.model_name = "portal";
	objs->portal.type_enum = OBJ_PORTAL;
	objs->portal.is_drawable = 1;
	objs->portal.is_oriented = 0;
	objs->portal.is_solid = 0;
	objs->portal.nb_texs = 4;
	objs->portal.offy = 0;
	objs->portal.gsets[0] = gset_builder("tex/gset_p/",
			objs->portal.nb_texs);
	if (!objs->portal.gsets[0])
		return (ft_eprintf("PORTAL GSET failed to load !\n"), NULL);
	tex = objs->portal.gsets[0]->xwalls[0];
	objs->portal.width = CELL_WIDTH;
	objs->portal.half_w = objs->portal.width >> 1;
	objs->portal.height = (int)(objs->portal.width *(tex->height
				/ (float)tex->width));
	objs->portal.half_h = objs->portal.height >> 1;
	objs->portal.proj_width = (int)(objs->portal.width * 0.7);
	objs->portal.proj_height = (int)(objs->portal.height * 0.7);
	return (&objs->portal);
}

t_omdl	*init_lever_model(t_objs *objs)
{
	mlx_texture_t	*tex;

	if (objs->lever.type_enum)
		return (NULL);
	objs->lever.model_name = "lever";
	objs->lever.type_enum = OBJ_LEVER;
	objs->lever.is_drawable = 0;
	objs->lever.is_oriented = 0;
	objs->lever.nb_texs = 2;
	objs->lever.gsets[0] = gset_builder("tex/gset_lev/", objs->lever.nb_texs);
	if (!objs->lever.gsets[0])
		return (NULL);
	tex = objs->lever.gsets[0]->xwalls[0];
	objs->lever.width = CELL_WIDTH;
	objs->lever.half_w = objs->lever.width >> 1;
	objs->lever.height = (int)(objs->lever.width * (tex->height
				/ (float)tex->width));
	objs->lever.half_h = objs->lever.height >> 1;
	return (&objs->lever);
}
