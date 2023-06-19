/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 10:21:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/19 19:17:30 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	adjust_draw_frame(t_cub *cub, t_objd *od)
{
	int	i;

	od->rd = od->rdata + od->loffs[0];
	i = 0;
	while (i < od->dims[0] && od->rd[i].dist < od->odist)
		++i;
	od->toffs[0] += i;
	od->dims[0] -= i;
	od->loffs[0] += i;
	od->rd += i;
	od->x = -1;
	while (++od->x < od->dims[0] && od->odist <= od->rd[od->x].dist)
		od->tys[od->x] = (int)(od->x * od->tincrs[0]);
	od->dims[0] = od->x;
	if (od->x == 0)
		return ;
	od->loffs[2] = od->loffs[0] + od->x;
	od->so = od->loffs[0] + od->loffs[1] * SCN_WIDTH - 1;
	od->bincr = SCN_WIDTH - od->dims[0];
	od->pxls += (int)(od->toffs[0] * od->tincrs[0])
		+ ((int)(od->toffs[1] * od->tincrs[1]) * od->tex->width);
	od->dbuff = ((float *)cub->renderer.dbuff) + od->so;
	od->pbuff = ((uint32_t *)cub->renderer.objs_layer->pixels) + od->so;
}

void	__render_obj(t_cub *cub, t_objd *od)
{
	uint32_t	*tb;
	int			i;
	int			j;

	adjust_draw_frame(cub, od);
	if (od->dims[0] == 0)
		return ;
	j = -1;
	while (++j < od->dims[1])
	{
		tb = od->pxls + (int)(j * od->tincrs[1]) * od->tex->width;
		i = -1;
		while (++i < od->dims[0])
		{
			++od->dbuff;
			++od->pbuff;
			od->cl = tb[od->tys[i]];
			if (!od->cl || (*od->dbuff && (*od->dbuff < od->odist)))
				continue ;
			*od->pbuff = od->cl;
			*od->dbuff = od->odist;
		}
		od->dbuff += od->bincr;
		od->pbuff += od->bincr;
	}
}

static inline void	init_draw_objects(t_cub *cub, t_objd *od)
{
	memset(cub->renderer.objs_layer->pixels, 0, sizeof(uint32_t)
		* SCN_WIDTH * SCN_HEIGHT);
	memset(cub->renderer.dbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);
	od->pframe = cub->renderer.pframe;
	od->rdata = cub->hero.rcast.rdata;
	od->pdata = cub->hero.rcast.pdata;
	od->isproj = cub->renderer.isproj;
	od->dbuff = cub->renderer.dbuff;
	od->dpbuff = cub->renderer.dpbuff;
	od->obj = cub->objs.instances;
	od->ppos[0] = cub->hero.ply_obj->px;
	od->ppos[1] = cub->hero.ply_obj->py;
}

static inline void	__render_portal_projection(t_cub *cub, t_objd *od)
{
	od->dims[0] = (int)(od->obj->type->proj_width * od->ratio);
	od->dims[1] = (int)(od->obj->type->proj_height * od->ratio);
	od->pframe[0] = od->drawx - (od->dims[0] >> 1);
	od->pframe[2] = ft_clamp(od->pframe[0] + od->dims[0], 0, SCN_WIDTH);
	od->pframe[0] = ft_clamp(od->pframe[0], 0, SCN_WIDTH);
	if (od->pframe[0] == od->pframe[2])
		return ;
	od->pframe[1] = cub->scn_midy - (od->dims[1] >> 1)
		+ (od->ratio * od->obj->type->offy);
	od->pframe[3] = ft_clamp(od->pframe[1] + od->dims[1], 0, SCN_HEIGHT);
	od->pframe[1] = ft_clamp(od->pframe[1], 0, SCN_HEIGHT);
	od->dims[0] = od->pframe[2] - od->pframe[0];
	od->dims[1] = od->pframe[3] - od->pframe[1];
	memset(cub->renderer.dpbuff, 0, sizeof(float) * SCN_WIDTH * SCN_HEIGHT);
	memset(cub->renderer.isproj, 0, sizeof(char) * SCN_WIDTH * SCN_HEIGHT);
	cub->renderer.portal = od->obj;
	__label_isproj((uint32_t *)cub->renderer.objs_layer->pixels,
		od->isproj, od->pframe, od->dims);
	prtl_proj_vectors(od->pdata + od->pframe[0] - 1,
		&cub->map, od->obj, od->pframe[2] - od->pframe[0]);
	order_draw_call(cub->draw_threads, 3, 6);
}

void	render_objects(t_cub *cub)
{
	t_objd	od;

	init_draw_objects(cub, &od);
	while (od.obj)
	{
		if (od.obj == cub->hero.ply_obj && next_obj(&od.obj))
			continue ;
		__rdr_obj_draw_check(cub, &od);
		od.pframe[0] = 0;
		od.pframe[1] = 0;
		od.pframe[2] = SCN_WIDTH;
		od.pframe[3] = SCN_HEIGHT;
		if ((!od.dims[0] || (od.odist <= 1.0f)
				|| __rdr_obj_out_of_frame(&od))
			&& next_obj(&od.obj))
			continue ;
		__rdr_select_draw_texture(&od, od.obj);
		__rdr_setup_draw_objs(cub, &od, od.pframe, od.obj->type->offy);
		__render_obj(cub, &od);
		if (obj_get_type(od.obj) == OBJ_PORTAL && od.obj->isactive)
			__render_portal_projection(cub, &od);
		od.obj = od.obj->next;
	}
}
