/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_funcs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 19:03:24 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 16:09:11 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline uint32_t	*init_wcol(t_cub *cub, t_rdata *rd, t_rcol *rc, int *tw)
{
	mlx_texture_t	*tex;
	int				tex_start_x;

	tex = cub->map.mx[rd->cy][rd->cx]->xwalls[rd->side];
	*tw = tex->width;
	rc->half_texh = (tex->height >> 1);
	tex_start_x = (int)(rd->tex_ratio * tex->width);
	rc->scn_height = ft_clamp(rd->tex_height, 0, SCN_HEIGHT);
	rc->half_height = (rc->scn_height >> 1);
	rc->ratio = (float)tex->height / (float)rd->tex_height;
	rc->scn_start_y = ((SCN_HEIGHT - rc->scn_height) >> 1);
	return ((uint32_t *)tex->pixels + tex_start_x);
}

// rd is ptr to array of raycasting results data (array len = SCN_WIDTH).
void	render_walls(t_cub *cub)
{
	int			i;
	int			j;
	t_rcol		rc;
	uint32_t	*pxls;
	int			tex_width;
	t_rdata		*rd;

	rd = cub->hero.rcast.rdata;
	clear_image_buffer(cub->renderer.walls_layer);
	i = -1;
	while (++i < SCN_WIDTH)
	{
		pxls = init_wcol(cub, rd + i, &rc, &tex_width);
		j = -1;
		while (++j < rc.scn_height)
			cub_put_pixel(cub->renderer.walls_layer, i, rc.scn_start_y + j,
				pxls[(int)(((j - rc.half_height) * rc.ratio)
					+ rc.half_texh) *tex_width]);
	}
}
