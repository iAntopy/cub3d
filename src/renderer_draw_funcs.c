/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_draw_funcs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 19:03:24 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/25 03:45:10 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline uint32_t	*init_wall_column_data(t_cub *cub, t_rdata *rd, t_rcol *rc, int *tw)
{
	mlx_texture_t	*tex;
	int				tex_start_x;

	tex = cub->tex.walls[rd->side];
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
void	render_walls(t_cub *cub, t_rdata *rd)
{
	int			i;
	int			j;
	t_rcol		rc;
	uint32_t	*pxls;
	int			tex_width;

//	printf("render_walls started \n");
	clear_image_buffer(cub->renderer.walls_layer);
	rc.walls_layer = cub->renderer.walls_layer;
	i = -1;
	while (++i < SCN_WIDTH)
	{
		pxls = init_wall_column_data(cub, rd + i, &rc, &tex_width);
		j = -1;
		while (++j < rc.scn_height)
		{
			cub_put_pixel(rc.walls_layer, i, rc.scn_start_y + j,
				pxls[(int)(((j - rc.half_height) * rc.ratio)
					+ rc.half_texh) *tex_width]);
		}
	}
	cub->renderer.requires_update = 0;
//	printf("render_walls DONE \n");
}

static uint32_t	floor_get_pixel(mlx_texture_t *tex, int x, int y)
{
	if (x < 0 || y < 0)
		return (0);
	return (((uint32_t *)tex->pixels)[x + y * tex->width]);
}

void	render_floor(t_cub *cub, t_rdata *rd)
{
//	const int	param_stride = sizeof(float) * SCN_WIDTH;
	const float	flr_texw_to_cellw = cub->inv_cw * cub->floor_tex->width;
	const float	flr_texh_to_cellw = cub->inv_cw * cub->floor_tex->height;
	float		*params;
	uint32_t	*pxls;
	uint32_t	*pxls_r;
	t_rdata		*r;
	t_rdata		*rr;
	int			iter_y;
	int			iter_x;
	
	printf("render floor entered \n");

	params = cub->renderer.floor_factors - 1;
	pxls = (uint32_t *)cub->renderer.bg_layer->pixels
		+ (SCN_WIDTH * (cub->scn_midy + 1)) - 1;
//	printf("pxls (x, y) : (%ld, %ld)\n", (pxls - (uint32_t *)cub->renderer.bg_layer->pixels) % SCN_WIDTH, (pxls - (uint32_t *)cub->renderer.bg_layer->pixels) / SCN_WIDTH);
	pxls_r = pxls + SCN_WIDTH;
	iter_y = cub->scn_midy;
	while (iter_y--)//++i < SCN_HEIGHT)
	{
		r = rd - 1;
		rr = rd + SCN_WIDTH;
		pxls_r = pxls + SCN_WIDTH;
		iter_x = cub->scn_midx;
		while (iter_x--)//++j < cub->scn_midx)
		{
			++r;
			--rr;
			++params;
			*(++pxls) = floor_get_pixel(cub->floor_tex, (int)(fmodf((*r->rx) * (*params)
				+ (*r->px), CELL_WIDTH) * flr_texw_to_cellw),
				(int)(fmodf((*r->ry) * (*params) + (*r->py), CELL_WIDTH)
						* flr_texh_to_cellw));
			*(--pxls_r) = floor_get_pixel(cub->floor_tex, (int)(fmodf((*rr->rx) * (*params)
				+ (*rr->px), CELL_WIDTH) * flr_texw_to_cellw),
				(int)(fmodf((*rr->ry) * (*params) + (*rr->py), CELL_WIDTH)
						* flr_texh_to_cellw));
		}
		params += cub->scn_midx;
		pxls += cub->scn_midx;
	}
}
