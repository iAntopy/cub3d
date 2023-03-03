/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:39:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/03 01:04:48 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	print_rays(t_mtx *xs, t_mtx *ys)
{
	int	len;
	int	i;

	len = xs->shape[0];
	i = -1;
	while (++i < len)
	{
		printf("%d. ( %f, %f )\n", i, _mtx_index_f(xs, i, 0), _mtx_index_f(ys, i, 0));
	}
}

typedef struct s_ray_intersect_data
{
	t_cub	*cub;
	int		idx;

//	int		sx;//	starting cell x	
//	int		sy;//	starting cell y
//	float	px;//	hero pos x
//	float	py;//	hero pos y
	float	dx;//	ray vector delta x
	float	dy;//	ray vector delta y
	int		dirx;//	simple direction of ray. dx < 0 -> -1, dy >= 0 = 1
	int		diry;//	simple direction of ray. dy < 0 -> -1, dy >= 0 = 1

	int		c_offx;//	x offset of cell to check in collision map
	int		c_offy;//	y offset of cell to check in collision map

	int		cx;//	current cell x
	int		cy;//	current cell y
	float	ix;//	current intersect x
	float	iy;//	current intersect y

	//	line func data Y = ax + b
	float	a;//	ray slope
	float	inv_a;//a inverse == 1/a;
	float	b;//	ray y offset
	
//	float	*dist;//	ptr to distance array where to put distance results

}	t_rayint;

inline void	raycast_init_single_vect(t_cub *cub, t_rayint *ri, int ray_idx)
{
	ri->idx = ray_idx;
	ri->dx = _mtx_index_f(cub->hero->rays[0], vi, 0);
	ri->dy = _mtx_index_f(cub->hero->rays[1], vi, 0);
	ri->c_offx = (ri->dx >= 0);
	ri->c_offy = (ri->dy >= 0);
	ri->dirx = (2 * ri->c_offx) - 1;
	ri->diry = (2 * ri->c_offy) - 1;
	ri->cx = ri->sx;
	ri->cy = ri->sy;
	ri->a = dy / dx;
	ri->inv_a = 1 / ri->a;
	ri->b = ri->py - (ri->a * ri->px);
}

// Returns 1 if wall was reached
inline int	raycast_find_cell_intersect(t_rayint *ri)
{
	float	i_vert[2];//	x, y intersect of ray with next vertical cell segment.
	float	i_hori[2];//	x, y intersect of ray with next horizontal cell segment.
	float	*col_ptr;

	i_vert[0] = (ri->cx + ri->c_offx) * CELL_WIDTH;// TODO: Change for indexing in reference coords map;
	i_hori[1] = (ri->cy + ri->c_offy) * CELL_WIDTH;// TODO: Change for indexing in reference coords map;
	
	i_vert[1] = ri->a * i_vert[0] + ri->b;//		y = ax + b;
	i_hori[0] = (i_hori[1] - ri->b) * ri->inv_a;//	x = (y - b) / a;

	if ((i_vert[0] + i_vert[1]) < (i_hor[0] + i_hori[1]))
	{
		ri->cy += ri->diry;
		ri->ix = i_vert[0];
		ri->iy = i_vert[1];
	}
	else
	{
		ri->cx += ri->dirx;
		ri->ix = i_hori[0];
		ri->iy = i_hori[1];
	}
	if (get_is_wall(ri->cub, ri->cx, ri->cy))
	{
		col_ptr = _mtx_index_fptr(ri->cub.hero.collisions, ri->idx, 0);
		*(col_ptr++) = ri->ix;
		*col_ptr = ri->iy;
		return (1);
	}
	return (0);

//	raw_dists[0] = (i_vert[0] * i_vert[0] + i_vert[1] * i_vert[1]);
//	raw_dists[1] = (i_hori[0] * i_hori[0] + i_hori[1] * i_hori[1]);
//	if (raw_dists[0] < raw_dists[1])
//	{
//		ri->cy += ri->diry;
//		ri->ix = i_vert
//	}
}

// Assumes vectors are initialised correctly.
int	raycast_all_vectors(t_cub *cub)
{
	t_rayint	ri;
	int			vi;// vector index from [0-SCN_WIDTH[
	int			collision_occured;

	ri->cub = cub;
	ri.sx = cub->hero.cell_x;
	ri.sy = cub->hero.cell_y;
	ri.px = cub->hero.px;
	ri.py = cub->hero.py;
	collision_occured = 0;
	vi = -1;
	while (++vi < SCN_WIDTH)
	{
		raycast_init_single_vect(cub, &ri, vi);
		while (!raycast_find_cell_intersect(&ri))
			continue ;
	}
	return (0);
}

int	init_raycaster(t_cub *cub)
{
	t_hero	*hero;
	
	hero = &cub->hero;
	hero->thetas = mtx_linspace(hero->ori - FOV_HF, hero->ori + FOV_HF, SCN_WIDTH, 1);
	hero->rays[0] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	hero->rays[1] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	if (!hero->thetas)
		return (-1);
	hero->collisions = mtx_create_empty(SCN_WIDTH, 2, DTYPE_F);
	hero->distances = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
//	mtx_print(hero->thetas);
//	mtx_display_info(hero->thetas);

	mtx_cos(hero->thetas, hero->rays[0]);
	mtx_sin(hero->thetas, hero->rays[1]);

//	mtx_print(hero->rays[0]);
//	mtx_display_info(hero->rays[0]);
//	mtx_print(hero->rays[1]);
//	mtx_display_info(hero->rays[1]);
	print_rays(hero->rays[0], hero->rays[1]);
	return (0);
}
