/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_deleters_and_getter.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 22:38:51 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_oinst	*get_obj(t_cub *cub, int id)
{
	t_oinst	*elem;

	elem = cub->objs.instances;
	while (elem)
	{
		if (elem->_id == id)
			return (elem);
		elem = elem->next;
	}
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
	if (obj_get_type(elem) == type_enum)
	{
		cub->objs.instances = elem->next;
		return (ft_free_p((void **)&elem));
	}
	while (elem->next)
	{
		if (obj_get_type(elem->next) == type_enum)
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
