/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/11 05:59:58 by iamongeo         ###   ########.fr       */
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
	return (0);
}

void	clear_image_buffer(mlx_image_t *img)
{
//	int	*ptr;
//	int	r;
	ft_memclear(img->pixels, img->width * img->height * sizeof(int));
//	r = -1;
//	while (++r < img->height)
//	{
//		ptr = img->pixels + r * img->width;
//		ft_memclear(ptr, img->width * img->height * sizeof(int));
//	}
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
int	find_wall_texture_pixel(int *pxls, int offset)
{
//	printf("%x, ", pxls[offset]);
	return (pxls[offset]);
}


static void	render_walls(t_cub *cub)
{
	int	i;
	int	j;
	float	*tex_info;
	int	half_texh;
	int	scn_fheight;
	int	scn_height;
	
	int	scn_start_y;
	int	tex_start_x;
	int	*sides;
	float	rays_x;
	float	rays_y;
	mlx_texture_t	*tex;
	int	*pxls;
//	int	y_offset;
//	float	incr;
	float	ratio;
	float	param;
	float	rx;
	float	ry;
//	float	height_to_fheight;
//	float	fheight_to_tex;

	clear_image_buffer(cub->renderer.walls_layer);
	sides = (int *)cub->hero.coll_sides->arr - 1;
	tex_info = (float *)cub->hero.tex_infos->arr - 1;//_mtx_index_fp(cub->hero->tex_infos, i, 0);
	i = -1;
	while (++i < SCN_WIDTH)
	{
		rays_x = _mtx_index_f(cub->hero.rays[0], i, 0);//(float *)cub->hero.rays[0]->arr - 1;
		rays_y = _mtx_index_f(cub->hero.rays[1], i, 0);//(float *)cub->hero.rays[0]->arr - 1;
		tex = cub->tex.walls[*(++sides)];
		half_texh = (tex->height >> 1);
		tex_start_x = (int)(*(++tex_info) * tex->width);
		scn_fheight = (int)*(++tex_info);//_mtx_index_f(cub->hero.tex_infos, i, 1);
		scn_height = ft_clamp(scn_fheight, 0, SCN_HEIGHT);


		scn_start_y = ((SCN_HEIGHT - scn_height) >> 1);// divide by 2. (SCN_HEIGHT / 2 - height / 2)
//		y_offset = (tex->height >> 2);//(fheight - height) >> 1;// divide by 2
//		printf("col %d, sx, sy : (%d, %d), fh, h: (%d, %d), yoff %d\n", i, tex_start_x, scn_start_y,
//			scn_fheight, scn_height, y_offset);
//		pxls = (int *)tex->pixels + tex_start_x + y_offset * tex->width;
//		incr = (float)(tex->height - y_offset) / (float)scn_height;// * tex->width;

		ratio = (float)tex->height / (float)scn_fheight;
		pxls = (int *)tex->pixels + tex_start_x;
//		pxls = (int *)tex->pixels + tex_start_x + half_texh * tex->width;
//			+ (int)((tex->height - (scn_start_y * ratio)) >> 1) * tex->width;

		j = -1;
		while (++j < scn_height)
			mlx_put_pixel(cub->renderer.walls_layer, i, scn_start_y + j,
				//start_y + j, pxls[(int)(j * incr) * tex->width]);//find_wall_texture_pixel(pxls, (int)(j * incr) * tex->width));
//				find_wall_texture_pixel(pxls, (int)(j * incr) * tex->width));
//				find_wall_texture_pixel(pxls, (int)(half_texh - (j * ratio - (scn_height >> 2))) * tex->width));
				find_wall_texture_pixel(pxls,
					((int)((j - (scn_height >> 1)) * ratio) + half_texh) * tex->width));

//////////////	FLOORCASTING  //////////////////////
/*
		j = scn_start_y + j - 1;
		while (++j < SCN_HEIGHT)
		{
			param = get_floorcaster_param(cub, i, j);
			//rx = *(++rays_x) * param;
			//ry = *(++rays_y) * param;
			rx = rays_x * param + cub->hero.px;
			ry = rays_y * param + cub->hero.py;
			printf("floor (rayx, rayy) : (%f, %f),  pixel collision : (%f, %f), param : %f\n", rays_x,
				rays_y, rx, ry, param);
		}
	}
*/
}

// Called anytime a noticeable change is made
void	render_scene(t_cub *cub)
{
//	printf("(((_______( RENDERING THAT SCENE BABYY !! )_________)))\n\n");
	render_walls(cub);
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

	printf("bla : \n");//,cub->tex.walls[0]);
//	cub->renderer.ui_layer = mlx_texture_to_image(cub->mlx, cub->tex.walls[0]);
	printf("bla2\n");
//	mlx_set_instance_depth(cub->renderer.ui_layer->instances, 1);

	if (!cub->renderer.bg_layer || !cub->renderer.walls_layer || !cub->renderer.ui_layer)
		return (-1);
	mlx_set_color_in_rows(cub->renderer.bg_layer, 0, SCN_HEIGHT / 2, cub->tex.color[0]);//0xffffe77b);
	mlx_set_color_in_rows(cub->renderer.bg_layer, SCN_HEIGHT / 2, SCN_HEIGHT, cub->tex.color[1]);//0xff63615d);
	//mlx_set_bg_color(cub->renderer.ui_layer, 0xf05ae686);
//	mlx_set_bg_color_traditional(cub->renderer.ui_layer, 0xf786e65a);//  5ae686);
	// mlx_set_bg_color_traditional(cub->renderer.ui_layer, 0x00ff007f);//  5ae686);
//	mlx_set_bg_color_traditional(cub->renderer.ui_layer, 0xf75ae686);
	

	mlx_image_to_window(cub->mlx, cub->renderer.bg_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.walls_layer, 0, 0);
	if (ENABLE_MINIMAP)
		mlx_image_to_window(cub->mlx, cub->renderer.ui_layer,
			MINIMAP_PADX, SCN_HEIGHT - MINIMAP_PADY - MINIMAP_HEIGHT);
	printf("init renderer : exit \n");
	return (0);
}
