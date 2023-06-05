/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_linkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 15:58:59 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	link_portal_instances(t_oinst *prtl1, t_oinst *prtl2)
{
	printf("link_portal_instances : prtl1 type %d, prtl2 type %d\n",
		prtl1->type->type_enum, prtl2->type->type_enum);
	if (!prtl1 || !prtl2
		|| (prtl1->type->type_enum != OBJ_PORTAL)
		|| (prtl2->type->type_enum != OBJ_PORTAL))
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
		|| (lever->type->type_enum != OBJ_LEVER)
		|| (prtl->type->type_enum != OBJ_PORTAL))
		return (report_err("Either lever is not an OBJ_LEVER\
 or prtl not a OBJ_PORTAL.\n"));
//	if (lever->alleg != prtl->alleg)
//		return (report_err("Can't link lever to portal of differente team.\n"));
	lever->relative = prtl;
	return (0);
}

int	link_fireball_to_target(t_oinst *fball, t_oinst *target)
{
	printf("Linking fireball to target. fball : %p, target : %p, fball type enum : %d\n", fball, target, fball->type->type_enum);
	if (!fball || !target
		|| (fball->type->type_enum != OBJ_FIREBALL))
		return (report_err("fball is not a OBJ_FIREBALL.\n"));
	if (fball->alleg == target->alleg)
		return (report_err("Cannot link fireball to player of same team.\n"));
	fball->relative = target;
	fball->isactive = 1;
	return (0);
}

int	link_firepit_to_target(t_oinst *fpit, t_oinst *target)
{
	printf("Linking firepit to target. fpit : %p, target : %p, fpit type enum : %d\n", fpit, target, fpit->type->type_enum);
	if (!fpit || !target
		|| (fpit->type->type_enum != OBJ_FIREPIT))
		return (report_err("fpit is not a OBJ_FIREPIT.\n"));
	if (fpit->alleg == target->alleg)
		return (report_err("Cannot link firepit to player of same team.\n"));
	fpit->relative = target;
	fpit->isactive = 1;
	return (0);
}
