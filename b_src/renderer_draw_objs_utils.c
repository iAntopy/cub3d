/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_objs_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 03:54:32 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/27 01:38:43 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

inline int	__rdr_obj_out_of_frame(t_objd *od)
{
	return (((od->drawx + (od->dims[0] >> 1)) < od->pframe[0])
		|| (od->pframe[2] <= (od->drawx - (od->dims[0] >> 1))));
}

// Assumes is_drawable == true, checked earlier.
inline void	__rdr_select_draw_texture(t_objd *od, t_oinst *obj)
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

inline void	__rdr_setup_draw_objs(t_cub *cub, t_objd *od, int *pframe, int offy)
{
	od->tincrs[0] = (float)od->tex->width / (float)od->dims[0];
	od->tincrs[1] = (float)od->tex->height / (float)od->dims[1];
	memset(od->toffs, 0, sizeof(int) * 2);
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
		od->toffs[1] = od->pframe[1] - od->loffs[1];
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

inline void	__rdr_obj_draw_check(t_cub *cub, t_objd *od)
{
	od->dims[0] = 0;
	if (!od->obj->type->is_drawable || !od->obj->gset)
		return ;
	od->ov[0] = od->obj->px - od->ppos[0];
	od->ov[1] = od->obj->py - od->ppos[1];
	od->odist = (*cub->hero.dirx) * od->ov[0] + (*cub->hero.diry) * od->ov[1];
	od->obj->dist = od->odist;
	od->ratio = cub->near_z / od->odist;
	od->drawx = (int)(((*cub->hero.dirx) * od->ov[1] - (*cub->hero.diry)
				* od->ov[0]) * od->ratio) + cub->scn_midx;
	od->dims[0] = (int)(od->ratio * od->obj->type->width);
	od->dims[1] = (int)(od->ratio * od->obj->type->height);
	return ;
}

inline void	__label_isproj(uint32_t *pb, char *ip, int *pf, int *pdims)
{
	const int	start_offset = pf[0] + (pf[1] * SCN_WIDTH);
	const int	buff_jump = SCN_WIDTH - pdims[0];
	int			w;
	int			h;

	pb += start_offset - 1;
	ip += start_offset - 1;
	h = pdims[1];
	while (h--)
	{
		w = pdims[0];
		while (w--)
		{
			++ip;
			if (*(++pb) == PROJ_COLOR)
				*ip = 1;
		}
		pb += buff_jump;
		ip += buff_jump;
	}
}
