/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_floor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 17:27:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/20 22:54:53 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

typedef struct s_floor_ceiling_rend_data
{
	t_cub			*cub;
	t_rdata			*rd;
	int				x;
	int				y;
	float			*params;
	uint32_t		*fl_p;
	uint32_t		*wl_p;
	uint32_t		*cl_p;
	mlx_texture_t	**tex_arr;
}	t_flrd;

static void	__render_flr_cil_incr(t_flrd *fd)
{
	++fd->rd;
	++fd->params;
	++fd->fl_p;
	++fd->wl_p;
	++fd->cl_p;
}

static void	__init_flr_ceil_render_data(t_cub *cub, t_flrd *fd)
{
	fd->cub = cub;
	fd->x = -1;
	fd->y = 0;
	fd->params = cub->renderer.floor_factors - 1;
	fd->fl_p = (uint32_t *)cub->renderer.bg_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy + 1)) - 1;
	fd->wl_p = (uint32_t *)cub->renderer.walls_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy + 1)) - 1;
	fd->cl_p = (uint32_t *)cub->renderer.bg_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy - 2)) - 1;
}

static void	__floor_ceil_draw_pixel(t_flrd *fd, t_rdata *rd, int *c, float *t)
{
	const float	inv_cw = fd->cub->inv_cw;

	if (is_wall(&fd->cub->map, c[0], c[1]))
	{
		if (rd->side == N_SIDE || rd->side == S_SIDE)
			c[1] = rd->cy - rd->cincr_y;
		else
			c[0] = rd->cx - rd->cincr_x;
	}
	fd->tex_arr = fd->cub->map.mx[c[1]][c[0]]->xwalls;
	if (!fd->tex_arr)
		return ;
	*fd->fl_p = get_tex_pixel(fd->tex_arr[0],
			t[0] * fd->tex_arr[0]->width * inv_cw,
			t[1] * fd->tex_arr[0]->height * inv_cw);
	if (!fd->tex_arr[1])
		*fd->cl_p = 0x0;
	else
		*fd->cl_p = get_tex_pixel(fd->tex_arr[1],
				t[0] * fd->tex_arr[1]->width * inv_cw,
				t[1] * fd->tex_arr[1]->height * inv_cw);
}

static int	__isvalid_floor_intersect(t_flrd *fd, float *p, int *c)
{
	p[0] = *fd->rd->rx * (*fd->params) + fd->cub->hero.ply_obj->px;
	p[1] = *fd->rd->ry * (*fd->params) + fd->cub->hero.ply_obj->py;
	if (*fd->wl_p || p[0] < 0.0f || p[1] < 0.0f)
		return (-1);
	get_cell(p[0], p[1], c, c + 1);
	if (c[0] < 0 || c[1] < 0
		|| c[0] >= fd->cub->map.width || c[1] >= fd->cub->map.height)
		return (-1);
	return (0);
}

void	render_floor_sky(t_cub *cub)
{
	t_flrd			fd;
	float			p[2];
	int				c[2];
	float			t[2];

	__render_sky(cub);
	printf("Draw floor start\n");
	__init_flr_ceil_render_data(cub, &fd);
	while (++fd.y < cub->scn_midy)
	{
		fd.rd = cub->hero.rcast.rdata - 1;
		fd.x = -1;
		while (++fd.x < SCN_WIDTH)
		{
			__render_flr_cil_incr(&fd);
			if (__isvalid_floor_intersect(&fd, p, c) < 0)
				continue ;
			find_vector_delta(get_grid_coords(&cub->map, c[0], c[1]), p, t);
			__floor_ceil_draw_pixel(&fd, fd.rd, c, t);
		}
		fd.cl_p -= 2 * SCN_WIDTH;
	}
	printf("Draw floor end\n");
}
