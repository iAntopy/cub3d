/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:29:56 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 15:12:52 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	report_err(char *msg)
{
	ft_eprintf("Error\n\t- %s\n", msg);
	return (-1);
}

int	report_err_strerror(char *msg)
{
	ft_eprintf("Error\n\t- %s :: %s\n", msg, mlx_strerror(mlx_errno));
	return (-1);
}

int	report_mlx_init_error(void)
{
	ft_eprintf("Error\n\t- MLX42 init failure :: %s\n", mlx_strerror(mlx_errno));
	return (EXIT_FAILURE);
}

void	*report_mlx_tex_load_failed(char *tex)
{
	ft_eprintf("Error\n\t- Failed to load texture %s. MLX err {%s}\n",
		tex, mlx_strerror(mlx_errno));
	return (NULL);
}

int	report_malloc_error(void)
{
	ft_eprintf("Error\n\t- malloc failed.\n");
	return (-1);
}
