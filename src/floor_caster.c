/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_caster.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 03:58:14 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/15 16:38:39 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
/*
#undef SCN_WIDTH
#define SCN_WIDTH 20
#undef SCN_HEIGHT
#define SCN_HEIGHT 20
*/
/*
static void	print_floorcaster_params(t_cub *cub)
{
	int	i;
	int	j;
	float	*ps;

	ps = cub->renderer.param_factors;
	i = 0;
	while (++i < cub->scn_midy)
	{
		printf("[ ");
		j = -1;
		while (++j < SCN_WIDTH)
			printf("%.2f, ", ps[i * SCN_WIDTH + j]);
		printf("]\n");
	}
}

static void	print_near_z_dists(t_cub *cub)
{
	int	i;

	printf("[ ");
	i = -1;
	while (++i < cub->scn_midx)
		printf("%.2f, ", cub->renderer.near_z_dists[i]);
	i = -1;
	while (++i < cub->scn_midx)
		printf("%.2f, ", cub->renderer.near_z_dists[cub->scn_midx - i - 1]);
	printf("]\n");
}
*/
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
//	printf("trying to malloc param_factors size : %zu,
//		and near_z_dists size : %zu\n",
//		sizeof(float) * SCN_WIDTH * cub->scn_midy,
//		sizeof(float) * (SCN_WIDTH / 2));
	printf("init floorcaster entered \n");
	if (!ft_malloc_p(sizeof(float) * SCN_WIDTH * cub->scn_midy,
			(void **)&cub->renderer.floor_factors)
		|| !ft_malloc_p(sizeof(float) * (SCN_WIDTH / 2),
			(void **)&cub->renderer.near_z_dists))
		return (report_malloc_error());
	printf("init floorcaster SUCCESS\n");
	return (0);
}
/*
int	main()
{
	t_cub	cub;

	ft_memclear(&cub, sizeof(t_cub));
	cub.scn_midx = SCN_WIDTH / 2;
	cub.scn_midy = SCN_HEIGHT / 2;
	cub.near_z = (float)cub.scn_midx / tanf(0.5f * FOV);
//	printf("near_x : %f\n", cub.near_z);
	if (init_floorcaster(&cub) < 0)
		return (-1);
//	print_near_z_dists(&cub);
	print_floorcaster_params(&cub);
	return (0);
}
*/
