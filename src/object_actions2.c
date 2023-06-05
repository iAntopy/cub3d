/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_actions2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 16:08:21 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 16:08:54 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


int	__obj_action_lever(t_oinst *obj, t_cub *cub)
{
	t_oinst	*prtl;
	t_oinst	*other;

	if (obj->isactive)
	{
		prtl = (t_oinst *)obj->relative;
		if (obj->counter > 600)
		{
			activate_portal((t_oinst *)obj->relative, 0);
			obj->isactive = 0;
			obj->special_gset.xwalls[0] = obj->gset->xwalls[0];
			obj->counter = 0;
		}
		++obj->counter;
	}
	if (obj->relative)
	{
		prtl = (t_oinst *)obj->relative;
		other = cub->objs.instances;
		while (other)
		{
			if (other->type->type_enum == OBJ_PLAYER
				&& other->cx == obj->cx && other->cy == obj->cy)
			{
				activate_portal(prtl, 1);
				obj->isactive = 1;
				obj->special_gset.xwalls[0] = obj->gset->xwalls[1];
				break ;
			}
			other = other->next;
		}
	}
	return (0);
}