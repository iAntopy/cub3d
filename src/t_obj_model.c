/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_obj_model.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 09:30:18 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/14 22:52:53 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/// now morph to objx data manager ...
/// set array data ++ frame access
/// order by id ...

t_omdl	*init_portal_model(t_objs *objs)
{
	mlx_texture_t	*tex;
	
	objs->portal.model_name = "portal";
	objs->portal.type_enum = OBJ_PORTAL;
	objs->portal.is_drawable = 1;
	objs->portal.nb_texs = 4;
	objs->portal.draw_offy = 0;
	objs->portal.gset =  gset_builder("tex/gset_p/", 4);
	tex = objs->portal.gset->xwalls[0];
	// printf("A Portal ptr : %p  \n", objs->portal.gset->xwalls[0]);
//	objs->portal.texs[0] = objs->portal.gset->xwalls[0];
	objs->portal.width = CELL_WIDTH;
	objs->portal.half_w = objs->portal.width >> 1;
	objs->portal.height = objs->portal.width * (tex->height / tex->width);
	objs->portal.half_h = objs->portal.height >> 1;
	// printf("Portal object model initialized !\n");
	return (&objs->portal);
}

t_omdl	*init_lever_model(t_objs *objs)
{
	mlx_texture_t	*tex;
	
	objs->lever.model_name = "lever";
	objs->lever.type_enum = OBJ_LEVER;
	objs->lever.is_drawable = 0;
	objs->lever.nb_texs = 2;
//	objs->portal.draw_offy = 20;
	objs->lever.gset = gset_builder("tex/gset_lev/", 2);
	tex = objs->lever.gset->xwalls[0];
	// printf("A Lever ptr : %p  \n", objs->lever.gset->xwalls[0]);
//	objs->lever.texs[0] = objs->lever.gset->xwalls[0];
	objs->lever.width = CELL_WIDTH;
	objs->lever.half_w = objs->lever.width >> 1;
	objs->lever.height = objs->lever.width * (tex->height / tex->width);
	objs->lever.half_h = objs->lever.height >> 1;
	// printf("Lever object model initialized !\n");
	return (&objs->lever);
}

t_omdl	*init_fireball_model(t_objs *objs)
{
	mlx_texture_t	*tex;

	objs->fball.model_name = "fireball";
	objs->fball.type_enum = OBJ_FIREBALL;
	objs->fball.is_drawable = 1;
	objs->fball.nb_texs = 6;
	objs->fball.draw_offy = 0;
	objs->fball.gset = gset_builder("tex/gset_fb/", 6);
	tex = objs->lever.gset->xwalls[0];
	// printf("A Fireball ptr : %p  \n", objs->fball.gset->xwalls[0]);
//	objs->fball.texs[0] = objs->fball.gset->xwalls[0];
	objs->fball.width = CELL_WIDTH;
	objs->fball.half_w = objs->fball.width >> 1;
	objs->fball.height = objs->fball.width * (tex->height / tex->width);
	objs->fball.half_h = objs->fball.height >> 1;
	// printf("FireBall object model initialized !\n");
	return (&objs->fball);
}

// static t_omdl	*init_firepit_model(t_objs *objs)
// {
// 	//const char	*tex_path1 = "tex/fireball/tmp/1_0.png";
// 	const char	*tex_path1 = "tex/fireball/alpha_firepit.png";
// //	const char	*tex_path2 = "tex/fireball/tmp/1_1.png";
// 	mlx_texture_t	*tex;

// 	objs->firepit.model_name = "Fireball";
// 	objs->firepit.type_enum = OBJ_FIREPIT;
// 	objs->firepit.nb_texs = 1;
// 	objs->firepit.draw_offy = 20;
// 	printf("Init Fireball model ; Try load  png\n");
// 	tex = mlx_load_png(tex_path1);
// 	objs->firepit.texs[0] = tex;
// 	if (!objs->firepit.texs[0])
// 		return (report_mlx_tex_load_failed((char *)tex_path1));
// 	printf("Init firepit model ; png load SUCCESSFUL !\n");
// //	printf("tex w h (%d, %d)\n", tex->width, tex->height);
// 	objs->firepit.width = CELL_WIDTH;
// 	objs->firepit.half_w = objs->firepit.width >> 1;
// 	objs->firepit.height = objs->firepit.width * (tex->height / (float)tex->width);
// 	objs->firepit.half_h = objs->firepit.height >> 1;
// //	printf("firepit w h : (%d, %d), half w h (%d, %d)\n", 
// //		objs->firepit.width, objs->firepit.height, objs->firepit.half_w, objs->firepit.half_h);
// //	objs->firepit.speed = 1.0f;
// //	objs->firepit.dmg = 5;
// //	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
// 	printf("firepit object model initialized !\n");
// 	return (&objs->firepit);
// }

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

t_objx	*get_pos(t_cub *cub, t_map *m, int o_cells, int id)
{
	t_objx *objx;
	char idx;

	/// add new id_member
	objx = (t_objx *)malloc(sizeof(t_objx ) * 1);    
	idx = *ft_substr(cub->box.chrs, o_cells, 1);
	
	objx->name = idx;//cub->box.chrs[o_cells];		 // '#' char name 
	objx->obj_id = id;		          
	if (o_cells < cub->box.n_lvls)
		objx->o_type = 2;
	else if (o_cells < (cub->box.n_prts + cub->box.n_lvls))
		objx->o_type = 1;
	else
		objx->o_type = 3;
	// printf("META_ID[%d]typ[%d](Name//Alleg//Reltv)::{%c}::",objx->obj_id, objx->o_type, objx->name);
	objx->alleg = m->raw[o_cells][2] - 48;		    // txtr_ref
	objx->relativ =  m->raw[o_cells][4];
	// printf("[%d]::{%c}",  objx->alleg, objx->relativ);
	objx->opos[0] = m->pos_x;
	objx->opos[1] = m->pos_y;
	// printf("_(x[%d], y[%d])::\n\n", objx->opos[0], objx->opos[1]);
	if (m->pos_x <= 0 || m->pos_y <= 0)
	{
		report_err("No META char found in map.");
		m->flg_chk = 1;
		return (objx);
	}
	
	return (objx);
}
