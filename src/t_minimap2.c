/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 20:56:55 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/30 18:44:45 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	minimap_pixel_is_wall(t_cub *cub, int *ms)
{
//	const int	mmp_world_scaler = CELL_WIDTH * 10.0f;
	int	        ps[2];
	int         cs[2];

	ps[0] = (cub->hero.ply_obj->px + (ms[0] - cub->map.mmp_mid[0]) * cub->map.map_to_world_x_rt);// / (float)MMP_WIDTH)
//		* mmp_world_scaler);
	ps[1] = (cub->hero.ply_obj->py + (ms[1] - cub->map.mmp_mid[1]) * cub->map.map_to_world_y_rt);// / (float)MMP_HEIGHT)
//		* mmp_world_scaler);

	cs[0] = (int)(ps[0] * cub->inv_cw);
	cs[1] = (int)(ps[1] * cub->inv_cw);
//	if (cs[0] >= 0 && cs[1] >= 0 && cs[0] < cub->map.width && cs[1] < cub->map.height)
//		printf("map pix potential : ps (%d, %d), ms (%d, %d)\n", ps[0], ps[1], cs[0], cs[1]);
	
	return (cs[0] < 0 || cs[1] < 0
		|| cs[0] >= cub->map.width || cs[1] >= cub->map.height
		|| is_wall(&cub->map, cs[0], cs[1]));
}
/*
static minimap_draw_objs(t_cub *cub)
{
	const int	mmp_world_scaler = CELL_WIDTH * 10.0f;
	t_oinst		*obj;
	int			ms[2];
	

	obj = cub->objs.instances;
	while (obj)
	{
		ms[0] = (obj->px - cub->hero.ply_obj->px + cub->map.mmp_mid[0]) / mmp_world_scaler * ;
		ms[1] = obj->py - cub->hero.ply_obj->py + cub->map.mmp_mid[1];
		
	}
}
*/
#define MMP_FOV_LEN 65
// ms : map coords (x, y)
// ms_to_mid is perpendicular vector from normal
static int	minimap_pixel_in_fov(t_cub *cub, int *ms, const float *left_r, const float *right_r)
{
	const int	to_mid[2] = {ms[0] - cub->map.mmp_mid[0], ms[1] - cub->map.mmp_mid[1]};
	const int	is_in_front = 0 < (to_mid[0] * (*cub->hero.dirx) + to_mid[1] * (*cub->hero.diry));
	
//	printf("dot left : %f, dot right : %f\n", (to_mid[1] * left_r[0] - to_mid[0] * left_r[1]), 
//		(to_mid[1] * right_r[0] - to_mid[0] * right_r[1]));
	return (is_in_front && (((to_mid[1] * left_r[0] - to_mid[0] * left_r[1]) > 0))
		!= ((to_mid[1] * right_r[0] - to_mid[0] * right_r[1]) > 0)
		&& sqrtf(to_mid[0] * to_mid[0] + to_mid[1] * to_mid[1]) < MMP_FOV_LEN);
}

void	minimap_draw_player_and_fov(t_cub *cub, const float *left_r, const float *right_r)
{
	int	left_ray_end[2] = {cub->map.mmp_mid[0] + left_r[0] * MMP_FOV_LEN,
		cub->map.mmp_mid[1] + left_r[1] * MMP_FOV_LEN};
	int	right_ray_end[2] = {cub->map.mmp_mid[0] + right_r[0] * MMP_FOV_LEN,
		cub->map.mmp_mid[1] + right_r[1] * MMP_FOV_LEN};
	int	pos[2] = {cub->map.mmp_mid[0], cub->map.mmp_mid[1]};
	
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
	const float	left_ray[2] = {*cub->hero.fov_lx, *cub->hero.fov_ly};
	const float	right_ray[2] = {*cub->hero.fov_rx, *cub->hero.fov_ry};
	const int	colors[2][2] = {{0xff5c3110  & TRANSPARENCY,
		0xff5c3110 & 0xff7f7fff & TRANSPARENCY},
		{0xffffffff & TRANSPARENCY, 0xff7f7fff & TRANSPARENCY}};
	int 		mx;
	int			my;
	int			ms[2];
	int			is_in_fov;

	my = -1;
	while (++my < MMP_HEIGHT)
	{
		mx = -1;
		while (++mx < MMP_WIDTH)
		{
			ms[0] = mx;
			ms[1] = my;
			is_in_fov = minimap_pixel_in_fov(cub, ms, left_ray, right_ray);
			if (minimap_pixel_is_wall(cub, ms))
				cub_put_pixel(cub->renderer.mmap_layer, mx, my, colors[0][is_in_fov]);//(0xff5c3110 & (0xff7f7fff * is_in_fov)) & TRANSPARENCY);
			else
				cub_put_pixel(cub->renderer.mmap_layer, mx, my, colors[1][is_in_fov]);//(0xffffffff & (0xff7f7fff * is_in_fov)) & TRANSPARENCY);
		}
	}
//	minimap_draw_player_and_fov(cub, left_ray, right_ray);
	mlx_draw_square(cub->renderer.mmap_layer, cub->map.mmp_ply_off, 10,
		0xff0000ff & TRANSPARENCY);
}
