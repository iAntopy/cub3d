/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:29:56 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/12 17:32:17 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	report_err(char *msg)
{
	ft_eprintf(msg);
	return (-1);
}

int	report_mlx_init_error(void)
{
	ft_eprintf("ERROR : MLX42 init failure : %s\n", mlx_strerror(mlx_errno));
	return (EXIT_FAILURE);
}

int	report_mlx_tex_load_failed(void)
{
//	ft_eprintf("ERROR : mlx texture failed to load.\n");
	ft_eprintf("%s\n", mlx_strerror(mlx_errno));
	return (-1);
}

int	report_malloc_error(void)
{
	ft_eprintf("ERROR : malloc failed.\n");
	return (-1);
}
