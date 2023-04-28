/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 10:21:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/28 10:57:14 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//static int	is_obj_in_fov(t_hero *hero, t_oinst *obj)
static int	is_point_in_fov(t_hero *hero, int x, int y)
{
//	return ((((*hero->fov_lx) * obj->oy + (*hero->fov_ly) * -obj->ox) > 0)
//		!= (((*hero->fov_rx) * obj->oy + (*hero->fov_ry) * -obj->ox) > 0));
	return ((((*hero->fov_lx) * y + (*hero->fov_ly) * -x) > 0)
		!= (((*hero->fov_rx) * y + (*hero->fov_ry) * -x) > 0));
}

void	render_objects(t_cub *cub, t_rdata *rd)
{
	t_oinst	*obj;
	int	drawx;
//	float	u_proj;
	float	ratio;
//	int	i;
	int	scn_width;
	int	scn_height;
	float	scn_halfw;
	float	scn_halfh;
	float	tex_incrx;
	float	tex_incry;
	int	tex_offx;
	int	tex_offy;
	int	start[2];
	int	end[2];
//	uint32_t	*pxls;
	int	i;
	int	j;
	mlx_texture_t	*tex;

	clear_image_buffer(cub->renderer.objs_layer);
	obj = cub->objs.instances;
	while (obj)
	{
		obj->ox = obj->px - cub->hero.px;
		obj->oy = obj->py - cub->hero.py;
		
		obj->dist = (*cub->hero.dirx) * obj->ox + (*cub->hero.diry) * obj->oy;
//		obj->ux = obj->ox / obj->dist;
//		obj->uy = obj->oy / obj->dist;
//		obj->ox_left = obj->px - obj->uy;
//		obj->oy_left = obj->py + obj->ux;
//		obj->ox_right = obj->px + obj->uy;
//		obj->oy_right = obj->py - obj->ux;
//		printf("(ox, oy) : (%f, %f), (lx, ly) : (%f, %f), (rx, ry) : (%f, %f)\n",
//			obj->ox, obj->oy,
//			obj->ox - (*cub->hero.diry) * obj->type->width,
//			obj->oy + (*cub->hero.dirx) * obj->type->width,
//			obj->ox + (*cub->hero.diry) * obj->type->width,
//			obj->oy - (*cub->hero.dirx) * obj->type->width);

		if ((obj->dist <= 0)
			|| !(is_point_in_fov(&cub->hero, obj->ox, obj->oy)
			|| is_point_in_fov(&cub->hero,
				obj->ox - (*cub->hero.diry) * obj->type->half_w,
				obj->oy + (*cub->hero.dirx) * obj->type->half_w)
			|| is_point_in_fov(&cub->hero,
				obj->ox + (*cub->hero.diry) * obj->type->half_w,
				obj->oy - (*cub->hero.dirx) * obj->type->half_w)))
		{
			printf("BAILOUT\n");
			obj = obj->next;
			continue ;
		}
		printf("DRAW\n");
	//	drawx = (*cub->hero.diry) * obj->ox
	//		- (*cub->hero.dirx) * obj->oy
	//		+ cub->scn_midx;
	//	drawx = cub->near_proj_factor / u_proj;
		ratio = cub->near_z / obj->dist;
		drawx = (int)(((*cub->hero.dirx) * obj->oy - (*cub->hero.diry) * obj->ox)
			* ratio) + cub->scn_midx;

		scn_width = (int)(ratio * obj->type->width);
		scn_halfw = scn_width >> 1;
		scn_height = (int)(ratio * obj->type->height);
		scn_halfh = scn_height >> 1;
		tex = obj->type->texs[obj->tex_idx];
		tex_incrx = (float)tex->width / (float)scn_width;
		tex_incry = (float)tex->height / (float)scn_height;
		tex_offx = 0;
		tex_offy = 0;
//		printf("tex increments x / y : %f, %f\n", tex_incrx, tex_incry);
		start[0] = drawx - scn_halfw;
		start[1] = cub->scn_midy - scn_halfh;
		end[0] = drawx + scn_halfw;
		end[1] = cub->scn_midy + scn_halfh;

//		pxls = (uint32_t *)tex->pixels;
		
		if (start[0] < 0)
		{
//			pxls -= (int)(start[0] * tex_incrx);
			tex_offx = -start[0];
			scn_width += start[0];
			start[0] = 0;
		}
		if (start[1] < 0)
		{
//			pxls -= (int)(start[1] * tex_incry * tex->width);
			tex_offy = -start[1];
			scn_height += start[1];
			start[1] = 0;
		}
		if (end[0] > SCN_WIDTH)
			scn_width -= (end[0] - SCN_WIDTH);// * tex_incx;
		//	pxls -= (end[0] - SCN_WIDTH) * tex_incrx;
		if (end[1] > SCN_HEIGHT)
			scn_height -= (end[1] - SCN_HEIGHT);// * tex_incy;
		//	pxls -= (end[1] - SCN_HEIGHT) * tex_incry * tex->width;
		
		//pxls = (uint32_t *)obj->type->texs[obj->tex_idx]->pixels;
//		if (start[0] < 0 || start[1] < 0 || end[0] >= SCN_WIDTH || end[1] >= SCN_HEIGHT)
//		{
//			obj = obj->next;
//			continue ;
//		}
		printf("RENDERING PORTAL OBJECT !\n");
		j = -1;
		while (++j < scn_width)
		{
			if (rd[j + start[0]].dist < obj->dist)
				continue ;
			i = -1;
			while (++i < scn_height)// && ((u_proj < rd[j + start[0]].dist)
			//	&& (cub->renderer.objs_layer->pixels[j * start[0]
			//		+ i * start[1] * SCN_WIDTH] >> 24) == 0))
			{
//				printf("put tex px (%d, %d) at scn coord (%d, %d).\n", (int)(j * tex_incrx),
//					(int)(i * tex_incry), j + start[0], i + start[1]);
				cub_put_pixel(cub->renderer.objs_layer, j + start[0], i + start[1],
					((uint32_t *)tex->pixels)[
						(int)((j + tex_offx) * tex_incrx)
						+ (int)((i + tex_offy) * tex_incry) * tex->width]);
			//		pxls[(int)(j * tex_incrx + i * tex_incry * tex->width)]);
			
				//	pxls[(int)(j * tex_incrx + i * tex_incry * obj->type->texs[obj->tex_idx]->width)]);
			//		get_texture_pixel(tex,
			//			(int)((j + tex_offx) * tex_incrx),
			//			(int)((i + tex_offy) * tex_incry)));
			//	rd[j + start[0]].dist = u_proj;
			}
		//	pxls += (int)(tex_incry * tex->width);
		//	++start[0];
		//	++start[1];
		}
			
		//printf("draw obj at scn col : %d, u_proj : %f, (ox, oy) : (%f, %f), scn w/h : %f, %f\n",
		//	(int)drawx, u_proj, obj->ox, obj->oy, scn_width, scn_height);

		obj = obj->next;
	}
}

