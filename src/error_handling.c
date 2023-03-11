/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:29:56 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/04 05:25:39 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	report_mlx_init_error(void)
{
	ft_eprintf("ERROR : mlx library failed to initialize.\n");
	return (-1);
}

int	report_malloc_error(void)
{
	ft_eprintf("ERROR : malloc failed.\n");
	return (-1);
}
