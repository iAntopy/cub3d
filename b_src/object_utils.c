/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:55:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 22:37:37 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	obj_get_type(t_oinst *obj)
{
	return (obj->type->type_enum);
}

int	obj_get_issolid(t_oinst *obj)
{
	return (obj->type->is_solid);
}

int	obj_get_isactive(t_oinst *obj)
{
	return (obj->isactive);
}

int	obj_get_width(t_oinst *obj)
{
	return (obj->type->width);
}

int	*obj_type_alleg(int type, int alleg)
{
	static int	type_alleg[2];

	type_alleg[0] = type;
	type_alleg[1] = alleg;
	return (type_alleg);
}
