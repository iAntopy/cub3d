/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_linkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 21:24:56 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	link_portal_instances(t_oinst *prtl1, t_oinst *prtl2)
{
	if (!prtl1 || !prtl2
		|| (obj_get_type(prtl1) != OBJ_PORTAL)
		|| (obj_get_type(prtl2) != OBJ_PORTAL))
		return (report_err("Can't link non portals with this function.\n"));
	if (prtl1->alleg != prtl2->alleg)
		return (report_err("Can't link portals of differente team.\n"));
	prtl1->relative = prtl2;
	if (prtl2->relative == prtl2)
		prtl2->relative = prtl1;
	return (0);
}

int	link_lever_to_portal(t_oinst *lever, t_oinst *prtl)
{
	if (!lever || !prtl
		|| (obj_get_type(lever) != OBJ_LEVER)
		|| (obj_get_type(prtl) != OBJ_PORTAL))
		return (report_err("Either lever is not an OBJ_LEVER\
 or prtl not a OBJ_PORTAL.\n"));
	lever->relative = prtl;
	return (0);
}

int	link_fireball_to_target(t_oinst *fball, t_oinst *target)
{
	if (!fball || !target || fball->relative == target
		|| (obj_get_type(fball) != OBJ_FIREBALL))
		return (report_err("fball is not a OBJ_FIREBALL.\n"));
	if (fball->alleg == target->alleg)
		return (report_err("Cannot link fireball to player of same team.\n"));
	fball->relative = target;
	fball->isactive = 1;
	return (0);
}

int	link_firepit_to_target(t_oinst *fpit, t_oinst *target)
{
	if (!fpit || !target
		|| (obj_get_type(fpit) != OBJ_FIREPIT))
		return (report_err("fpit is not a OBJ_FIREPIT.\n"));
	if (fpit->alleg == target->alleg)
		return (report_err("Cannot link firepit to player of same team.\n"));
	fpit->relative = target;
	fpit->isactive = 1;
	return (0);
}
