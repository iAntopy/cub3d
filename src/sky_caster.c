/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sky_caster.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 02:30:02 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/15 00:21:58 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_skycaster(t_cub *cub)
{
	int	i;
	
	if (!cub->box.sky)
		return (report_err("No sky texture provided.\n"));
	printf("sky textures LOADED\n");
	cub->tex.skymap = cub->box.sky;
	printf("sky ptr : %p\n", cub->tex.skymap);

	// cub->tex.open_sky = 1;
	// MOVE THESE SOMEWHERE !
//	cub->renderer.flrw_to_cw = cub->inv_cw * cub->floor_tex->width;
//	cub->renderer.flrh_to_cw = cub->inv_cw * cub->floor_tex->height;
	cub->renderer.sky_radial_width = cub->tex.skymap->width * cub->inv_two_pi;
	cub->renderer.sky_fov_to_tex = FOV60 * cub->renderer.sky_radial_width;
	cub->renderer.sky_ht_to_midy = cub->tex.skymap->height / ((float)SCN_HEIGHT * 0.666f);
	cub->renderer.sky_ori_offset = (int)(cub->hero.ori * cub->renderer.sky_radial_width);
	i = -1;
	while (++i < cub->scn_midy)
		cub->renderer.sky_yoffsets[i] = (int)(i * cub->renderer.sky_ht_to_midy);
	return (0);
}
