/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_model_creation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/28 11:07:46 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_omdl	*init_portal_model(t_objs *objs)
{
	const char	*tex_path = "tex/ext/Portal1.png";
	mlx_texture_t	*tex;

	objs->portal.model_name = "Portal";
	objs->portal.type_enum = OBJ_PORTAL;
	objs->portal.nb_texs = 1;
	printf("Init Portal model ; Try load  png\n");
	tex = mlx_load_png(tex_path);
	if (!tex)
		return (report_mlx_tex_load_failed((char *)tex_path));
	printf("Init Portal model ; png load SUCCESSFUL !\n");
	objs->portal.width = CELL_WIDTH;
	objs->portal.half_w = objs->portal.width >> 1;
	objs->portal.height = objs->portal.width * (tex->height / tex->width);
	objs->portal.half_h = objs->portal.height >> 1;
	printf("Portal object model initialized !\n");
	objs->portal.texs[0] = tex;
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
static int	create_portal_instance(t_cub *cub, int *pos, int *obj_id)
{
	t_oinst	*new_obj;

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_error());

	new_obj->type = &cub->objs.portal;
	new_obj->_id = ++(*obj_id);
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	printf("Single Portal instance created at pos (%d, %d)\n", pos[0], pos[1]);
	return (0);	
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
int	create_obj_instance(t_cub *cub, int px, int py, int type_enum)
{
	static int	obj_id;
	int			cellx;
	int			celly;
	int			pos[2];
	
	cellx = px / CELL_WIDTH;
	celly = py / CELL_WIDTH;
	if (is_wall(&cub->map, cellx, celly))
		return (report_err("ERROR : Trying to create object in wall.\n"));
	pos[0] = px;
	pos[1] = py;
	if (type_enum == OBJ_PORTAL)
		obj_id = create_portal_instance(cub, pos, &obj_id);
	return (obj_id);
}
