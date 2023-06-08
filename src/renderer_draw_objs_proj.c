/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs_proj.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 20:26:50 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/08 04:24:53 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	setup_tex_and_layer_offsets(t_objd *od)
{
	int	j;

	j = -1;
	while (++j < od->dims[1])
	{
		od->tys[j] = (int)((j + od->toffs[1]) * od->tincrs[1]) *od->tex->width;
		od->lys[j] = j + od->loffs[1];
	}
}

void	__render_proj_obj(t_cub *cub, t_objd *od)
{
	int			i;
	int			j;

	setup_tex_and_layer_offsets(od);
	i = -1;
	while (++i < od->dims[0])
	{
		if (od->pdata[i + od->loffs[0]].dist < od->odist)
			continue ;
		od->so = i + od->loffs[0];
		od->to = (int)((i + od->toffs[0]) * od->tincrs[0]);
		od->dp = cub->renderer.dpbuff + od->so + cub->yoffs[od->loffs[1] - 1];
		od->ip = cub->renderer.isproj + od->so + cub->yoffs[od->loffs[1] - 1];
		j = -1;
		while (++j < od->dims[1])
		{
			od->dp += SCN_WIDTH;
			od->ip += SCN_WIDTH;
			od->cl = od->pxls[od->to + od->tys[j]];
			if (!od->cl || !*od->ip || (*od->dp && od->odist > *od->dp))
				continue ;
			cub_put_pixel(cub->renderer.objs_layer, od->so, od->lys[j], od->cl);
			*od->dp = od->odist;
		}
	}
}
/*
// Assumes is_drawable == true, checked earlier.
static inline void	select_draw_proj_texture(t_objd *op, t_oinst *obj)
{
	const float		rad_to_idx_ratio = 8.0f * M_INV_TAU;
	float			dpos[2];
	float			rel_ori;
	int				idx;

	if (!obj)
		return ;
	od->tex = NULL;
	if (obj->type->is_oriented)
	{
		dpos[0] = obj->px - od->ppos[0];
		dpos[1] = obj->py - od->ppos[1];
		rel_ori = atan2f(dpos[1], dpos[0]) - obj->ori + FOV45_HF;
		while (rel_ori < 0)
			rel_ori += M_TAU;
		idx = (int)(rel_ori * rad_to_idx_ratio) % 8;
		od->tex = obj->gset->xwalls[idx];
	}
	else
		od->tex = obj->gset->xwalls[obj->tex_idx];
	od->pxls = (uint32_t *)(od->tex->pixels);
}
*/
/*
static inline void	setup(t_cub *cub, t_objd *op, const int *pframe, int offy)
{
	od->tincrs[0] = (float)od->tex->width / (float)od->dims[0];
	od->tincrs[1] = (float)od->tex->height / (float)od->dims[1];
	*((size_t *)od->toffs) = 0;
	od->loffs[0] = od->drawx - (od->dims[0] >> 1);
	od->loffs[1] = cub->scn_midy - (od->dims[1] >> 1) + (int)(offy * od->ratio);
	od->loffs[2] = od->loffs[0] + od->dims[0];
	od->loffs[3] = od->loffs[1] + od->dims[1];
	if (od->loffs[0] < pframe[0])
	{
		od->toffs[0] = pframe[0] - od->loffs[0];
		od->dims[0] -= od->toffs[0];
		od->loffs[0] = pframe[0];
	}
	if (od->loffs[1] < pframe[1])
	{
		od->toffs[1] = pframe[1] - od->loffs[1];
		od->dims[1] -= od->toffs[1];
		od->loffs[1] = pframe[1];
	}
	if (od->loffs[2] > pframe[2])
		od->dims[0] -= (od->loffs[2] - pframe[2]);
	if (od->loffs[3] > pframe[3])
		od->dims[1] -= (od->loffs[3] - pframe[3]);
	od->loffs[2] = ft_clamp(od->loffs[2], pframe[0], pframe[2]);
	od->loffs[3] = ft_clamp(od->loffs[3], pframe[1], pframe[3]);
}
*/

static inline void	init_draw_proj_objects(t_cub *cub, t_objd *od)
{
	od->pframe = cub->renderer.pframe;
	od->pdata = cub->hero.rcast.pdata;
	od->isproj = cub->renderer.isproj;
	od->dpbuff = cub->renderer.dpbuff;
	od->obj = cub->objs.instances;
	od->prtl = (t_oinst *)cub->renderer.portal;
	od->link = (t_oinst *)cub->renderer.portal->relative;
//	od->ppos[0] = cub->hero.ply_obj->px + (od->link->px - od->prtl->px);
//	od->ppos[1] = cub->hero.ply_obj->py + (od->link->py - od->prtl->py);
	od->ppos[0] = od->link->px - od->prtl->px;
	od->ppos[1] = od->link->py - od->prtl->py;
	od->pdist = (*cub->hero.dirx) * od->ppos[0] + (*cub->hero.diry) * od->ppos[1];
	od->ppos[0] += cub->hero.ply_obj->px;  
	od->ppos[1] += cub->hero.ply_obj->py;  
}
/*
static inline void	obj_draw_check(t_cub *cub, t_objd *od)
{
	od->tex = NULL;
	if (!od->obj->type->is_drawable || !od->obj->gset
//		|| od->obj == cub->hero.ply_obj)
		return ;
	od->ov[0] = od->obj->px - od->ppos[0];//cub->hero.ply_obj->px;
	od->ov[1] = od->obj->py - od->ppos[1];//cub->hero.ply_obj->py;
	od->odist = (*cub->hero.dirx) * od->ov[0] + (*cub->hero.diry) * od->ov[1];
//	od->tex = select_draw_texture(cub, od->obj);
	od->ratio = cub->near_z / od->odist;
	od->drawx = (int)(((*cub->hero.dirx) * od->ov[1] - (*cub->hero.diry)
				* od->ov[0]) * od->ratio) + cub->scn_midx;
	od->dims[0] = (int)(od->ratio * od->obj->type->width);
	od->dims[1] = (int)(od->ratio * od->obj->type->height);
	return ;
}
*/
/*
static inline void	obj_draw_check(t_cub *cub, t_objd *op)
{
	od->dims[0] = 0;
	if (!od->obj->type->is_drawable || !od->obj->gset)
		return ;
	od->ov[0] = od->obj->px - od->ppos[0];
	od->ov[1] = od->obj->py - od->ppos[1];
	od->odist = (*cub->hero.dirx) * od->ov[0] + (*cub->hero.diry) * od->ov[1];
	od->ratio = cub->near_z / od->odist;
	od->drawx = (int)(((*cub->hero.dirx) * od->ov[1] - (*cub->hero.diry)
				* od->ov[0]) * od->ratio) + cub->scn_midx;
	od->dims[0] = (int)(od->ratio * od->obj->type->width);
	od->dims[1] = (int)(od->ratio * od->obj->type->height);
}
*/
void	__render_proj_objects(t_cub *cub)
{
	t_objd		od;

	init_draw_proj_objects(cub, &od);
	printf("Try project obj 0\n");
	if (od.pdist < 0.0f)
		return ;
	printf("Try project obj 0.5\n");
	while (od.obj)
	{
		__rdr_obj_draw_check(cub, &od);
		printf("Try project obj 1\n");
		if ((!od.dims[0] || ((od.odist - 1.0f) <= od.pdist)
			|| (od.drawx + (od.dims[0] >> 1)) < od.pframe[0]
			|| od.pframe[2] <= (od.drawx - (od.dims[0] >> 1)))
			&& next_obj(&od.obj))
			continue ;

		printf("Try project obj 2\n");
		__rdr_select_draw_texture(&od, od.obj);
		printf("Try project obj 3\n");
		__rdr_setup_draw_objects(cub, &od, od.pframe, od.obj->type->offy);
		printf("Try project obj 4\n");
		__render_proj_obj(cub, &od);
		printf("Try project obj 5\n");
		od.obj = od.obj->next;
	}
}
