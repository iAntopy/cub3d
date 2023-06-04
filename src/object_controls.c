/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:45:52 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/04 15:21:09 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// delta is a pointer to an array that will receive the vector between 
// obj and pos.
int	is_in_solid_object(t_cub *cub, t_oinst *obj, float *pos, float *delta)
{
	t_oinst	*other;

	other = cub->objs.instances;
	while (other)
	{
		if (!other->type->is_solid || !obj->type->is_solid || obj == other)
		{
			other = other->next;
			continue ;
		}
		delta[0] = other->px - pos[0];
		delta[1] = other->py - pos[1];
		if ((delta[0] * delta[0] + delta[1] * delta[1])
			< (other->type->width * other->type->width / 2.0f))
			return (1);
		other = other->next;
	}
	delta[0] = 0.0f;
	delta[1] = 0.0f;
	return (0);
}

// give map coord and a pointer to a int cell[2] return array to get cell 
// values for this position.
void	get_cell(float px, float py, int *cx, int *cy)
{
	static const float	inv_cw = 1.0f / CELL_WIDTH;

	*cx = (int)(inv_cw * px);
	*cy = (int)(inv_cw * py);
	printf("get_cell : pos : (%f, %f), cell : (%d, %d)\n", px, py, *cx, *cy);
}

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
	float	pos[2];// = {obj->px + dx, obj->py + dy};
	int 		cell[2];
	float	delta[4];
	int	iswall;
    
	if (!obj || (!dx && !dy))
		return ;
	pos[0] = obj->px + dx;
	pos[1] = obj->py + dy;
//	get_cell(pos[0], pos[1], cell, cell + 1);
//	cx = (int)(cub->inv_cw * (obj->px + dx));
//	cy = (int)(cub->inv_cw * (obj->py + dy));
//	if (!get_is_cell_within_bounds(&cub->map, cx, cy))
//		return ;
	if (is_in_solid_object(cub, obj, pos, delta))
	{
		delta[2] = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
		delta[0] /= delta[2];
		delta[1] /= delta[2];
		delta[3] = dx * delta[1] - dy * delta[0];
		delta[2] = obj->type->width - delta[2];

		//obj->px += delta[0] * delta[3];
		//obj->py += delta[1] * delta[3];
		dx = delta[1] * delta[3] - delta[0] * delta[2];
		dy = -delta[0] * delta[3] - delta[1] * delta[2];
		//dy = -delta[0] * delta[3];
		pos[0] = obj->px + dx;
		pos[1] = obj->py + dy;

//		cx = (int)(cub->inv_cw * (obj->px + dx));
//		cy = (int)(cub->inv_cw * (obj->py + dy));
	}
	get_cell(pos[0], pos[1], cell, cell + 1);
	iswall = is_wall(&cub->map, cell[0], cell[1]);
	obj->px += dx * !(iswall && (obj->cx == cell[0]));
	obj->py += dy * !(iswall && (obj->cy == cell[1]));//& (0xffffffff * is_wall * (obj->cy == cell[1]));
	get_cell(obj->px, obj->py, &obj->cx, &obj->cy);//cell, cell + 1);

	/*

	if (is_wall(&cub->map, cell[0], cell[1]))
	{
		obj->px += dx * (obj->cx == cell[0]);
		obj->py += dy * (obj->cy == cell[1]);
	}
	else
	{
		obj->px += dx;
		obj->py += dy;
	}
	obj->cx = (int)(cub->inv_cw * obj->px);
	obj->cy = (int)(cub->inv_cw * obj->py);
	*/
	cub->renderer.requires_update = 1;
}

void	obj_move_rel(t_cub *cub, t_oinst *obj, float d_walk, float d_strafe)
{
	float	pos[2];
	float	ori_vect[2];
	float	delta[4];
	int		cell[2];
	int		iswall;
//	float	dy;
//	int		cx;
//	int		cy;

	if (!obj || (!d_strafe && !d_walk))// || !ply || ply->type->type_enum != OBJ_PLAYER)
		return ;
	printf("\nori pos : (%f, %f), d_walk : %f, d_strafe : %f\n", obj->px, obj->py, d_walk, d_strafe);
	ori_vect[0] = cosf(obj->ori);
	ori_vect[1] = sinf(obj->ori);
	printf("raw ori_vect : [%f, %f]\n", ori_vect[0], ori_vect[1]);
	delta[0] = (d_walk * ori_vect[0]) - (d_strafe * ori_vect[1]);
	ori_vect[1] = (d_walk * ori_vect[1]) + (d_strafe * ori_vect[0]);
	ori_vect[0] = delta[0];
	pos[0] = obj->px + ori_vect[0];
	pos[1] = obj->py + ori_vect[1];
	printf("ovect : [%f, %f], pos : (%f, %f)\n", ori_vect[0], ori_vect[1], pos[0], pos[1]);
//	get_cell(pos[0], pos[1], cell, cell + 1);//cell, cell + 1);
//	cx = (int)(cub->inv_cw * (obj->px + dx));
//	cy = (int)(cub->inv_cw * (obj->py + dy));
//	if (!get_is_cell_within_bounds(&cub->map, cx, cy))
//		return ;
	if (is_in_solid_object(cub, obj, pos, delta))// obj->px + dx, obj->py + dy, ori_vect))
	{
		delta[2] = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
		printf("IN SOLID OBJ : delta : [%f, %f], dist : %f\n", delta[0], delta[1], delta[2]);
		if (delta[2] < 0)
			printf("delta is negative\n");
		else
			printf("delta is positive\n");

		delta[0] /= delta[2];
		delta[1] /= delta[2];
//		delta[2] = ((obj->type->width >> 1) - delta[2]) * 0.5f;
		delta[3] = delta[1] * ori_vect[0] - delta[0] * ori_vect[1];
		//ori_vect[0] = delta[1] * delta[3] - delta[0] * delta[2];
		//ori_vect[1] = -delta[0] * delta[3] - delta[1] * delta[2];
		ori_vect[0] = delta[1] * delta[3];// + delta[0] * 0.1f;// * delta[2];
		ori_vect[1] = -delta[0] * delta[3];// + delta[1] * 0.1f;//delta[2];
		pos[0] = obj->px + delta[0] * delta[2] - delta[0] * 32.0f + ori_vect[0];
		pos[1] = obj->py + delta[1] * delta[2] - delta[1] * 32.0f + ori_vect[1];
		//pos[1] = obj->py + ori_vect[1];
//		cx = (int)(cub->inv_cw * (obj->px + dx));
//		cy = (int)(cub->inv_cw * (obj->py + dy));
	}
//	get_cell(obj->px, obj->py, cell, cell + 1);//cell, cell + 1);
	get_cell(pos[0], pos[1], cell, cell + 1);
//	printf("Checking is_wall : cell : (%d, %d)\n", cell[0], cell[1]);
	iswall = is_wall(&cub->map, cell[0], cell[1]);
//	if (iswall && (obj->cx == cell[0]))
//	printf("iswall : %d\n", iswall);
	obj->px += ori_vect[0] * !(iswall && (obj->cx != cell[0]));
	obj->py += ori_vect[1] * !(iswall && (obj->cy != cell[1]));//& (0xffffffff * is_wall * (obj->cy == cell[1]));
	get_cell(obj->px, obj->py, &obj->cx, &obj->cy);//cell, cell + 1);
/*
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
*/
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

//	printf("place obj at pos : (%f, %f), cell (%d, %d)\n", px, py, cx, cy);
	if (!get_is_cell_within_bounds(&cub->map, cx, cy)
		|| is_wall(&cub->map, cx, cy))
		return ;
	obj->px = px;
	obj->py = py;
	obj->cx = cx;
	obj->cy = cy;
}
