/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_model_creation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/03 22:57:15 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Pass either OBJ_ACTIVATE or OBJ_DEACTIVATE as mode.
int	activate_portal(t_oinst *obj, int mode)
{
	mlx_texture_t	*tex;

	if (obj->type->type_enum != OBJ_PORTAL)
		return (ft_eprintf("ACTIVATION FAILED : obj id %d (%p) is not a portal obj !\n", obj->_id, obj));
	if (obj->isactive && mode == OBJ_DEACTIVATE)
	{
		printf("DEACTIVATING PORTAL id %d\n", obj->_id);
		obj->isactive = 0;
		obj->tex_idx = 0;
		tex = obj->type->texs[0];
		obj->type->height = obj->type->width * (tex->height / tex->width);
	}
	else if (!obj->isactive && mode == OBJ_ACTIVATE)
	{
		if (!obj->link)
			return (ft_eprintf("Cannot activate portal without a link\n"), -1);
		printf("ACTIVATING PORTAL id %d\n", obj->_id);
		obj->isactive = 1;
		obj->tex_idx = 1;
		tex = obj->type->texs[1];
		obj->type->height = obj->type->width * (tex->height / tex->width);
	}
	else
		return (-1);
	return (0);
}

static t_omdl	*init_portal_model(t_objs *objs)
{
	const char	*tex_path1 = "tex/ext/Portal1.png";
	const char	*tex_path2 = "tex/ext/Portal_void_whitealpha.png";
	mlx_texture_t	*tex;

	objs->portal.model_name = "Portal";
	objs->portal.type_enum = OBJ_PORTAL;
	objs->portal.nb_texs = 1;
	printf("Init Portal model ; Try load  png\n");
	tex = mlx_load_png(tex_path1);
	objs->portal.texs[0] = tex;
	if (!objs->portal.texs[0])
		return (report_mlx_tex_load_failed((char *)tex_path1));
//	objs->portal.texs[1] = tex;//mlx_load_png(tex_path2);
	objs->portal.texs[1] = mlx_load_png(tex_path2);
	if (!objs->portal.texs[1])
		return (report_mlx_tex_load_failed((char *)tex_path2));
	printf("Init Portal model ; png load SUCCESSFUL !\n");
	objs->portal.width = CELL_WIDTH;
	objs->portal.half_w = objs->portal.width >> 1;
	objs->portal.height = objs->portal.width * (tex->height / tex->width);
	objs->portal.half_h = objs->portal.height >> 1;
//	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
	printf("Portal object model initialized !\n");
	return (&objs->portal);
}

static void	clear_obj_model(t_omdl *mdl)
{
	int	i;

	i = -1;
	while (++i < mdl->nb_texs)
	{
		if (mdl->texs[i])
		{
			mlx_delete_texture(mdl->texs[i]);
			mdl->texs[i] = NULL;
		}
	}
}

// pos is in world coord, NOT cell coord.
static int	create_portal_instance(t_cub *cub, int *pos, int *obj_id, t_oinst *link)
{
	t_oinst	*new_obj;

	if (link && link->type->type_enum != OBJ_PORTAL)
		return (report_err("Creating a portal instance trying to link to non portal obj.\n"));

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());

	new_obj->type = &cub->objs.portal;
	new_obj->_id = ++(*obj_id);
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->link = link;
	if (link)
	{
		printf("linking portal %d (%p) to portal %d (%p)\n", new_obj->_id, new_obj, link->_id, link);
		link->link = new_obj;
	}
	new_obj->isactive = 0;
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single Portal instance created at pos (%d, %d)\n", pos[0], pos[1]);
	return (new_obj->_id);	
}

t_oinst	*get_oinst_by_id(t_cub *cub, int id)
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
void	destroy_all_obj_instances(t_cub *cub)
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
	destroy_all_obj_instances(cub);
	clear_obj_model(&cub->objs.portal);
	// ADD clear_obj_model() calls as nb of init object models grow.
	printf("Clearing objects framework SUCCESS\n");
}

// If more drawable models should exist, initiaize their models here.
// Add more init model func calls as needed.
int	init_obj_framework(t_cub *cub)
{
	if (!init_portal_model(&cub->objs))
		return (-1);
	// ADD more models for extra fun.
	// ...
	return (0);
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
int	create_obj_instance(t_cub *cub, int pos[2], int type_enum, void *param)
{
	static int	obj_id;
	int			cellx;
	int			celly;
	
	cellx = pos[0] / CELL_WIDTH;
	celly = pos[1] / CELL_WIDTH;
	if (is_wall(&cub->map, cellx, celly))
		return (report_err("ERROR : Trying to create object in wall.\n"));
	if (type_enum == OBJ_PORTAL)
		obj_id = create_portal_instance(cub, pos, &obj_id, param);
	return (obj_id);
}
