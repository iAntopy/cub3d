/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 20:56:55 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/04 22:35:07 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	minimap_pixel_is_wall(t_cub *cub, int *ms)
{
	int	        ps[2];
	int         cs[2];

	ps[0] = (cub->hero.ply_obj->px + (ms[0] - cub->map.mmp_mid[0]) * cub->map.map_to_world_x_rt);// / (float)MMP_WIDTH)
	ps[1] = (cub->hero.ply_obj->py + (ms[1] - cub->map.mmp_mid[1]) * cub->map.map_to_world_y_rt);// / (float)MMP_HEIGHT)
	cs[0] = (int)(ps[0] * cub->inv_cw);
	cs[1] = (int)(ps[1] * cub->inv_cw);
	return (cs[0] < 0 || cs[1] < 0
		|| cs[0] >= cub->map.width || cs[1] >= cub->map.height
		|| is_wall(&cub->map, cs[0], cs[1]));
}

static int	pick_icon_color_according_to_otype(t_oinst *obj)
{
	static const int	color_chart[7] = {0, 0xff25e807 & TRANSPARENCY,
		0xffb84028 & TRANSPARENCY,
		0xff1bf5ee & TRANSPARENCY, 0xff0c8bfa & TRANSPARENCY,
		0xff1535eb & TRANSPARENCY, 0xffebc315 & TRANSPARENCY};
	
	return (color_chart[obj->type->type_enum]);
}

static void	minimap_draw_objs(t_cub *cub)
{
	t_oinst		*obj;
	int			ms[2];
	int			icon_col;

	obj = cub->objs.instances;
	while (obj)
	{
		ms[0] = (int)((obj->px - cub->hero.ply_obj->px) * cub->map.world_to_map_x_rt) + cub->map.mmp_mid[0];
		ms[1] = (int)((obj->py - cub->hero.ply_obj->py) * cub->map.world_to_map_y_rt) + cub->map.mmp_mid[1];
		if (ms[0] < -5 || MMP_WIDTH + 5 <= ms[0] || ms[1] < -5 || MMP_HEIGHT + 5 <= ms[1])
		{
			obj = obj->next;
			continue ;
		}
		icon_col = pick_icon_color_according_to_otype(obj);
		ms[0] -= 5;
		ms[1] -= 5;
		mlx_draw_square(cub->renderer.mmap_layer, ms, 10, icon_col & TRANSPARENCY);
		obj = obj->next;
	}
}

static int	minimap_pixel_in_fov(t_cub *cub, int *ms, const float *left_r, const float *right_r)
{
	const int	to_mid[2] = {ms[0] - cub->map.mmp_mid[0], ms[1] - cub->map.mmp_mid[1]};
	const int	is_in_front = 0 < (to_mid[0] * (*cub->hero.dirx) + to_mid[1] * (*cub->hero.diry));
	int			scn_x;
	float		dist;
	float		ratio;
	
	if (is_in_front && (((to_mid[1] * left_r[0] - to_mid[0] * left_r[1]) > 0))
		!= ((to_mid[1] * right_r[0] - to_mid[0] * right_r[1]) > 0))
	{
		if (!to_mid[0] && !to_mid[1])
			return (0);

		dist = ((*cub->hero.dirx) * to_mid[0] + (*cub->hero.diry) * to_mid[1])
			* cub->map.map_to_world_x_rt;
		ratio = cub->near_z / dist;
		scn_x = (int)((((*cub->hero.dirx) * to_mid[1] * cub->map.map_to_world_x_rt)
			- (*cub->hero.diry) * to_mid[0] * cub->map.map_to_world_y_rt)
			* ratio) + cub->scn_midx;
		if (dist > cub->hero.rcast.rdata[scn_x].dist)
			return (0);
		return (1);
	}
	return (0);
}

/*
void	minimap_draw_player_and_fov(t_cub *cub, const float *left_r, const float *right_r)
{
	int		left_ray_end[2] = {cub->map.mmp_mid[0] + left_r[0] * MMP_FOV_LEN,
		cub->map.mmp_mid[1] + left_r[1] * MMP_FOV_LEN};
	int		right_ray_end[2] = {cub->map.mmp_mid[0] + right_r[0] * MMP_FOV_LEN,
		cub->map.mmp_mid[1] + right_r[1] * MMP_FOV_LEN};
	int		pos[2] = {cub->map.mmp_mid[0], cub->map.mmp_mid[1]};
	float	ratio;

	
	
//	left_ray_end[0] += cub->map.mmp_mid[0];
//	left_ray_end[1] += cub->map.mmp_mid[1];
//	right_ray_end[0] += cub->map.mmp_mid[0];
//	right_ray_end[1] += cub->map.mmp_mid[1];


//	mlx_draw_line(cub->renderer.mmap_layer, pos, left_ray_end, 0xff0000ff & TRANSPARENCY);
//	mlx_draw_line(cub->renderer.mmap_layer, pos, right_ray_end, 0xff0000ff & TRANSPARENCY);
	mlx_draw_square(cub->renderer.mmap_layer, cub->map.mmp_ply_off, 10,
		0xff0000ff & TRANSPARENCY);
}
*/
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
	minimap_draw_objs(cub);
}
