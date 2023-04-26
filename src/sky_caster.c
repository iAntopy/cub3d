/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sky_caster.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 02:30:02 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/25 21:48:11 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//const int	tex_col = (int)((x - cub->scn_midx) * cub->inv_sw * cub->skymap_fov_to_tex
//		+ cub->skymap_tex_offset) % cub->tex.skymap->width;
/*
int	clear_skycaster(t_cub *cub)
{
	ft_free_p((void **)&cub->renderer.sky_base_toffs);
	ft_free_p((void **)&cub->renderer.sky_toffs);
	return (0);
}

// Called on player rotate
void	update_sky_toffs(t_cub *cub, int *base_toffs, int *toffs)
{
	int	x;

//	printf("base toffs : %p, toffs : %p\n", base_toffs, toffs);
	x = -1;
	while (++x < SCN_WIDTH)
		*(toffs++) = (*(base_toffs++) + cub->renderer.sky_ori_offset)
			% cub->sky_tex->width;
}

// Called on fov change.
void	update_sky_base_toffs(t_cub *cub, int *base_toffs, int *toffs)
{
	int	x;

//	printf("sky base toffs update :: base toffs : %p, toffs : %p, tex : %p\n", base_toffs, toffs, cub->sky_tex);
	x = -1;
	while (++x < SCN_WIDTH)
	{
		*base_toffs = (int)((x - cub->scn_midx) * cub->inv_sw
			* cub->sky_fov_to_tex);
		*toffs = (*base_toffs + cub->renderer.sky_ori_offset)
			% cub->sky_tex->width;
		base_toffs++;
		toffs++;
	}
}


int	init_skycaster(t_cub *cub)
{
	if (!ft_malloc_p(sizeof(int) * SCN_WIDTH, (void **)&cub->renderer.sky_base_toffs)
		|| !ft_malloc_p(sizeof(int) * SCN_WIDTH, (void **)&cub->renderer.sky_toffs))
		return (report_malloc_error());
	printf("INIT toffs arrays :: base toffs : %p, toffs : %p\n", cub->renderer.sky_base_toffs,
		cub->renderer.sky_toffs);
	cub->sky_fov_to_tex = FOV60 * cub->sky_radial_width;
	update_sky_base_toffs(cub, cub->renderer.sky_base_toffs, cub->renderer.sky_toffs);
	return (0);
}
*/