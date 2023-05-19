/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_obj_model.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 09:30:18 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/19 07:01:28 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/// now morph to objx data manager ...
/// set array data ++ frame access
/// order by id ...

t_omdl	*init_player_model(t_objs *objs)
{
//	mlx_texture_t	*tex;
	printf("\n\nTrying to init player model.");
	if (objs->player.type_enum)
		return (NULL);
	printf("\n\n INITIALIZING PLAYER TYPE !!!! \n");
	objs->player.model_name = "player";
	objs->player.type_enum = OBJ_PLAYER;
	objs->player.is_drawable = 1;
	objs->player.is_oriented = 1;
	objs->player.nb_texs = 8;
	objs->player.draw_offy = 0;
	objs->player.gsets[0] = gset_builder("tex/gset_player/", objs->player.nb_texs);
	if (!objs->player.gsets[0])
		return (NULL);
	objs->player.gsets[1] = objs->player.gsets[0];
	objs->player.gsets[2] = objs->player.gsets[0];
	objs->player.gsets[3] = objs->player.gsets[0];
//	objs->player.gset =  gset_builder("tex/gset_p/", 4);
//	tex = objs->portal.gset->xwalls[0];
// 	printf("A Portal ptr : %p  \n", objs->portal.gset->xwalls[0]);
//	objs->player.texs[0] = objs->portal.gset->xwalls[0];
	objs->player.width = 32;
	objs->player.half_w = objs->player.width >> 1;
	objs->player.height = CELL_WIDTH;
//	objs->player.height = objs->portal.width * (tex->height / tex->width);
	objs->player.half_h = objs->player.height >> 1;
	// printf("Portal object model initialized !\n");
	return (&objs->player);
}

t_omdl	*init_spawnpoint_model(t_objs *objs)
{
//	mlx_texture_t	*tex;
	printf("player enum at init spawnpoint start: %d\n", objs->player.type_enum);
	if (objs->spawnp.type_enum)
		return (NULL);
	objs->spawnp.model_name = "spawnpoint";
	objs->spawnp.type_enum = OBJ_SPAWNPOINT;
	printf("INIT OBJ_SPAWNPOINT type struct with spawnp.type_enum = %d\n", objs->spawnp.type_enum);
	objs->spawnp.is_drawable = 0;
	objs->spawnp.is_oriented = 0;
	objs->spawnp.nb_texs = 0;
	objs->spawnp.draw_offy = 0;
//	objs->spawnp.gsets[0] = gset_builder("tex/gset_spawnp/", objs->spawnp.nb_texs);
//	if (!objs->spawnp.gsets[0])
//		return (NULL);
	objs->spawnp.width = CELL_WIDTH;
	objs->spawnp.half_w = objs->spawnp.width >> 1;
	objs->spawnp.height = CELL_WIDTH;
//	objs->spawnp.height = objs->portal.width * (tex->height / tex->width);
	objs->spawnp.half_h = objs->spawnp.height >> 1;
	// printf("Portal object model initialized !\n");
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
	printf("INIT OBJ_PORTAL type struct with portal.type_enum = %d\n", objs->portal.type_enum);
	objs->portal.is_drawable = 1;
	objs->portal.is_oriented = 0;
	objs->portal.nb_texs = 4;
	objs->portal.draw_offy = 0;
	objs->portal.gsets[0] =  gset_builder("tex/gset_p/", objs->portal.nb_texs);
	if (!objs->portal.gsets[0])
		return (ft_eprintf("PORTAL GSET failed to load !\n"), NULL);
	tex = objs->portal.gsets[0]->xwalls[0];
	// printf("A Portal ptr : %p  \n", objs->portal.gset->xwalls[0]);
	//		objs->portal.texs[0] = objs->portal.gset->xwalls[0];
	objs->portal.width = CELL_WIDTH;
	objs->portal.half_w = objs->portal.width >> 1;
	objs->portal.height = objs->portal.width * (tex->height / tex->width);
	objs->portal.half_h = objs->portal.height >> 1;
	objs->portal.proj_width = (int)(objs->portal.width * 0.7);//objs->portal.width >> 1;
	objs->portal.proj_height = (int)(objs->portal.height * 0.7);//objs->portal.height >> 1;
	// printf("Portal object model initialized !\n");
	return (&objs->portal);
}

t_omdl	*init_lever_model(t_objs *objs)
{
	mlx_texture_t	*tex;
	
	if (objs->lever.type_enum)
		return (NULL);
	objs->lever.model_name = "lever";
	objs->lever.type_enum = OBJ_LEVER;
	printf("INIT OBJ_LEVER type struct with lever.type_enum = %d\n", objs->lever.type_enum);
	objs->lever.is_drawable = 0;
	objs->lever.is_oriented = 0;
	objs->lever.nb_texs = 2;
//	objs->portal.draw_offy = 20;
	objs->lever.gsets[0] = gset_builder("tex/gset_lev/", objs->lever.nb_texs);
	if (!objs->lever.gsets[0])
		return (NULL);
	tex = objs->lever.gsets[0]->xwalls[0];
	// printf("A Lever ptr : %p  \n", objs->lever.gset->xwalls[0]);
//	objs->lever.texs[0] = objs->lever.gset->xwalls[0];
	objs->lever.width = CELL_WIDTH;
	printf("objs ptr : %p, tex ptr : %p\n", objs, tex);
	printf("objs->lever ptr : %p\n", &objs->lever);
	printf("objs->lever.width : %d\n", objs->lever.width);
	objs->lever.half_w = objs->lever.width >> 1;
	objs->lever.height = objs->lever.width * (tex->height / tex->width);
	objs->lever.half_h = objs->lever.height >> 1;
	// printf("Lever object model initialized !\n");
	printf("INIT OBJ_LEVER  at exit type struct with lever.type_enum = %d\n", objs->lever.type_enum);
	return (&objs->lever);
}

t_omdl	*init_fireball_model(t_objs *objs)
{
	mlx_texture_t	*tex;

	if (objs->fireball.type_enum)
		return (NULL);
	objs->fireball.model_name = "fireball";
	objs->fireball.type_enum = OBJ_FIREBALL;
	objs->fireball.is_drawable = 1;
	objs->fireball.is_oriented = 0;
	objs->fireball.nb_texs = 4;
	objs->fireball.draw_offy = 0;
	objs->fireball.gsets[0] = gset_builder("tex/gset_fireball/", objs->fireball.nb_texs);
	if (!objs->fireball.gsets[0])
		return (NULL);
	tex = objs->fireball.gsets[0]->xwalls[0];
	// printf("A Fireball ptr : %p  \n", objs->fball.gset->xwalls[0]);
//	objs->fball.texs[0] = objs->fball.gset->xwalls[0];
	objs->fireball.width = CELL_WIDTH;
	objs->fireball.half_w = objs->fireball.width >> 1;
	objs->fireball.height = objs->fireball.width * (tex->height / tex->width);
	objs->fireball.half_h = objs->fireball.height >> 1;
	objs->fireball.speed = 2.0f;
	// printf("FireBall object model initialized !\n");
	return (&objs->fireball);
}

t_omdl	*init_firepit_model(t_objs *objs)
{
	//const char	*tex_path1 = "tex/fireball/tmp/1_0.png";
//	const char	*tex_path1 = "tex/fireball/alpha_firepit.png";
	//	const char	*tex_path2 = "tex/fireball/tmp/1_1.png";
	mlx_texture_t	*tex;
	t_matrx			*gset;

	if (objs->firepit.type_enum)
		return (NULL);
	objs->firepit.model_name = "firepit";
	objs->firepit.type_enum = OBJ_FIREPIT;
	objs->firepit.is_drawable = 1;
	objs->firepit.is_oriented = 0;
	objs->firepit.nb_texs = 4;
	objs->firepit.draw_offy = 20;

	if (!ft_malloc_p(sizeof(t_matrx), (void **)&gset))
	{
		report_malloc_error();
		return (NULL);
	}
	objs->firepit.gsets[0] = gset_builder("tex/gset_firepit/", objs->firepit.nb_texs);
	if (!objs->firepit.gsets[0])
		return (NULL);
//	tex = mlx_load_png(tex_path1);
	tex = objs->firepit.gsets[0]->xwalls[0];
	//	objs->firepit.texs[0] = tex;
	//	if (!objs->firepit.texs[0])
	//		return (report_mlx_tex_load_failed((char *)tex_path1));
	printf("Init firepit model ; png load SUCCESSFUL !\n");
	//		printf("tex w h (%d, %d)\n", tex->width, tex->height);
	objs->firepit.width = CELL_WIDTH;
	objs->firepit.half_w = objs->firepit.width >> 1;
	objs->firepit.height = objs->firepit.width * (tex->height / (float)tex->width);
	objs->firepit.half_h = objs->firepit.height >> 1;
//	printf("firepit w h : (%d, %d), half w h (%d, %d)\n", 
//		objs->firepit.width, objs->firepit.height, objs->firepit.half_w, objs->firepit.half_h);
//	objs->firepit.speed = 1.0f;
//	objs->firepit.dmg = 5;
//	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
	printf("firepit object model initialized !\n");
	return (&objs->firepit);
}

void 	p_list_objx(t_objx **objx, int id, int num)
{
	if (num == 0)
		num = id + 1;
	while(id < num)
	{
		printf("LIST_META_obj_id[%d]::NAME:{%c} :: type_[%d] >>>",  objx[id]->obj_id, objx[id]->name, objx[id]->o_type);
		printf(" RLTV_[%c] <<< {A:%d}_", objx[id]->relativ, objx[id]->alleg );    
		printf("__( x[%d], y[%d] )::\n", objx[id]->opos[0], objx[id]->opos[1]);
		id++;
	}
}	

