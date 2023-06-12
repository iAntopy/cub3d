/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_caster.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 03:58:14 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/02 17:51:22 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	clear_floorcaster(t_cub *cub)
{
	if (cub->renderer.near_z_dists)
		ft_free_p((void **)&cub->renderer.near_z_dists);
	if (cub->renderer.floor_factors)
		ft_free_p((void **)&cub->renderer.floor_factors);
	return (0);
}

// Requires malloced cub->renderer.near_z_dists
void	update_near_z_dists(t_cub *cub)
{
	int			i;
	float		*nz;
	const int	half_w = cub->scn_midx;

	nz = cub->renderer.near_z_dists;
	printf("update_near_z : nz ptr : %p\n", nz);
	i = -1;
	while (++i < half_w)
		nz[i] = sqrtf(cub->near_z * cub->near_z + (half_w - i) * (half_w - i));
}

// y value must be in the bottom half of the window exclusively
// (y MUST NOT == scn_midy)
float	get_floorcaster_param(t_cub *cub, int x, int y)
{
	return (cub->renderer.floor_factors[((y - cub->scn_midy) * SCN_WIDTH) + x]);
}

// i should start at 1. First row will all be div/0.
void	update_floorcaster_params(t_cub *cub)
{
	int			i;
	int			j;
	const int	half_w = cub->scn_midx;
	float		*ps;
	float		factor;

	if (!cub->renderer.floor_factors)
		init_floorcaster(cub);
	printf("UPDATE FLOORCASTER\n");
	update_near_z_dists(cub);
	ps = cub->renderer.floor_factors;
	i = 0;
	while (++i < cub->scn_midy)
	{
		j = -1;
		while (++j < half_w)
		{
			factor = PLAYER_HEIGHT
				/ (sinf(atanf(i / cub->renderer.near_z_dists[j])));
			ps[i * SCN_WIDTH + j] = factor;
			ps[i * SCN_WIDTH + (SCN_WIDTH - j - 1)] = factor;
		}
	}
}

int	init_floorcaster(t_cub *cub)
{
	printf("INIT FLOORCASTER \n");
	if (cub->renderer.floor_factors)
		return (0);
	if (!ft_malloc_p(sizeof(float) * SCN_WIDTH * cub->scn_midy,
			(void **)&cub->renderer.floor_factors)
		|| !ft_malloc_p(sizeof(float) * (SCN_WIDTH / 2),
			(void **)&cub->renderer.near_z_dists))
		return (report_malloc_error());
	return (0);
}
