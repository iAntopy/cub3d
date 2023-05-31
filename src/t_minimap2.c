/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_minimap2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 20:56:55 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/30 22:43:29 by iamongeo         ###   ########.fr       */
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
		//if (obj->type->type_enum == OBJ_PORTAL)
		icon_col = 0xff00ff00;
//		printf("obj %d: (%d, %d)\n", obj->_id, ms[0], ms[1]);
		ms[0] -= 5;//ft_clamp(ms[0] - 5, 0, MMP_WIDTH);
		ms[1] -= 5;//ft_clamp(ms[1] - 5, 0, MMP_WIDTH);
		mlx_draw_square(cub->renderer.mmap_layer, ms, 10, icon_col & TRANSPARENCY);
		obj = obj->next;
	}
}

/*
#define MMP_FOV_LEN 65
// ms : map coords (x, y)
// ms_to_mid is perpendicular vector from normal
static int	minimap_pixel_in_fov(t_cub *cub, int *ms, const float *left_r, const float *right_r)
{
	const int	to_mid[2] = {ms[0] - cub->map.mmp_mid[0], ms[1] - cub->map.mmp_mid[1]};
	const int	is_in_front = 0 < (to_mid[0] * (*cub->hero.dirx) + to_mid[1] * (*cub->hero.diry));
	float			proj_len;
	float		mmp_len;
	float		theta;
	int			scn_x;
	
//	printf("dot left : %f, dot right : %f\n", (to_mid[1] * left_r[0] - to_mid[0] * left_r[1]), 
//		(to_mid[1] * right_r[0] - to_mid[0] * right_r[1]));
	if (is_in_front && (((to_mid[1] * left_r[0] - to_mid[0] * left_r[1]) > 0))
		!= ((to_mid[1] * right_r[0] - to_mid[0] * right_r[1]) > 0))
	{
		if (!to_mid[0] && !to_mid[1])
			return (0);
		
//		theta = atan2f(delta[1], delta[0]) - cub->hero.ply_obj->ori;
//		if (theta < -M_PI)
//			theta += M_TAU;
//		scn_x = (int)((theta / cub->fov) * SCN_WIDTH + cub->scn_midx);
//		if (theta < 0.0f)
//			theta += M_TAU;
//		printf("rel angle : %f\n", theta);//, scn_x);
		proj_len = to_mid[1] * (*cub->hero.dirx) - to_mid[0] * (*cub->hero.diry);
		mmp_len = sqrtf(to_mid[0] * to_mid[0] + to_mid[1] * to_mid[1]);
		theta = asinf(proj_len / mmp_len);
		scn_x = (int)(theta / cub->fov * SCN_WIDTH) + cub->scn_midx;
		if (mmp_len * cub->map.map_to_world_x_rt > cub->hero.rcast.rdata[scn_x].dist)
			return (0);

		return (1);
	}
	return (0);
}
*/
static int	minimap_pixel_in_fov(t_cub *cub, int *ms, const float *left_r, const float *right_r)
{
	const int	to_mid[2] = {ms[0] - cub->map.mmp_mid[0], ms[1] - cub->map.mmp_mid[1]};
	const int	is_in_front = 0 < (to_mid[0] * (*cub->hero.dirx) + to_mid[1] * (*cub->hero.diry));
//	float			proj_len;
//	float		mmp_len;
//	float		theta;
	int			scn_x;
	float		dist;
	float		ratio;
	
//	printf("dot left : %f, dot right : %f\n", (to_mid[1] * left_r[0] - to_mid[0] * left_r[1]), 
//		(to_mid[1] * right_r[0] - to_mid[0] * right_r[1]));
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
