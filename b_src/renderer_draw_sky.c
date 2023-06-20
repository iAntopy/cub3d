/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_sky.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 21:28:15 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/19 21:39:20 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

typedef struct s_sky_proj_data
{
	int			*soffy;
	float		*dpbuff;
	char		*isproj;
	uint32_t	*tpx;
	int			twidth;
	int			toffs_x[SCN_WIDTH];
}	t_skyp;

static void	__fill_sky_x_tex_offsets(t_cub *cub, int *tofs, int start, int end)
{
	int		x;
	float	tmp_offx;

	x = start - 1;
	tofs += x;
	while (++x < end)
	{
		tmp_offx = (x - cub->scn_midx) * cub->inv_sw
			* cub->renderer.sky_fov_to_tex + cub->renderer.sky_ori_offset;
		if (tmp_offx < 0.0)
			tmp_offx += cub->tex.skymap->width;
		*(++tofs) = (int)fmodf(tmp_offx, cub->tex.skymap->width);
	}
}

void	__render_proj_sky(t_cub *cub, uint32_t *pbuff, int *pframe)
{
	t_skyp		sp;
	uint32_t	*pb;
	int			*tp;
	int			x;
	int			y;

	__fill_sky_x_tex_offsets(cub, sp.toffs_x, pframe[0], pframe[2]);
	y = pframe[1] - 1;
	while (++y < cub->scn_midy)
	{
		sp.soffy = cub->renderer.sky_yoffsets + y;
		sp.dpbuff = cub->renderer.dpbuff + y * SCN_WIDTH;
		sp.isproj = cub->renderer.isproj + y * SCN_WIDTH;
		tp = sp.toffs_x + pframe[0];
		pb = pbuff + pframe[0] + y * SCN_WIDTH;
		x = pframe[0] - 1;
		while (++x < pframe[2])
		{
			if (sp.isproj[x] && !sp.dpbuff[x])
				*pb = ((uint32_t *)cub->tex.skymap->pixels)[*tp
					+ (*sp.soffy) * cub->tex.skymap->width] & TRANSPARENCY;
			++tp;
			++pb;
		}
	}
}

void	__render_sky(t_cub *cub)
{
	int			texture_xoffsets[SCN_WIDTH];
	uint32_t	*pxls;
	int			*tofs[2];
	int			x;
	int			y;

	tofs[0] = texture_xoffsets - 1;
	__fill_sky_x_tex_offsets(cub, tofs[0], 0, SCN_WIDTH);
	pxls = (uint32_t *)cub->renderer.sky_layer->pixels;
	tofs[1] = cub->renderer.sky_yoffsets - 1;
	y = -1;
	while (++y < (cub->scn_midy - 1))
	{
		tofs[0] = texture_xoffsets - 1;
		++tofs[1];
		x = -1;
		while (++x < SCN_WIDTH)
			*(++pxls) = ((uint32_t *)cub->tex.skymap->pixels)[*(++tofs[0])
				+ (*tofs[1]) * cub->tex.skymap->width];
	}
}
