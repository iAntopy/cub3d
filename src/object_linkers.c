/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_linkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 20:43:41 by iamongeo         ###   ########.fr       */
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
	if (prtl1->allegiance != prtl2->allegiance)
		return (report_err("Can't link portals of differente team.\n"));
	prtl1->relative = prtl2;
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
	if (lever->allegiance != prtl->allegiance)
		return (report_err("Can't link lever to portal of differente team.\n"));
	lever->relative = prtl;
	return (0);
}

int	link_fireball_to_player(t_oinst *fball, t_hero *player)
{
	if (!fball || !player
		|| (fball->type->type_enum != OBJ_FIREBALL))
		return (report_err("fball is not a OBJ_FIREBALL.\n"));
	if (fball->allegiance == player->allegiance)
		return (report_err("Cannot link fireball to player of same team.\n"));
	fball->relative = player;
	return (0);
}

int	link_firepit_to_player(t_oinst *fpit, t_hero *player)
{
	if (!fpit || !player
		|| (fpit->type->type_enum != OBJ_FIREPIT))
		return (report_err("fpit is not a OBJ_FIREPIT.\n"));
	if (fpit->allegiance == player->allegiance)
		return (report_err("Cannot link firepit to player of same team.\n"));
	fpit->relative = player;
	return (0);
}
