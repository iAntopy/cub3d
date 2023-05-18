/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 20:37:48 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 20:50:07 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	report_threads_err(t_thdraw *threads, char *err, int print_strerr)
{
	stop_draw_threads(threads);
	if (print_strerr)
		fperror("Error\n\t - %s", err);
	else
		report_err(err);
	return (-1);
}

int	report_obj_err(t_oinst *obj, char *msg)
{
	ft_eprintf("ERROR with obj %s <id %d, ptr %p> : %s\n", 
		obj->type->model_name, obj->_id, obj, msg);
	return (-1);
}