/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:39:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/02 01:27:35 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	print_rays(t_mtx *xs, t_mtx *ys)
{
	int	len;
	int	i;

	len = xs->shape[0];
	i = -1;
	while (++i < len)
	{
		printf("%d. ( %f, %f )\n", i, *(float *)mtx_index(xs, i, 0),
				*(float *)mtx_index(ys, i, 0));
	}
}

int	init_raycaster(t_cub *cub)
{
	t_hero	*hero;
	
	hero = &cub->hero;
	hero->thetas = mtx_linspace(hero->ori - FOV_HF, hero->ori + FOV_HF, SCN_WIDTH, 1);
	hero->rays[0] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	hero->rays[1] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	if (!hero->thetas)
		return (-1);
//	mtx_print(hero->thetas);
//	mtx_display_info(hero->thetas);

	mtx_cos(hero->thetas, hero->rays[0]);
	mtx_sin(hero->thetas, hero->rays[1]);

//	mtx_print(hero->rays[0]);
//	mtx_display_info(hero->rays[0]);
//	mtx_print(hero->rays[1]);
//	mtx_display_info(hero->rays[1]);
	print_rays(hero->rays[0], hero->rays[1]);
	return (0);
}
