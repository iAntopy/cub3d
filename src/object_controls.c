/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:45:52 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/02 21:50:45 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	obj_set_orientation(t_cub *cub, t_oinst *obj, float ori)
{
	if (!obj || !ori)
		return ;
	obj->ori = ori;
    while (obj->ori < 0)
		obj->ori += M_TAU;
    obj->ori = fmodf(obj->ori, M_TAU);
	cub->renderer.requires_update = 1;
}

void	obj_rotate(t_cub *cub, t_oinst *obj, float rot)
{
	if (!obj || !rot)
		return ;
	obj->ori += rot;
    while (obj->ori < 0)
		obj->ori += M_TAU;
    obj->ori = fmodf(obj->ori, M_TAU);
	cub->renderer.requires_update = 1;
}

void	obj_move_abs(t_cub *cub, t_oinst *obj, float dx, float dy)
{
    int cx;
    int cy;
    
    if (!obj || (!dx && !dy))
		return ;
	cx = (int)(cub->inv_cw * (obj->px + dx));
	cy = (int)(cub->inv_cw * (obj->py + dy));
	if (!get_is_cell_within_bounds(&cub->map, cx, cy))
		return ;
	if (is_wall(&cub->map, cx, cy))
	{
		obj->px += dx * (obj->cx == cx);
		obj->py += dy * (obj->cy == cy);
	}
	else
	{
		obj->px += dx;
		obj->py += dy;
	}
	obj->cx = (int)(cub->inv_cw * obj->px);
	obj->cy = (int)(cub->inv_cw * obj->py);
	cub->renderer.requires_update = 1;
}

void	obj_move_rel(t_cub *cub, t_oinst *obj, float d_walk, float d_strafe)
{
	float	ori_vect[2];
	float	dx;
	float	dy;
	int		cx;
	int		cy;

	if (!obj || (!d_strafe && !d_walk))// || !ply || ply->type->type_enum != OBJ_PLAYER)
		return ;
	ori_vect[0] = cosf(obj->ori);
	ori_vect[1] = sinf(obj->ori);
	dx = (d_walk * ori_vect[0]) - (d_strafe * ori_vect[1]);
	dy = (d_walk * ori_vect[1]) + (d_strafe * ori_vect[0]);
	cx = (int)(cub->inv_cw * (obj->px + dx));
	cy = (int)(cub->inv_cw * (obj->py + dy));
	if (!get_is_cell_within_bounds(&cub->map, cx, cy))
		return ;
	if (is_wall(&cub->map, cx, cy))
	{
		obj->px += dx * (obj->cx == cx);
		obj->py += dy * (obj->cy == cy);
	}
	else
	{
		obj->px += dx;
		obj->py += dy;
	}
	obj->cx = (int)(cub->inv_cw * obj->px);
	obj->cy = (int)(cub->inv_cw * obj->py);
	cub->renderer.requires_update = 1;
}

void    obj_set_position(t_cub *cub, t_oinst *obj, float px, float py)
{
	int		cx;
	int		cy;

	if (!obj)
		return ;
	cx = (int)(cub->inv_cw * px);
	cy = (int)(cub->inv_cw * py);

	printf("place obj at pos : (%f, %f), cell (%d, %d)\n", px, py, cx, cy);
	if (!get_is_cell_within_bounds(&cub->map, cx, cy)
		|| is_wall(&cub->map, cx, cy))
		return ;
	obj->px = px;
	obj->py = py;
	obj->cx = cx;
	obj->cy = cy;
}
