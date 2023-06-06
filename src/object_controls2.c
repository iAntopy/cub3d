/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_controls2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 23:02:12 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 22:39:39 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	next_obj(t_oinst **obj_p)
{
	*obj_p = (*obj_p)->next;
	return (1);
}

void	obj_look_at(t_oinst *obj, t_oinst *target)
{
	float	dist;

	find_vector_delta(&obj->px, &target->px, &obj->dx);
	normalize_vec2(&obj->dx, &dist);
	obj->ori = atan2(obj->dy, obj->dx);
	if (obj->ori < 0)
		obj->ori += M_TAU;
}
