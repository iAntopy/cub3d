/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_obj_model_extend.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 00:02:51 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/20 19:50:57 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_omdl	*init_fireball_model(t_objs *objs)
{
	mlx_texture_t	*tex;

	if (objs->fireball.type_enum)
		return (NULL);
	objs->fireball.model_name = "fireball";
	objs->fireball.type_enum = OBJ_FIREBALL;
	objs->fireball.is_drawable = 1;
	objs->fireball.is_oriented = 0;
	objs->fireball.is_solid = 0;
	objs->fireball.nb_texs = 4;
	objs->fireball.offy = 0;
	objs->fireball.gsets[0] = gset_builder("tex/gset_fb/",
			objs->fireball.nb_texs);
	if (!objs->fireball.gsets[0])
		return (NULL);
	tex = objs->fireball.gsets[0]->xwalls[0];
	objs->fireball.width = CELL_WIDTH;
	objs->fireball.half_w = objs->fireball.width >> 1;
	objs->fireball.height = (int)(objs->fireball.width * (tex->height
				/ (float)tex->width));
	objs->fireball.half_h = objs->fireball.height >> 1;
	objs->fireball.speed = 2.0f;
	return (&objs->fireball);
}

t_omdl	*init_firepit_model(t_objs *objs)
{
	mlx_texture_t	*tex;

	if (objs->firepit.type_enum)
		return (NULL);
	objs->firepit.model_name = "firepit";
	objs->firepit.type_enum = OBJ_FIREPIT;
	objs->firepit.is_drawable = 1;
	objs->firepit.is_oriented = 0;
	objs->firepit.is_solid = 1;
	objs->firepit.nb_texs = 4;
	objs->firepit.offy = 20;
	objs->firepit.gsets[0] = gset_builder("tex/gset_firepit/",
			objs->firepit.nb_texs);
	if (!objs->firepit.gsets[0])
		return (NULL);
	tex = objs->firepit.gsets[0]->xwalls[0];
	objs->firepit.width = CELL_WIDTH;
	objs->firepit.half_w = objs->firepit.width >> 1;
	objs->firepit.height = (int)(objs->firepit.width * (tex->height
				/ (float)tex->width));
	objs->firepit.half_h = objs->firepit.height >> 1;
	return (&objs->firepit);
}
