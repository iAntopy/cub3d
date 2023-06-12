/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handler2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 17:26:54 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 17:28:37 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	on_close(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	mlx_close_window(cub->mlx);
}

void	on_scroll(double deltax, double deltay, void *param)
{
	t_cub	*cub;

	(void)deltax;
	cub = (t_cub *)param;
	if (deltax)
		cub_player_zoom(cub, deltax * 0.1f);
	else
		cub_player_zoom(cub, deltay * 0.1f);
}
