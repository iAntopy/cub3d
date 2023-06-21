/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_deleters_and_getters2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 19:14:00 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/19 19:14:21 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	delete_all_scheduled_for_deletion(t_cub *cub)
{
	t_oinst	*elem;
	t_oinst	*tmp;

	if (!cub->objs.instances)
		return ;
	elem = cub->objs.instances;
	while (elem->next)
	{
		if (elem->next->scheduled_for_deletion)
		{
			tmp = elem->next;
			elem->next = tmp->next;
			ft_free_p((void **)&tmp);
		}
		else
			elem = elem->next;
	}
	elem = cub->objs.instances;
	if (elem->scheduled_for_deletion)
	{
		tmp = elem->next;
		ft_free_p((void **)&elem);
		cub->objs.instances = tmp;
	}
}

int	obj_schedule_for_deletion(t_oinst *obj)
{
	if (!obj)
		return (-1);
	obj->scheduled_for_deletion = 1;
	return (1);
}
