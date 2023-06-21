/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs_proj.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 20:26:50 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/19 21:55:26 by iamongeo         ###   ########.fr       */
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

static inline void	__setup_draw_proj_column(t_cub *cub, t_objd *od, int i)
{
	int	buff_offset;

	od->so = i + od->loffs[0];
	od->to = (int)((i + od->toffs[0]) * od->tincrs[0]);
	buff_offset = od->so + (od->loffs[1] - 1) * SCN_WIDTH;
	od->dp = cub->renderer.dpbuff + buff_offset;
	od->ip = cub->renderer.isproj + buff_offset;
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
		__setup_draw_proj_column(cub, od, i);
		j = -1;
		while (++j < od->dims[1])
		{
			od->dp += SCN_WIDTH;
			od->ip += SCN_WIDTH;
			od->cl = od->pxls[od->to + od->tys[j]];
			if (!od->cl || !*od->ip || (*od->dp && od->odist > *od->dp))
				continue ;
			cub_put_pixel(cub->renderer.objs_layer, od->so, od->lys[j],
				od->cl & TRANSPARENCY);
			*od->dp = od->odist;
		}
	}
}

static inline void	init_draw_proj_objects(t_cub *cub, t_objd *od)
{
	od->pframe = cub->renderer.pframe;
	od->pdata = cub->hero.rcast.pdata;
	od->isproj = cub->renderer.isproj;
	od->dbuff = cub->renderer.dbuff;
	od->dpbuff = cub->renderer.dpbuff;
	od->obj = cub->objs.instances;
	od->prtl = (t_oinst *)cub->renderer.portal;
	od->link = (t_oinst *)cub->renderer.portal->relative;
	od->ppos[0] = cub->hero.ply_obj->px + (od->link->px - od->prtl->px);
	od->ppos[1] = cub->hero.ply_obj->py + (od->link->py - od->prtl->py);
	od->pdist = (*cub->hero.dirx) * (od->link->px - od->ppos[0])
		+ (*cub->hero.diry) * (od->link->py - od->ppos[1]);
}

void	__render_proj_objects(t_cub *cub)
{
	t_objd		od;

	init_draw_proj_objects(cub, &od);
	if (od.pdist < 0.0f)
		return ;
	while (od.obj)
	{
		__rdr_obj_draw_check(cub, &od);
		if ((!od.dims[0] || ((od.odist - 1.0f) <= od.pdist) || od.prtl == od.obj
				|| __rdr_obj_out_of_frame(&od))
			&& next_obj(&od.obj))
			continue ;
		__rdr_select_draw_texture(&od, od.obj);
		__rdr_setup_draw_objs(cub, &od, od.pframe, od.obj->type->offy);
		__render_proj_obj(cub, &od);
		od.obj = od.obj->next;
	}
}
