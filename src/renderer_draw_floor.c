/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_floor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 17:27:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/13 20:34:39 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
/*
static uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y)
{
//	if (x < 0 || y < 0)
//		return (0);
//	printf("px: x %d, y %d", x, y);
	return (((uint32_t *)tex->pixels)[x + y * tex->width]);
}
*/
static void	__render_floor_init(t_cub *cub, uint32_t **b, float **p)
{
	*p = cub->renderer.floor_factors - 1;
	b[0] = (uint32_t *)cub->renderer.bg_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy + 1)) - 1;
	b[1] = (uint32_t *)cub->renderer.walls_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy + 1)) - 1;
}

static void	__render_ceiling_init(t_cub *cub, uint32_t **b, float **p)
{
	__render_floor_init(cub, b, p);
	b[2] = (uint32_t *)cub->renderer.bg_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy - 2)) - 1;
}

static void	__render_floor_incr(float **ps, const float **rays, uint32_t **bs)
{
//	static int	i;

//	printf("i : %d ", i++);
	++(*ps);
	++rays[0];
	++rays[1];
	++(bs[0]);
	++(bs[1]);
}

static void	__render_flr_cil_incr(float **ps, const float **rays, uint32_t **bs)
{
	++(*ps);
	++rays[0];
	++rays[1];
	++(bs[0]);
	++(bs[1]);
	++(bs[2]);
	++(bs[3]);
}

void	__render_sky(t_cub *cub, t_rdata *rd)
{
	int			texture_xoffsets[SCN_WIDTH];
	uint32_t	*pxls;
	int			*tofs[2];
	int			x;
	int			y;

	(void)rd;
	tofs[0] = texture_xoffsets - 1;
	x = -1;
	while (++x < SCN_WIDTH)
		*(++tofs[0]) = (int)((x - cub->scn_midx) * cub->inv_sw
				* cub->renderer.sky_fov_to_tex
				+ cub->renderer.sky_ori_offset) % cub->tex.skymap->width;
	pxls = (uint32_t *)cub->renderer.bg_layer->pixels;
	tofs[1] = cub->renderer.sky_yoffsets - 1;
	y = -1;
	while (++y < cub->scn_midy)
	{
		tofs[0] = texture_xoffsets - 1;
		++tofs[1];
		x = -1;
		while (++x < SCN_WIDTH)
			*(++pxls) = ((uint32_t *)cub->tex.skymap->pixels)[*(++tofs[0])
				+ (*tofs[1]) * cub->tex.skymap->width];
	}
}

static void	__render_floor_sky(t_cub *cub, t_rdata *rd)
{
//	const float		flr_ratios[2] = {};//{cub->renderer.flrw_to_cw, cub->renderer.flrh_to_cw};
	const float		*rays[2] = {rd[0].rx, rd[0].ry};
	float			*params;
	uint32_t		*buffs[3];
	int				incr[2];
	int				cx;
	int				cy;
	float			mx;
	float			my;
	float			x;
	float			y;
	mlx_texture_t	**tex_arr;

	__render_sky(cub, rd);
	__render_floor_init(cub, buffs, &params);
	incr[1] = 0;
	while (++incr[1] < cub->scn_midy)
	{
		incr[0] = -1;
		while (++incr[0] < SCN_WIDTH)
		{
			__render_floor_incr(&params, rays, buffs);
//			printf("*rays[0] %f, ", *rays[0]);
//			printf("*params %f\n", *params);
			x = *rays[0] * (*params) + cub->hero.px;
			y = *rays[1] * (*params) + cub->hero.py;
	//		printf("x, y : %f, %f\n", x, y);
//			printf("cx, cy : %d, %d\n", cx, cy);
			if (*buffs[1] || x < 0.0f || y < 0.0f)
//				|| x >= cub->map.width || y >= cub->map.height)
				continue ;
			cx = (int)(x * cub->inv_cw);//fmodf(x, CELL_WIDTH);
			cy = (int)(y * cub->inv_cw);//fmodf(y, CELL_WIDTH);
			if (cx >= cub->map.width || cy >= cub->map.height)
				continue ;
			mx = x - (cx * CELL_WIDTH);//fmodf(x, CELL_WIDTH);//x - cx;
			my = y - (cy * CELL_WIDTH);//fmodf(y, CELL_WIDTH);//y - cy;
			tex_arr = cub->map.mx[cy][cx]->xwalls;
//			printf("tex_arr : %p, mx %f, my %f\n", tex_arr, mx, my);
			if (!tex_arr)// || tex_arr[1] != NULL)
				continue ;
	//		printf("drawing on mx %f, my %f\n", mx, my);
			*buffs[0] = get_tex_pixel(tex_arr[0], mx * tex_arr[0]->width * cub->inv_cw,//* flr_ratios[0],
				my * tex_arr[0]->height * cub->inv_cw);//flr_ratios[1]);
//			*buffs[0] = floor_get_pixel(cub->floor_tex,
//					(int)(fmodf(*rays[0] * (*params) + cub->hero.px, CELL_WIDTH)
//						* flr_ratios[0]),
//					(int)(fmodf(*rays[1] * (*params) + cub->hero.py, CELL_WIDTH)
//						* flr_ratios[1]));
		}
		rays[0] = rd[0].rx;
		rays[1] = rd[0].ry;
	}
}

static void	__render_floor_ceiling(t_cub *cub, t_rdata *rd)
{
	const float		*rays[2] = {rd[0].rx, rd[0].ry};
	float			*params;
	uint32_t		*buffs[4];
	int				incr[2];
	int				cx;
	int				cy;
	float			mx;
	float			my;
	float			x;
	float			y;
	mlx_texture_t	**tex_arr;

//	__render_floor_init(cub, buffs, &params);
	printf("renderering floor and ceiling \n");
	__render_ceiling_init(cub, buffs, &params);
	incr[1] = 0;
	while (++incr[1] < cub->scn_midy)
	{
		incr[0] = -1;
		while (++incr[0] < SCN_WIDTH)
		{
			__render_flr_cil_incr(&params, rays, buffs);
			x = *rays[0] * (*params) + cub->hero.px;
			y = *rays[1] * (*params) + cub->hero.py;
	//		printf("x, y : %f, %f\n", x, y);
//			printf("cx, cy : %d, %d\n", cx, cy);
			if (*buffs[1] || x < 0.0f || y < 0.0f)
//				|| x >= cub->map.width || y >= cub->map.height)
				continue ;
			cx = (int)(x * cub->inv_cw);//fmodf(x, CELL_WIDTH);
			cy = (int)(y * cub->inv_cw);//fmodf(y, CELL_WIDTH);
			if (cx >= cub->map.width || cy >= cub->map.height)
				continue ;			
			mx = x - (cx * CELL_WIDTH);//fmodf(x, CELL_WIDTH);//x - cx;
			my = y - (cy * CELL_WIDTH);//fmodf(y, CELL_WIDTH);//y - cy;
			tex_arr = cub->map.mx[cy][cx]->xwalls;
		//	printf("tex_arr : %p, mx %f, my %f\n", tex_arr, mx, my);
			if (!tex_arr)// || tex_arr[2] != NULL)
				continue ;
		//	printf("drawing on mx %f, my %f\n", mx, my);
			*buffs[0] = get_tex_pixel(tex_arr[0],
				mx * tex_arr[0]->width * cub->inv_cw,//* flr_ratios[0],
				my * tex_arr[0]->height * cub->inv_cw);//flr_ratios[1]);
			*buffs[2] = get_tex_pixel(tex_arr[1],
				mx * tex_arr[1]->width * cub->inv_cw,//* flr_ratios[0],
				my * tex_arr[1]->height * cub->inv_cw);//flr_ratios[1]);
//			*buffs[0] = floor_get_pixel(cub->floor_tex,
//					(int)(fmodf(*rays[0] * (*params) + cub->hero.px, CELL_WIDTH)
//						* flr_ratios[0]),
//					(int)(fmodf(*rays[1] * (*params) + cub->hero.py, CELL_WIDTH)
//						* flr_ratios[1]));
		}
		rays[0] = rd[0].rx;
		rays[1] = rd[0].ry;
		buffs[2] -= 2 * SCN_WIDTH;
	}
}

void	render_floor_sky(t_cub *cub)//, t_rdata *rd)
{
	if (cub->tex.open_sky)
		__render_floor_sky(cub, cub->hero.rcast.rdata);// rd);
	else
		__render_floor_ceiling(cub, cub->hero.rcast.rdata);//;rd);
}
