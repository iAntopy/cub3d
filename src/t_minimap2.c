/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 20:56:55 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/29 22:52:21 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	minimap_pixel_is_wall(t_cub *cub, int mx, int my)
{
	const int	mmp_world_scaler = CELL_WIDTH * 10.0f;
	int	        ps[2];
	int         cs[2];

	ps[0] = (cub->hero.ply_obj->px + ((mx - cub->map.mmp_mid[0]) / (float)MMP_WIDTH)
		* mmp_world_scaler);
	ps[1] = (cub->hero.ply_obj->py + ((my - cub->map.mmp_mid[1]) / (float)MMP_HEIGHT)
		* mmp_world_scaler);

	cs[0] = (int)(ps[0] * cub->inv_cw);
	cs[1] = (int)(ps[1] * cub->inv_cw);
//	if (cs[0] >= 0 && cs[1] >= 0 && cs[0] < cub->map.width && cs[1] < cub->map.height)
//		printf("map pix potential : ps (%d, %d), ms (%d, %d)\n", ps[0], ps[1], cs[0], cs[1]);
	
	return (cs[0] < 0 || cs[1] < 0
		|| cs[0] >= cub->map.width || cs[1] >= cub->map.height
		|| is_wall(&cub->map, cs[0], cs[1]));
}

#define MMP_FOV_LEN 50
void	minimap_draw_player_and_fov(t_cub *cub)
{
	const float	left_ray[2] = {*cub->hero.fov_lx, *cub->hero.fov_ly};
	const float	right_ray[2] = {*cub->hero.fov_rx, *cub->hero.fov_ry};
	int			left_ray_end[2] = {cub->map.mmp_mid[0] + left_ray[0] * MMP_FOV_LEN,
		cub->map.mmp_mid[1] + left_ray[1] * MMP_FOV_LEN};
	int			right_ray_end[2] = {cub->map.mmp_mid[0] + right_ray[0] * MMP_FOV_LEN,
		cub->map.mmp_mid[1] + right_ray[1] * MMP_FOV_LEN};
	int			pos[2] = {cub->map.mmp_mid[0], cub->map.mmp_mid[1]};
	
//	left_ray_end[0] += cub->map.mmp_mid[0];
//	left_ray_end[1] += cub->map.mmp_mid[1];
//	right_ray_end[0] += cub->map.mmp_mid[0];
//	right_ray_end[1] += cub->map.mmp_mid[1];
	mlx_draw_line(cub->renderer.mmap_layer, pos, left_ray_end, 0xff0000ff & TRANSPARENCY);
	mlx_draw_line(cub->renderer.mmap_layer, pos, right_ray_end, 0xff0000ff & TRANSPARENCY);
	mlx_draw_square(cub->renderer.mmap_layer, cub->map.mmp_ply_off, 10,
		0xff0000ff & TRANSPARENCY);
}

//mmap_layer
void    update_minimap(t_cub *cub)
{
	int 		mx;
	int			my;

	my = -1;
	while (++my < MMP_HEIGHT)
	{
		mx = -1;
		while (++mx < MMP_WIDTH)
		{
			if (minimap_pixel_is_wall(cub, mx, my))
				cub_put_pixel(cub->renderer.mmap_layer, mx, my, 0xff5c3110 & TRANSPARENCY);
			else
				cub_put_pixel(cub->renderer.mmap_layer, mx, my, 0xffffffff & TRANSPARENCY);
		}
	}
	minimap_draw_player_and_fov(cub);
}
