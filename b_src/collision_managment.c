/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_managment.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 17:08:34 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 22:54:48 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	__manage_wall_collisions(t_cub *cub, t_oinst *obj)
{
	const int	half_w = obj->type->width >> 2;
	int			cell[2];

	if (!obj || !obj->type->is_solid)
		return ;
	if (get_cell(obj->px - half_w, obj->py, cell, cell + 1)
		&& is_wall(&cub->map, cell[0], cell[1]))
		obj->px += (cell[0] + 1) * CELL_WIDTH - (obj->px - half_w);
	else if (get_cell(obj->px + half_w, obj->py, cell, cell + 1)
		&& is_wall(&cub->map, cell[0], cell[1]))
		obj->px += cell[0] * CELL_WIDTH - (obj->px + half_w);
	if (get_cell(obj->px, obj->py - half_w, cell, cell + 1)
		&& is_wall(&cub->map, cell[0], cell[1]))
		obj->py += (cell[1] + 1) * CELL_WIDTH - (obj->py - half_w);
	else if (get_cell(obj->px, obj->py + half_w, cell, cell + 1)
		&& is_wall(&cub->map, cell[0], cell[1]))
		obj->py += cell[1] * CELL_WIDTH - (obj->py + half_w);
	get_cell(obj->px, obj->py, &obj->cx, &obj->cy);
}

static void	__mutual_displacement(t_cub *cub, t_oinst *o, float *p, float *dt)
{
	const float	delta_incr[2] = {
		dt[0] * (obj_get_width(o) - dt[2]) * 0.05f,
		dt[1] * (obj_get_width(o) - dt[2]) * 0.05f
	};

	p[0] -= delta_incr[0];
	p[1] -= delta_incr[1];
	o->px += delta_incr[0];
	o->py += delta_incr[1];
	__manage_wall_collisions(cub, o);
}

void	manage_collisions(t_cub *cub, t_oinst *ply, float *mv)
{
	t_oinst		*other;
	float		dt[3];
	float		pos[2];

	other = cub->objs.instances;
	pos[0] = ply->px + mv[0];
	pos[1] = ply->py + mv[1];
	while (other)
	{
		dt[0] = other->px - pos[0];
		dt[1] = other->py - pos[1];
		dt[2] = (dt[0] * dt[0] + dt[1] * dt[1]);
		if (!obj_get_issolid(other) || !obj_get_issolid(ply) || ply == other
			|| (dt[2] > ((obj_get_width(other) * obj_get_width(other) >> 1))))
		{
			other = other->next;
			continue ;
		}
		normalize_vec2(dt, &dt[2]);
		__mutual_displacement(cub, other, pos, dt);
		other = other->next;
	}
	ply->px = pos[0];
	ply->py = pos[1];
	__manage_wall_collisions(cub, ply);
}
