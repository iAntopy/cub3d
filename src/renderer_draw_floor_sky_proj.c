/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_floor_sky_proj.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 17:27:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/11 21:34:39 by iamongeo         ###   ########.fr       */
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

void	__render_proj_sky(t_cub *cub, uint32_t *pbuff, int *pframe, int width)
{
	int			texture_xoffsets[SCN_WIDTH];
	int			*tofs_p = texture_xoffsets + pframe[0] - 1;
//	uint32_t	*pxls;
	int			*tofs[2];
	int			x;
	int			y;
	float		*dbuff;
//	uint32_t	tex_col;
	uint32_t	*prtl_col_p;

	printf("__render_proj_sky : start pframe [%d, %d, %d, %d], width : %d\n", pframe[0], pframe[1], pframe[2], pframe[3], width);
	tofs[0] = tofs_p;
	x = pframe[0] - 1;//-1;
	while (++x < width)//SCN_WIDTH)
		*(++tofs[0]) = (int)((x - cub->scn_midx) * cub->inv_sw
				* cub->renderer.sky_fov_to_tex
				+ cub->renderer.sky_ori_offset) % cub->tex.skymap->width;
//	pxls = (uint32_t *)cub->renderer.objs_layer->pixels;
	tofs[1] = cub->renderer.sky_yoffsets + pframe[1] - 1;
	
	dbuff = cub->renderer.dpbuff;
	y = pframe[1] - 1;
	while (++y < cub->scn_midy)
	{
//		printf("y : %d\n", y);
		tofs[0] = tofs_p;
		++tofs[1];
		x = pframe[0] - 1;
		while (++x < width)//SCN_WIDTH)
		{
			prtl_col_p = pbuff + (x + y * SCN_WIDTH);
			if (*prtl_col_p == 0xffbcbbb0 && !dbuff[x * SCN_HEIGHT + y])
			{
				*prtl_col_p = ((uint32_t *)cub->tex.skymap->pixels)[*(++tofs[0])
					+ (*tofs[1]) * cub->tex.skymap->width];
			}
//			++pxls;
		}
	}
}
*/
void	__render_proj_sky(t_cub *cub, uint32_t *pbuff, int *pframe)//, int *pdims)
{
	int			toffs_x[SCN_WIDTH];
//	int			*tofs_p = texture_xoffsets + pframe[0] - 1;
//	uint32_t	*pxls;
//	int			*tofs[2];
	int			*sky_offy;
	int			i;
	int			j;
	float		*dpbuff;
	uint32_t	*pb;
	int			*tp;
	char		*isproj;
//	uint32_t	tex_col;
//	uint32_t	*prtl_col_p;

	printf("__render_proj_sky : start pframe [%d, %d, %d, %d]\n", pframe[0], pframe[1], pframe[2], pframe[3]);
//	tofs[0] = tofs_p;
	i = pframe[0] - 1;
//	tp = toffx_x + i;
	while (++i < pframe[2])//SCN_WIDTH)
		toffs_x[i] = (int)((i - cub->scn_midx) * cub->inv_sw
				* cub->renderer.sky_fov_to_tex
				+ cub->renderer.sky_ori_offset) % cub->tex.skymap->width;
//	pxls = (uint32_t *)cub->renderer.objs_layer->pixels;
	
	dpbuff = cub->renderer.dpbuff;
	j = pframe[1] - 1;
	while (++j < cub->scn_midy)
	{
//		tofs[0] = tofs_p;
//		printf("j : %d\n", j);
		sky_offy = cub->renderer.sky_yoffsets + j;
		dpbuff = cub->renderer.dpbuff + j * SCN_WIDTH;
		pb = pbuff + pframe[0] + j * SCN_WIDTH;
		tp = toffs_x + pframe[0];
		isproj = cub->renderer.isproj + j * SCN_WIDTH;
//		++tofs[1];
		i = pframe[0] - 1;
		while (++i < pframe[2])//SCN_WIDTH)
		{
//			prtl_col_p = pb + i;//pbuff + (x + y * SCN_WIDTH);
//			if (dpbuff[i * SCN_HEIGHT])// + y])
//				printf("OOPS ! dpbuff (%d, %d) : %f\n", i, j, dpbuff[i * SCN_HEIGHT]);
			if (isproj[i] && !dpbuff[i])// + y])
			{
	//			printf("tex_offx : %d, tex_offy : %d\n", *tp, *sky_offy);
				*pb = ((uint32_t *)cub->tex.skymap->pixels)[*tp//*(++tofs[0])
					+ (*sky_offy) * cub->tex.skymap->width] & 0xafffffff;
			}
//			++pxls;
//			++(toffs[0]);
			++tp;
			++pb;
		}
		++sky_offy;
	}
}

static void	__render_proj_floor_sky(t_cub *cub, t_pdata *pdata, uint32_t *pbuff, int *pframe)
{
//	const float	*rays[2];// = {pd[pframe[0]].rdata->rx, pd[pframe[0]].rdata->ry};
//	const int		pheight_inv = (float)(pframe[3] - pframe[1]);
//	float			*params = cub->renderer.floor_factors;// + pframe[0] - 1;
//	int				mids[2] = {(pframe[2] - pframe[0]) >> 1, (pframe[3] - pframe[1]) >> 1};
	int				i;
	int				j;
	float			p[2];
	int				c[2];
	float			t[2];
//	float		*pms;
//	uint32_t		*pb;
	float			*dpbuff;
	t_matrx			*pset;
	mlx_texture_t	*tex;
	t_pdata			*pd;
	char			*isproj;
//	float			divergent_lens_ratio;
	float			ray_scalar;
	
	printf("FLOOR SKY \n");

	__render_proj_sky(cub, pbuff, pframe);

//	printf("INIT pdata idx : %d\n", pdata->rdata->idx);
//	pdata += pframe[0] - 1;
	j = cub->scn_midy;
	while (++j < pframe[3])
	{
//		pms = params + (j - cub->scn_midy) * SCN_WIDTH;
//		printf("pms[(j (%d) - midy (%d)) (%d)] : %f\n", j, cub->scn_midy, (j - cub->scn_midy), *pms);
		dpbuff = cub->renderer.dpbuff + cub->buff_offys[j];//j * SCN_WIDTH;
		isproj = cub->renderer.isproj + cub->buff_offys[j];//j * SCN_WIDTH;
//		pb = pbuff + pframe[0] + j * SCN_WIDTH - 1;
		i = pframe[0] - 1;
//		divergent_lens_ratio = 
		pd = pdata + i;//pframe[0] - 1;
		while (++i < pframe[2])
		{
//			++pms;
			++pd;
			if (!isproj[i] || dpbuff[i])
				continue ;
//			printf("is proj and not depth buff\n");
//			divergent_lens_ratio = (j - cub->scn_midy) * pheight_inv;
//			ray_scalar = params[i + (j - cub->scn_midy) * SCN_WIDTH] - pd->dist;//(*pms) - pd->odist;
//				* cosf(divergent_lens_ratio * LENS_EFFECT_RAD) - pd->odist;
			ray_scalar = get_floorcaster_param(cub, i, j);// - pd->dist;

//			printf("ray_scalar : %f\n", ray_scalar);
//			ray_scalar = get_floorcaster_param(cub, i, j) - pd->odist;//(*pms) - pd->odist;
//			rays_scalar = (*pms) - pd->odist;
//			printf("i %d, pd id : %d, ray_scalar %f, odist %f, pd->px %f, pd->py %f\n",
//				i, pd->rdata->idx, ray_scalar, pd->odist, pd->px, pd->py);
			p[0] = *pd->rdata->rx * ray_scalar + pd->px;
			p[1] = *pd->rdata->ry * ray_scalar + pd->py;
//			printf("floor pos : (%f, %f)\n", p[0], p[1]);
//			p[0] = (*rays[0] * (*pms) + cub->hero.px);
//			p[1] = (*rays[1] * (*pms) + cub->hero.py);
//			printf("width_px : %d, height_px : %d\n", cub->map.width_px, cub->map.height_px);
			if (p[0] < 0.0f || cub->map.width_px <= p[0]
				|| p[1] < 0.0f || cub->map.height_px <= p[1])
				continue ;

			c[0] = (int)(p[0] * cub->inv_cw);
			c[1] = (int)(p[1] * cub->inv_cw);

//			printf("getting tex at cell (%d, %d)\n", c[0], c[1]);//, cub->map.mx[c[1]][c[0]]);
//			printf("px, py : (%f, %f)\n", p[0], p[1]);
//			printf("cx, cy : (%d, %d)\n", c[0], c[1]);
//			printf("mx %p\n", cub->map.mx);
//			printf("mx[0] %p\n", cub->map.mx[0]);
//			printf("mx[0][0] %p\n", cub->map.mx[0][0]);
			pset = cub->map.mx[c[1]][c[0]];
			
			if (!pset)
				continue ;
			tex = pset->xwalls[0];

			t[0] = p[0] - (c[0] * CELL_WIDTH);
			t[1] = p[1] - (c[1] * CELL_WIDTH);

			pbuff[i + j * SCN_WIDTH] = get_tex_pixel(tex, t[0] * tex->width * cub->inv_cw,
//			*pb = get_tex_pixel(tex, t[0] * tex->width * cub->inv_cw,
				t[1] * tex->height * cub->inv_cw) & 0xafffffff;
			
//			*buffs[0] = get_tex_pixel(tex_arr[0], mx * tex_arr[0]->width * cub->inv_cw,// flr_ratios[0],
//				my * tex_arr[0]->height * cub->inv_cw);//flr_ratios[1]);


		}
//		params += SCN_WIDTH;
//		++(rays[0]);
//		++(rays[1]);
	}
}

static void	__render_proj_floor_ceiling(t_cub *cub, t_pdata *pdata, uint32_t *pbuff, int *pframe)
{
//	const float	*rays[2];// = {pd[pframe[0]].rdata->rx, pd[pframe[0]].rdata->ry};
//	float			*params = cub->renderer.floor_factors;// + pframe[0] - 1;
//	int				mids[2] = {(pframe[2] - pframe[0]) >> 1, (pframe[3] - pframe[1]) >> 1};
	int				i;
	int				j;
	float			p[2];
	int				c[2];
	float			t[2];
//	float		*pms;
	uint32_t		*pf;
	uint32_t		*pc;
	float			*dpbuff_flr;
//	float			*dpbuff_cil;
	t_matrx			*pset;
	mlx_texture_t	*tex_flr;
	mlx_texture_t	*tex_cil;
	t_pdata			*pd;
	char			*isproj;
//	float			divergent_lens_ratio;

	float			ray_scalar;

//	printf("INIT pdata idx : %d\n", pdata->rdata->idx);
//	pdata += pframe[0] - 1;
//	printf("pframe mid color : %p\n", (void *)(size_t)pbuff[pframe[0] + ((pframe[2] - pframe[0]) >> 1) + (pframe[1] + 50) * SCN_WIDTH]);
	j = cub->scn_midy;
	while (++j < pframe[3])
	{
//		pms = params + (j - cub->scn_midy) * SCN_WIDTH;
//		printf("pms[(j (%d) - midy (%d)) (%d)] : %f\n", j, cub->scn_midy, (j - cub->scn_midy), *pms);
		dpbuff_flr = cub->renderer.dpbuff + cub->buff_offys[j];//j;
//		dpbuff_cil = cub->renderer.dpbuff + (SCN_HEIGHT - j);
		isproj = cub->renderer.isproj + cub->buff_offys[j];//j;
	//	printf("pframe[0] : %d, cil y %d, flr y %d\n", pframe[0], (SCN_HEIGHT - j), j);
		pf = pbuff + pframe[0] + cub->buff_offys[j] - 1;//j * SCN_WIDTH - 1;
//		pc = pbuff + pframe[0] + cub->buff_offys[SCN_HEIGHT - j] - 1;//(SCN_HEIGHT - j) * SCN_WIDTH - 1;
		i = pframe[0] - 1;
//		divergent_lens_ratio = 
		pd = pdata + i - 1;//pframe[0] - 1;
		while (++i < pframe[2])
		{
//			++pms;
			++pd;
			++pf;
//			++pc;
//			++isproj;
//			++dpbuff_flr;
//			if (!isproj[i * SCN_HEIGHT] || dpbuff_flr[i * SCN_HEIGHT])
			if (!isproj[i] || dpbuff_flr[i])
				continue ;
//			printf("wow");
//			divergent_lens_ratio = (j - cub->scn_midy) / (float)(pframe[3] - pframe[1]);
			ray_scalar = get_floorcaster_param(cub, i, (j - cub->scn_midy) * SCN_WIDTH) - pd->dist;

//			ray_scalar = params[i + (j - cub->scn_midy) * SCN_WIDTH] - pd->dist;//(*pms) - pd->odist;
//				* cosf(divergent_lens_ratio * LENS_EFFECT_RAD) - pd->odist;


//			ray_scalar = get_floorcaster_param(cub, i, j) - pd->odist;//(*pms) - pd->odist;
//			rays_scalar = (*pms) - pd->odist;
//			printf("i %d, pd id : %d, ray_scalar %f, odist %f, pd->px %f, pd->py %f\n",
//				i, pd->rdata->idx, ray_scalar, pd->odist, pd->px, pd->py);
			p[0] = *pd->rdata->rx * ray_scalar + pd->px;
			p[1] = *pd->rdata->ry * ray_scalar + pd->py;
			printf("floor pos : (%f, %f)\n", p[0], p[1]);
//			p[0] = (*rays[0] * (*pms) + cub->hero.px);
//			p[1] = (*rays[1] * (*pms) + cub->hero.py);
//			printf("width_px : %d, height_px : %d\n", cub->map.width_px, cub->map.height_px);
			if (p[0] < 0.0f || cub->map.width_px <= p[0]
				|| p[1] < 0.0f || cub->map.height_px <= p[1])
				continue ;
//			printf("p in bound\n");
			c[0] = (int)(p[0] * cub->inv_cw);
			c[1] = (int)(p[1] * cub->inv_cw);

//			printf("getting tex at cell (%d, %d)\n", c[0], c[1]);//, cub->map.mx[c[1]][c[0]]);
			pset = cub->map.mx[c[1]][c[0]];
			
			if (!pset)
				continue ;
			tex_flr = pset->xwalls[0];
			tex_cil = pset->xwalls[1];

			t[0] = p[0] - (c[0] * CELL_WIDTH);
			t[1] = p[1] - (c[1] * CELL_WIDTH);

//			printf("x : %d, *pf : %p, *pc : %p, pc raw offset : %ld\n", i, (void *)(size_t)*pf, (void *)(size_t)*pc, pc - pbuff);
			*pf = get_tex_pixel(tex_flr, t[0] * tex_flr->width * cub->inv_cw,
				t[1] * tex_flr->height * cub->inv_cw) & 0xafffffff;
			printf("*pf : %p, ", (void*)(size_t)*pf);

//			pc = pf - (2 * (j - cub->scn_midy) * SCN_WIDTH);
			pc = pbuff + i + (SCN_WIDTH * (SCN_HEIGHT - j));
//			if ((SCN_HEIGHT - j) > 200 && i < pframe[2] - 100)
//				printf("pc x : %ld\n", (size_t)pc % SCN_WIDTH);
//				printf("x, y : (%d, %d). *pc (%p) vs PROJ_COLOR (%p), !dpbuff_cil[i * SCN_HEIGHT] ? %d\n", i, j,
//					(void *)(size_t)*pc, (void *)(size_t)0xffbcbbb0, !dpbuff_cil[i * SCN_HEIGHT]);
//			if (*pc == 0xffbcbbb0 && !dpbuff_cil[i * SCN_HEIGHT])
//			{
//				printf("drawing ");
			*pc = get_tex_pixel(tex_cil, t[0] * tex_cil->width * cub->inv_cw,
				t[1] * tex_cil->height * cub->inv_cw) & 0xafffffff;
//			}
		}
	}
}
/*
static void	__render_floor_sky(t_cub *cub, t_pdata *pd, int *pframe)
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

	__render_proj_sky(cub, pframe, pframe[2] - pframe[0]);
	buffs[0];
//	__render_floor_init(cub, buffs, &params);
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
			*buffs[0] = get_tex_pixel(tex_arr[0], mx * tex_arr[0]->width * cub->inv_cw,// flr_ratios[0],
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
*/
/*
static void	__render_floor_ceiling(t_cub *cub, t_pdata *pd, int *pframe)
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
//	printf("renderering floor and ceiling \n");
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
				mx * tex_arr[0]->width * cub->inv_cw,// flr_ratios[0],
				my * tex_arr[0]->height * cub->inv_cw);//flr_ratios[1]);
			*buffs[2] = get_tex_pixel(tex_arr[1],
				mx * tex_arr[1]->width * cub->inv_cw,// flr_ratios[0],
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
*/
void	render_floor_sky_proj(t_cub *cub, uint32_t *pbuff, t_pdata *pd, int *pframe)
{
//	(void)pd;
//	printf("render_floor_sky_proj : start. Open sky ? %d\n", cub->tex.open_sky);
	if (cub->tex.open_sky)
//		__render_proj_sky(cub, pbuff, pframe);//, pframe[2] - pframe[0]);
		__render_proj_floor_sky(cub, pd, pbuff, pframe);
	else
		__render_proj_floor_ceiling(cub, pd, pbuff, pframe);
}
/*
void	render_floor_sky_proj(t_cub *cub, uint32_t *pbuff, t_pdata *pd, int *pframe)
{
	(void)pd;
	printf("render_floor_sky_proj : start. Open sky ? %d\n", cub->tex.open_sky);
	if (cub->tex.open_sky)
		__render_proj_sky(cub, pbuff, pframe, pframe[2] - pframe[0]);
		__render_floor_sky(cub, pd, pframe);
//	else
//		__render_floor_ceiling(cub, pd, pframe);
}
*/