/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/08 11:54:36 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	renderer_clear(t_cub *cub)
{
	printf("renderer clear entered \n");
	if (cub->renderer.bg_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
	if (cub->renderer.walls_layer)
		mlx_delete_image(cub->mlx, cub->renderer.walls_layer);
	if (cub->renderer.ui_layer)
		mlx_delete_image(cub->mlx, cub->renderer.ui_layer);
	return (0);
}

void	clear_image_buffer(mlx_image_t *img)
{
	ft_memclear(img->pixels, img->width * img->height * sizeof(int));
}

void	__mlx_fill_pixels(uint8_t *pixels, size_t total_bytes, int col)
{
	size_t	*b;
	int		*p;
	size_t	xl;

	p = (int *)&xl;
	*p = col;
	*(p + 1) = col;
	b = (size_t *)pixels;
	while (total_bytes >= sizeof(size_t))
	{
		*(b++) = xl;
		total_bytes -= sizeof(size_t);
	}
	p = (int *)b;
	while (total_bytes >= sizeof(int))
	{
		*(p++) = col;
		total_bytes -= sizeof(int);
	}
}

void	mlx_set_bg_color_traditional(mlx_image_t *img, uint32_t col)
{
	uint32_t	i;
	uint32_t	j;

	i = -1;
	while (++i < img->height)
	{
		j = -1;
		while (++j < img->width)
			mlx_put_pixel(img, j, i, col);
	}
}

void	mlx_set_bg_color(mlx_image_t *img, int col)
{
	__mlx_fill_pixels(img->pixels, img->width * img->height, col);
}

void	mlx_set_color_in_rows(mlx_image_t *img, int start, int end, int col)
{
	__mlx_fill_pixels(img->pixels + (sizeof(int) * start * img->width),
		sizeof(int) * (end - start) * img->width, col);
}
/*
// IS DUMMY FUNCTION FOR NOW. returns some red brick looking color.
int	find_wall_texture_pixel(int side)
{
		if (side == W_SIDE)
			return (0xD69550ff);
	//		return (0xa35940ff);
		else if (side == N_SIDE)
			return (0xe2a258ff);
	//		return (0x753d2aff);
		else if (side == E_SIDE)
			return (0xdc843cff);
	//		return (0xa35940ff);
		else if (side == S_SIDE)
			return (0xc26b35ff);
	//		return (0x944f38ff);
	//		return (0xc96c4dff);
		return (0xa35940ff);
	//	return (0xff3e519e);
}

int	texture_get_pixel(mlx_texture_t *tex, int x, int y)
{
	return ((int *)tex->pixels[y * tex->width + x]);
}
*/

void	cub_put_pixel(mlx_image_t *img, int x, int y, int col)
{
	((uint32_t *)img->pixels)[x + y * img->width] = col;
}

int	get_texture_pixel(mlx_texture_t *tex, int x, int y)
{
//	printf("get_tex_pix : skymap width : %d, x : %d, y : %d\n", tex->width, x, y);
//	if (x < 0 || y < 0)
//	{
//		dprintf(2, "x or y < 0 : %d, %d\n", x, y);
//		return 0xffffffff;
//	}
	return (((uint32_t *)tex->pixels)[x + y * tex->width]);
}

void	render_skymap_column(t_cub *cub, int x, int end_y)
{
	const int	tex_col = (int)((x - cub->scn_midx) * cub->inv_sw * cub->skymap_fov_to_tex
		+ cub->skymap_tex_offset) % cub->tex.skymap->width;
	int		y;

	end_y = ft_clamp(end_y, 0, cub->tex.skymap->height);
//	printf("skymap : ori_factor * skymap width : %f\n", cub->hero.ori_factor * cub->tex.skymap->width);
//	printf("skymap : (x - scn_midx) : %d, fov to tex : %f, tex_offset : %d, tex_col : %d\n", x - cub->scn_midx, cub->skymap_fov_to_texture, cub->skymap_tex_offset, tex_col);
//	printf("skymap : x : %d, end_y %d, tex_col : %d\n", x, end_y, tex_col);
	y = -1;
	while (++y < end_y)
		cub_put_pixel(cub->renderer.walls_layer, x, y,
			get_texture_pixel(cub->tex.skymap, tex_col, y));// potentially find some sophisticated function for y.
}

int	find_wall_texture_pixel(int *pxls, int offset)
{
//	printf("%x, ", pxls[offset]);
	return (pxls[offset]);
}

void	render_walls(t_cub *cub)
{
	const float	flr_texw_to_cellw = cub->inv_cw * cub->tex.floor->width;
	const float	flr_texh_to_cellw = cub->inv_cw * cub->tex.floor->height;
	int		i;
	int		j;

	t_rdata		*rd;


//	float		*tex_info;
	int		half_texh;
//	int		scn_fheight;
	int		scn_height;
	int		half_height;
	
	int		scn_start_y;
	int		tex_start_x;
//	int		*sides;
	mlx_texture_t	*tex;
	uint32_t	*pxls;
//	int		y_offset;
//	float		incr;
	float		ratio;
	
//	for FLOORCASTER
//	float		rays_x;
//	float		rays_y;
	float		param;
//	float		rx;
//	float		ry;

	rd = cub->hero.rcast.rdata - 1;

//	printf("Randy : Clear image buffer \n");
	clear_image_buffer(cub->renderer.walls_layer);
//	printf("Randy : Cleared \n");
//	sides = (int *)cub->hero.coll_sides->arr - 1;
//	tex_info = (float *)cub->hero.tex_infos->arr - 1;//_mtx_index_fp(cub->hero->tex_infos, i, 0);
//	printf("Dist for first ray : %f\n", _mtx_index_f(cub->hero.distances, 0, 0));

	i = -1;
	while (++i < SCN_WIDTH)
	{
//		rays_x = _mtx_index_f(cub->hero.rays[0], i, 0);//(float *)cub->hero.rays[0]->arr - 1;
//		rays_y = _mtx_index_f(cub->hero.rays[1], i, 0);//(float *)cub->hero.rays[0]->arr - 1;
		++rd;
		tex = cub->tex.walls[rd->side];
		half_texh = (tex->height >> 1);
		
//		dprintf(2, "tex ratio : %f, side : %d\n", rd->tex_ratio, rd->side);
//		if (rd->tex_ratio < 0)
			//rd->tex_ratio *= -1;
//			dprintf(2, "WARNING : TEX RATIO IS NEGATIVE !\n");
		
		tex_start_x = (int)(rd->tex_ratio * tex->width);
		scn_height = ft_clamp(rd->tex_height, 0, SCN_HEIGHT);
		half_height = (scn_height >> 1);

		ratio = (float)tex->height / (float)rd->tex_height;
		pxls = (uint32_t *)tex->pixels + tex_start_x;

//		if ((void *)pxls < (void *)tex->pixels)
//			dprintf(2, "WARNING : TEX PIXELS OFFSET IS IN DANGER ZONE !\n");

//		tex = cub->tex.walls[*(++sides)];
//		half_texh = (tex->height >> 1);
//		tex_start_x = (int)(*(++tex_info) * tex->width);
//		scn_fheight = (int)*(++tex_info);//_mtx_index_f(cub->hero.tex_infos, i, 1);
//		scn_height = ft_clamp(scn_fheight, 0, SCN_HEIGHT);

//		half_height = (scn_height >> 1);

//		printf("scn_fheight : %d, scn_height : %d, tex_start_x : %d\n", scn_fheight, scn_height, tex_start_x);
		scn_start_y = ((SCN_HEIGHT - scn_height) >> 1);// divide by 2. (SCN_HEIGHT / 2 - height / 2)
//		y_offset = (tex->height >> 2);//(fheight - height) >> 1;// divide by 2
//		printf("col %d, sx, sy : (%d, %d), fh, h: (%d, %d), yoff %d\n", i, tex_start_x, scn_start_y,
//			scn_fheight, scn_height, y_offset);
//		pxls = (int *)tex->pixels + tex_start_x + y_offset * tex->width;
//		incr = (float)(tex->height - y_offset) / (float)scn_height;// * tex->width;

//		ratio = (float)tex->height / (float)scn_fheight;
//		pxls = (uint32_t *)tex->pixels + tex_start_x;
//		pxls = (int *)tex->pixels + tex_start_x + half_texh * tex->width;
//			+ (int)((tex->height - (scn_start_y * ratio)) >> 1) * tex->width;

//		printf("Randy : wall while enter from height %d to %d\n", scn_start_y, scn_height);

//		dprintf(2, "perso orientation : %f\n", ((float *)cub->hero.rcast.ray_thetas->arr)[SCN_WIDTH >> 1]);

//////////////	WALLS RENDERING ///////////////////
		j = -1;
		while (++j < scn_height)
			cub_put_pixel(cub->renderer.walls_layer, i, scn_start_y + j,
				pxls[(int)(((j - half_height) * ratio) + half_texh) * tex->width]);
				//pxls[(int)(((half_height) * ratio) + half_texh) * tex->width]);
//			mlx_put_pixel(cub->renderer.walls_layer, i, scn_start_y + j,
//				find_wall_texture_pixel(pxls,
//					((int)((j - (scn_height >> 1)) * ratio) + half_texh) * tex->width));
		
//////////////	SKYMAP RENDERING ///////////////////
		render_skymap_column(cub, i, scn_start_y);

//////////////	FLOORCASTING  //////////////////////
		j = scn_start_y + j - 1;
		while (++j < SCN_HEIGHT)
		{
			param = get_floorcaster_param(cub, i, j);
//			rx = rays_x * param + cub->hero.px;
//			ry = rays_y * param + cub->hero.py;
//			rx = rays_x * param + cub->hero.px;
//			ry = rays_y * param + cub->hero.py;
//			printf("floor (rayx, rayy) : (%f, %f),  pixel collision : (%f, %f), param : %f\n",
//				rays_x, rays_y, rx, ry, param);
			cub_put_pixel(cub->renderer.walls_layer, i, j,
				get_texture_pixel(cub->tex.floor,
					(int)(fmodf((*rd->rx) * param + (*rd->px), CELL_WIDTH)
						* flr_texw_to_cellw),//cub->inv_cw * cub->tex.floor->width),
					(int)(fmodf((*rd->ry) * param + (*rd->py), CELL_WIDTH)
						* flr_texh_to_cellw)));//cub->inv_cw * cub->tex.floor->height)));
		}
	}
}

static int	is_obj_in_fov(t_hero *hero, t_oinst *obj)
{
	return ((((*hero->fov_lx) * obj->oy + (*hero->fov_ly) * -obj->ox) > 0)
		!= (((*hero->fov_rx) * obj->oy + (*hero->fov_ry) * -obj->ox) > 0));
}

static void	render_objects(t_cub *cub)
{
	t_oinst	*obj;
	float	drawx;
	float	u_proj;
	float	ratio;
//	int	i;
	float	scn_width;
	float	scn_height;
	float	scn_halfw;
	float	scn_halfh;
	float	tex_incrx;
	float	tex_incry;
	int	start[2];
	int	end[2];
	int	i;
	int	j;
	mlx_texture_t	*tex;

	obj = cub->objs.instances;
	while (obj)
	{
		obj->ox = obj->px - cub->hero.px;
		obj->oy = obj->py - cub->hero.py;
		
		u_proj = (*cub->hero.dirx) * obj->ox + (*cub->hero.diry) * obj->oy;
		if ((u_proj <= 0) || !is_obj_in_fov(&cub->hero, obj))
		{
			obj = obj->next;
			continue ;
		}
	//	drawx = (*cub->hero.diry) * obj->ox
	//		- (*cub->hero.dirx) * obj->oy
	//		+ cub->scn_midx;
	//	drawx = cub->near_proj_factor / u_proj;
		ratio = cub->near_z / u_proj;
		drawx = ((*cub->hero.dirx) * obj->oy - (*cub->hero.diry) * obj->ox)
			* ratio + cub->scn_midx;

		scn_width = ratio * obj->type->width;
		scn_halfw = scn_width / 2;
		scn_height = ratio * obj->type->height;
		scn_halfh = scn_height / 2;
		tex = obj->type->texs[obj->tex_idx];
		tex_incrx = tex->width / scn_width;
		tex_incry = tex->height / scn_height;
//		printf("tex increments x / y : %f, %f\n", tex_incrx, tex_incry);
		start[0] = drawx - scn_halfw;
		start[1] = cub->scn_midy - scn_halfh;
		end[0] = drawx + scn_halfw;
		end[1] = cub->scn_midy + scn_halfh;
		//pxls = (uint32_t *)obj->type->texs[obj->tex_idx]->pixels;
		if (start[0] < 0 || start[1] < 0 || end[0] >= SCN_WIDTH || end[1] >= SCN_HEIGHT)
		{
			obj = obj->next;
			continue ;
		}
		i = -1;
		while (++i < scn_height)
		{
			j = -1;
			while (++j < scn_width)
			{
//				printf("put tex px (%d, %d) at scn coord (%d, %d).\n", (int)(j * tex_incrx),
//					(int)(i * tex_incry), j + start[0], i + start[1]);
				cub_put_pixel(cub->renderer.walls_layer, j + start[0], i + start[1], 
				//	pxls[(int)(j * tex_incrx + i * tex_incry * obj->type->texs[obj->tex_idx]->width)]);
					get_texture_pixel(tex,
						(int)(j * tex_incrx),
						(int)(i * tex_incry)));
			}
		}
			
		//printf("draw obj at scn col : %d, u_proj : %f, (ox, oy) : (%f, %f), scn w/h : %f, %f\n",
		//	(int)drawx, u_proj, obj->ox, obj->oy, scn_width, scn_height);

		obj = obj->next;
	}
}

// Called anytime a noticeable change is made
void	render_scene(t_cub *cub)
{
//	printf("(((_______( RENDERING THAT SCENE BABYY !! )_________)))\n\n");
//	ft_deltatime_usec_note(NULL);
	render_walls(cub);
	render_objects(cub);
//	ft_deltatime_usec_note("What a day to be  alive");
	cub->renderer.requires_update = 0;
///	render_skymap(cub);	// potential addition
//	render_ui(cub);		// potential addition
}

// Called only once at start
int	init_renderer(t_cub *cub)
{
	printf("init renderer : start. mlx : %p\n", cub->mlx);

	cub->renderer.bg_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
//	mlx_set_instance_depth(cub->renderer.bg_layer->instances, 3);
	cub->renderer.walls_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
//	mlx_set_instance_depth(cub->renderer.walls_layer->instances, 2);
	cub->renderer.ui_layer = mlx_new_image(cub->mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);

//	printf("bla : \n");//,cub->tex.walls[0]);
//	cub->renderer.ui_layer = mlx_texture_to_image(cub->mlx, cub->tex.walls[0]);
//	printf("bla2\n");
//	mlx_set_instance_depth(cub->renderer.ui_layer->instances, 1);

	if (!cub->renderer.bg_layer || !cub->renderer.walls_layer || !cub->renderer.ui_layer)
		return (-1);
	mlx_set_color_in_rows(cub->renderer.bg_layer, 0, SCN_HEIGHT >> 1, cub->tex.color[0]);//0xffffe77b);
	mlx_set_color_in_rows(cub->renderer.bg_layer, SCN_HEIGHT >> 1, SCN_HEIGHT, cub->tex.color[1]);//0xff63615d);
	//mlx_set_bg_color(cub->renderer.ui_layer, 0xf05ae686);
//	mlx_set_bg_color_traditional(cub->renderer.ui_layer, 0xf786e65a);//  5ae686);
	// mlx_set_bg_color_traditional(cub->renderer.ui_layer, 0x00ff007f);//  5ae686);
//	mlx_set_bg_color_traditional(cub->renderer.ui_layer, 0xf75ae686);
	
//	mlx_image_t	*skymap_mockup = mlx_texture_to_image(cub->mlx, cub->tex.skymap);
//	if (skymap_mockup)
//		printf("SKYMAP MOCKUP SUCCESSFULL\n");
//	else
//		printf("SKYMAP MOCKUP FAILED\n");
//	mlx_image_to_window(cub->mlx, cub->renderer.bg_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.walls_layer, 0, 0);
//	mlx_image_to_window(cub->mlx, skymap_mockup, 0, SCN_HEIGHT - 128);
	if (ENABLE_MINIMAP)
		mlx_image_to_window(cub->mlx, cub->renderer.ui_layer,
			MINIMAP_PADX, SCN_HEIGHT - MINIMAP_PADY - MINIMAP_HEIGHT);
	printf("init renderer : exit \n");
	return (0);
}
