/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/13 22:02:12 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	renderer_clear(t_cub *cub)
{
	printf("renderer clear entered \n");
	if (cub->renderer.bg_layer)
		mlx_delete_image(cub->mlx, cub->renderer.bg_layer);
	if (cub->renderer.walls_layer)
		mlx_delete_image(cub->mlx, cub->renderer.walls_layer);
	return (0);
}

static uint32_t	*init_column_data(t_cub *cub, t_rdata *rd, t_rcol *rc)
{
	rc->tex = cub->tex.walls[rd->side];
	rc->half_texh = (rc->tex->height >> 1);
	rc->tex_start_x = (int)(rd->tex_ratio * rc->tex->width);
	rc->scn_height = ft_clamp(rd->tex_height, 0, SCN_HEIGHT);
	rc->half_height = (rc->scn_height >> 1);
	rc->ratio = (float)rc->tex->height / (float)rd->tex_height;
	rc->scn_start_y = ((SCN_HEIGHT - rc->scn_height) >> 1);
	return ((uint32_t *)rc->tex->pixels + rc->tex_start_x);
}

void	render_walls(t_cub *cub)
{
	int			i;
	int			j;
	t_rcol		rc;
	uint32_t	*pxls;

	clear_image_buffer(cub->renderer.walls_layer);
	i = -1;
	while (++i < SCN_WIDTH)
	{
		pxls = init_column_data(cub, cub->hero.rcast.rdata + i, &rc);
		j = -1;
		while (++j < rc.scn_height)
			cub_put_pixel(cub->renderer.walls_layer, i, rc.scn_start_y + j,
				pxls[(int)(((j - rc.half_height) * rc.ratio)
					+ rc.half_texh) *rc.tex->width]);
	}
	cub->renderer.requires_update = 0;
}

// Called only once at start to initialize mlx image buffers by layer.
int	init_renderer(t_cub *cub)
{
	cub->renderer.bg_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	cub->renderer.walls_layer = mlx_new_image(cub->mlx, SCN_WIDTH, SCN_HEIGHT);
	if (!cub->renderer.bg_layer || !cub->renderer.walls_layer)
		return (-1);
	mlx_set_color_in_rows(cub->renderer.bg_layer,
		0, SCN_HEIGHT >> 1, cub->tex.color[0]);
	mlx_set_color_in_rows(cub->renderer.bg_layer,
		SCN_HEIGHT >> 1, SCN_HEIGHT, cub->tex.color[1]);
	mlx_image_to_window(cub->mlx, cub->renderer.bg_layer, 0, 0);
	mlx_image_to_window(cub->mlx, cub->renderer.walls_layer, 0, 0);
	return (0);
}
