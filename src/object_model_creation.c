/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_model_creation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/15 15:12:39 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_new_obj_id(void)
{
	static int	counter;
	
	return (++counter);
}

int	report_obj_err(t_oinst *obj, char *msg)
{
	ft_eprintf("ERROR with obj %s <id %d, ptr %p> : %s\n", 
		obj->type->model_name, obj->_id, obj, msg);
	return (-1);
}

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
		tex = obj->type->gset->xwalls[0];
		obj->type->height = obj->type->width * (tex->height / tex->width);
	}
	else if (!obj->isactive && new_state == 1)
	{
		if (!obj->relative)
			return (ft_eprintf("Cannot activate portal without a link\n"), -1);
		printf("ACTIVATING PORTAL id %d\n", obj->_id);
		obj->isactive = 1;
		obj->tex_idx = 1;
		tex = obj->type->gset->xwalls[obj->allegiance];
		obj->type->height = obj->type->width * (tex->height / tex->width);
	}
	else
		return (-1);
	return (0);
}

// static t_omdl	*init_portal_model(t_objs *objs)
// {
// 	const char	*tex_path1 = "tex/ext/Portal1.png";
// 	const char	*tex_path2 = "tex/ext/Portal_void_whitealpha.png";
// 	mlx_texture_t	*tex;

// 	objs->portal.model_name = "Portal";
// 	objs->portal.type_enum = OBJ_PORTAL;
// 	objs->portal.nb_texs = 2;
// 	objs->portal.draw_offy = 0;
// 	printf("Init Portal model ; Try load  png\n");
// 	tex = mlx_load_png(tex_path1);
// 	objs->portal.texs[0] = tex;
// 	if (!objs->portal.texs[0])
// 		return (report_mlx_tex_load_failed((char *)tex_path1));
// //	objs->portal.texs[1] = tex;//mlx_load_png(tex_path2);
// 	objs->portal.texs[1] = mlx_load_png(tex_path2);
// 	if (!objs->portal.texs[1])
// 		return (report_mlx_tex_load_failed((char *)tex_path2));
// 	printf("Init Portal model ; png load SUCCESSFUL !\n");
// 	objs->portal.width = CELL_WIDTH;
// 	objs->portal.half_w = objs->portal.width >> 1;
// 	objs->portal.height = objs->portal.width * (tex->height / (float)tex->width);
// 	objs->portal.half_h = objs->portal.height >> 1;
// //	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
// 	printf("Portal object model initialized !\n");
// 	return (&objs->portal);
// }

// static t_omdl	*init_fireball_model(t_objs *objs)
// {
// 	//const char	*tex_path1 = "tex/fireball/tmp/1_0.png";
// 	const char	*tex_path1 = "tex/fireball/alpha_fireball.png";
// 	const char	*tex_path2 = "tex/fireball/tmp/1_1.png";
// 	mlx_texture_t	*tex;

// 	objs->fireball.model_name = "Fireball";
// 	objs->fireball.type_enum = OBJ_FIREBALL;
// 	objs->fireball.nb_texs = 2;
// 	objs->fireball.draw_offy = 0;
// 	printf("Init Fireball model ; Try load  png\n");
// 	tex = mlx_load_png(tex_path1);
// 	objs->fireball.texs[0] = tex;
// 	if (!objs->fireball.texs[0])
// 		return (report_mlx_tex_load_failed((char *)tex_path1));
// 	objs->fireball.texs[1] = mlx_load_png(tex_path2);
// 	if (!objs->fireball.texs[1])
// 		return (report_mlx_tex_load_failed((char *)tex_path2));
// 	printf("Init fireball model ; png load SUCCESSFUL !\n");
// 	printf("tex w h (%d, %d)\n", tex->width, tex->height);
// 	objs->fireball.width = 48;
// 	objs->fireball.half_w = objs->fireball.width >> 1;
// 	objs->fireball.height = objs->fireball.width * (tex->height / (float)tex->width);
// 	objs->fireball.half_h = objs->fireball.height >> 1;
// 	printf("fireball w h : (%d, %d), half w h (%d, %d)\n", 
// 		objs->fireball.width, objs->fireball.height, objs->fireball.half_w, objs->fireball.half_h);
// 	objs->fireball.speed = 1.0f;
// 	objs->fireball.dmg = 5;
// //	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
// 	printf("fireball object model initialized !\n");
// 	return (&objs->fireball);
// }


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


static void	clear_obj_model(t_omdl *mdl)
{
	t_matrx	*gset;
	int	i;

	if (!mdl || !mdl->gset)
		return ;
	gset = mdl->gset;
	i = -1;
	while (++i < mdl->nb_texs)
	{
		if (gset->xwalls[i])
		{
			mlx_delete_texture(gset->xwalls[i]);
			gset->xwalls[i] = NULL;
		}
	}
}

// pos is in world coord, NOT cell coord.
static int	create_portal_instance(t_cub *cub, int *pos, int allegiance,\
 t_oinst *link)
{
	t_oinst	*new_obj;

	if (link && link->type->type_enum != OBJ_PORTAL)
		return (report_err("Creating a portal instance trying to link to non portal obj.\n"));

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());

	new_obj->type = &cub->objs.portal;
	new_obj->_id = get_new_obj_id();
	new_obj->allegiance = allegiance;
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->action = __obj_action_portal;
	new_obj->relative = NULL;
	new_obj->isactive = 0;
//	new_obj->action = NULL;
	if (link)
	{
		printf("linking portal %d (%p) to portal %d (%p)\n", new_obj->_id, new_obj, link->_id, link);
		new_obj->relative = link;
		link->relative = new_obj;
	}
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single Portal instance [%d]created at pos (%d, %d)\n", allegiance, pos[0], pos[1]);
	return (new_obj->_id);	
}

// pos is in world coord, NOT cell coord.
static int	create_fireball_instance(t_cub *cub, int *pos, int allegiance, t_hero *link)
{
	t_oinst	*new_obj;

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());

	new_obj->type = &cub->objs.fireball;
	new_obj->_id = get_new_obj_id();
	new_obj->allegiance = allegiance;
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->dx = pos[2];
	new_obj->dy = pos[3];
	new_obj->relative = NULL;
//	new_obj->link = link;
	
	new_obj->action = __obj_action_fireball;
//	new_obj->action = NULL;
	if (link)
	{
//		printf("linking fireball %d (%p) to portal %d (%p)\n", new_obj->_id, new_obj, link->_id, link);
		new_obj->relative = link;
		new_obj->isactive = 1;
//		link->link = new_obj;
	}
	else
		new_obj->isactive = 0;
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single fireball instance created at pos (%d, %d)\n", pos[0], pos[1]);
	return (new_obj->_id);	
}

static int	create_lever_instance(t_cub *cub, int *pos, int allegiance, t_oinst *link)
{
	t_oinst	*new_obj;

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());

	new_obj->type = &cub->objs.lever;
	new_obj->_id = get_new_obj_id();
	new_obj->tex_idx = 0;
	new_obj->allegiance = allegiance;
	new_obj->px = (float)pos[0];
	new_obj->py = (float)pos[1];
	new_obj->relative = link;
	new_obj->isactive = 0;	
	new_obj->action = __obj_action_lever;
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	new_obj->special_gset.xwalls[1] = cub->map.mx[pos[1]][pos[0]]->xwalls[1];
	new_obj->special_gset.xwalls[0] = new_obj->type->gset->xwalls[0];
	cub->map.mx[pos[1]][pos[0]] = &new_obj->special_gset;
//	dual = cub->map.mx[pos[1]][pos[0]];
//	dual->xwalls[0] = new_obj->type->gset->xwalls[new_obj->tex_idx];
	return (new_obj->_id);
}

// pos is in world coord, NOT cell coord.
// static int	create_firepit_instance(t_cub *cub, int *pos, int *obj_id, t_hero *link)
// {
// 	t_oinst	*new_obj;

// //	if (link && link->type->type_enum != OBJ_PORTAL)
// //		return (report_err("Creating a fireball instance trying to link to non portal obj.\n"));

// 	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
// 		return (report_malloc_error());

// 	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
// 		return (report_malloc_error());

// 	new_obj->type = &cub->objs.firepit;
// 	new_obj->_id = ++(*obj_id);
// 	new_obj->tex_idx = 0;
// 	new_obj->px = pos[0];
// 	new_obj->py = pos[1];
// //	new_obj->dx = pos[2];
// //	new_obj->dy = pos[3];
// 	new_obj->relative = NULL;
// 	new_obj->isactive = 0;
// //	new_obj->link = link;
	
// 	new_obj->action = __obj_action_firepit;
// //	new_obj->action = NULL;
// 	if (link)
// 	{
// //		printf("linking fireball %d (%p) to portal %d (%p)\n", new_obj->_id, new_obj, link->_id, link);
// 		new_obj->relative = link;
// 		new_obj->isactive = 1;
// //		link->link = new_obj;
// 	}
// 	new_obj->next = cub->objs.instances;
// 	cub->objs.instances = new_obj;
// 	printf("Single firepit instance created at pos (%d, %d)\n", pos[0], pos[1]);
// 	return (new_obj->_id);	
// }

t_oinst	*get_obj(t_cub *cub, int id)
{
	t_oinst	*elem;

	printf("get oinst for id %d\n", id);
	elem = cub->objs.instances;
	while (elem)
	{
		if (elem->_id == id)
		{
			printf("OBJ with id %d FOUND : %p\n", id, elem);
			return (elem);
		}
		elem = elem->next;
	}
	printf("No object with id %d\n", id);
	return (NULL);
}

// Delete one specific object with id.
int	delete_oinst_by_id(t_cub *cub, int id)
{
	t_oinst	*elem;
	t_oinst	*tmp;
	
	elem = cub->objs.instances;
	if (!elem)
		return (-1);
	if (elem->_id == id)
	{
		cub->objs.instances = elem->next;
		return (ft_free_p((void **)&elem));
	}
	while (elem->next)
	{
		if (elem->next->_id == id)
		{
			tmp = elem->next;
			elem->next = elem->next->next;
			return (ft_free_p((void **)&tmp));
		}
		elem = elem->next;
	}
	return (-1);
}

// Delete all instances of a specific object type. eg.: delete all OBJ_PORTAL
int	delete_all_oinst_by_type(t_cub *cub, int type_enum)
{
	t_oinst	*elem;
	t_oinst	*tmp;
	
	elem = cub->objs.instances;
	if (!elem)
		return (-1);
	if (elem->type->type_enum == type_enum)
	{
		cub->objs.instances = elem->next;
		return (ft_free_p((void **)&elem));
	}
	while (elem->next)
	{
		if (elem->next->type->type_enum == type_enum)
		{
			tmp = elem->next;
			elem->next = elem->next->next;
			ft_free_p((void **)&tmp);
		}
		else
			elem = elem->next;
	}
	return (0);
}

// Destroys all object instances in world.
void	delete_all_obj_instances(t_cub *cub)
{
	t_oinst	*elem;
	t_oinst	*tmp;

	if (!cub->objs.instances)
		return ;
	elem = cub->objs.instances;
	while (elem)
	{
		tmp = elem->next;
		ft_free_p((void **)&elem);
		elem = tmp;
	}
	cub->objs.instances = NULL;
}

// Destroy all object instances AND free all object models.
void	clear_obj_framework(t_cub *cub)
{
	printf("Clearing objects framework\n");
	delete_all_obj_instances(cub);
	clear_obj_model(&cub->objs.portal);
	clear_obj_model(&cub->objs.fireball);
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
int	create_obj_instance(t_cub *cub, int *pos, int type_enum, int allegiance,\
 void *param)
{
	int	obj_id;
	int	cell[2];
	
	cell[0] = pos[0] / CELL_WIDTH;
	cell[1] = pos[1] / CELL_WIDTH;
	if (is_wall(&cub->map, cell[0], cell[1]))
		return (report_err("ERROR : Trying to create object in wall.\n"));
	if (type_enum == OBJ_PORTAL)
		obj_id = create_portal_instance(cub, pos, allegiance, param);
	else if (type_enum == OBJ_LEVER)
		obj_id = create_lever_instance(cub, cell, allegiance, param);
	else if (type_enum == OBJ_FIREBALL)
		obj_id = create_fireball_instance(cub, pos, allegiance, param);
	else
		return (ft_eprintf("Error : Invalid obj type\n"));
	// else if (type_enum == OBJ_FIREPIT)
	// 	obj_id = create_firepit_instance(cub, pos, &obj_id, param);
	return (obj_id);
}
