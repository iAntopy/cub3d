/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 01:09:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/12 15:34:59 by iamongeo         ###   ########.fr       */
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

static uint32_t	*init_column_data(t_cub *cub, t_rdata *rd, t_rcol *rc, int *tw)
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

	clear_image_buffer(cub->renderer.walls_layer);
	rc.walls_layer = cub->renderer.walls_layer;
	i = -1;
	while (++i < SCN_WIDTH)
	{
		pxls = init_column_data(cub, rd + i, &rc, &tex_width);
		j = -1;
		while (++j < rc.scn_height)
		{
			cub_put_pixel(rc.walls_layer, i, rc.scn_start_y + j,
				pxls[(int)(((j - rc.half_height) * rc.ratio)
					+ rc.half_texh) *tex_width]);
		}
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
