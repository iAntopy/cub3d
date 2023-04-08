/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_model_creation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/07 21:56:49 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// If more drawable models should exist, initiaize their models here.
int init_object_models(t_objs *objs)
{
	objs->portal.model_name = "Portal";
	objs->portal.type_enum = OBJ_PORTAL;
	objs->portal.width = CELL_WIDTH;
	objs->portal.half_width = objs->portal.width >> 1;
	objs->portal.max_texs = 1;
	objs->portal.texs[0] = mlx_load_png("tex/Portal1.png");
	if (!objs->portal.texs[0])
		return (-1);	
	return (0);
}

static int	create_portal_instance(t_cub *cub, int *pos, int *obj_id)
{
	t_oinst	*new_obj;

	if (!ft_malloc_p(sizeof(t_oinst), (void **)&new_obj))
		return (report_malloc_err());

	new_obj->type = &cub->objs.portal;
	new_obj->_id = ++(*obj_id);
	new_obj->tex_idx = 0;
	new_obj->px = pos[0];
	new_obj->py = pos[1];
	new_obj->next = cub->objs.instances;
	cub->objs.instances = new_obj;
	return (0);	
}

int	delete_obj_instance_by_id(t_cub *cub, int id)
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

// Add new obj to front of list.
int	create_obj_instance(t_cub *cub, int px, int py, int type_enum)
{
	static int	obj_id;
	int			cellx;
	int			celly;
	int			pos[2];
	
	cellx = px / CELL_WIDTH;
	celly = py / CELL_WIDTH;
	if (get_is_wall(&cub->map, cellx, celly))
		return (report_err("CUB ERROR : Trying to create object in wall.\n"));
	pos[0] = px;
	pos[1] = py;
	if (type_enum == OBJ_PORTAL)
		return (create_portal_instance(cub, pos, &obj_id));
	return (obj_id);
}