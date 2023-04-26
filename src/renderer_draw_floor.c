/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_floor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 17:27:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/26 17:27:19 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static uint32_t	floor_get_pixel(mlx_texture_t *tex, int x, int y)
{
	if (x < 0 || y < 0)
		return (0);
	return (((uint32_t *)tex->pixels)[x + y * tex->width]);
}

static void	__render_floor_init(t_cub *cub, uint32_t **b, float **p)
{
	*p = cub->renderer.floor_factors - 1;
	b[0] = (uint32_t *)cub->renderer.bg_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy + 1)) - 1;
	b[1] = (uint32_t *)cub->renderer.walls_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy + 1)) - 1;
}

static void	__render_floor_incr(float **ps, const float **rays, uint32_t **bs)
{
	++(*ps);
	++rays[0];
	++rays[1];
	++(bs[0]);
	++(bs[1]);
}

void	render_floor(t_cub *cub, t_rdata *rd)
{
	const float		flr_ratios[2] = {cub->flrw_to_cw, cub->flrh_to_cw};
	const float		*rays[2] = {rd[0].rx, rd[0].ry};
	float			*params;
	uint32_t		*buffs[2];
	int				incr[2];

	__render_floor_init(cub, buffs, &params);
	incr[1] = 0;
	while (++incr[1] < cub->scn_midy)
	{
		incr[0] = -1;
		while (++incr[0] < SCN_WIDTH)
		{
			__render_floor_incr(&params, rays, buffs);
			if (*buffs[1])
				continue ;
			*buffs[0] = floor_get_pixel(cub->floor_tex,
					(int)(fmodf(*rays[0] * (*params) + cub->hero.px, CELL_WIDTH)
						* flr_ratios[0]),
					(int)(fmodf(*rays[1] * (*params) + cub->hero.py, CELL_WIDTH)
						* flr_ratios[1]));
		}
		rays[0] = rd[0].rx;
		rays[1] = rd[0].ry;
	}
}	
