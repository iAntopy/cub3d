/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:39:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/13 19:17:37 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	raycaster_clear(t_rcast *rcast, int exit_status)
{
	mtx_clear_list(4, rcast->theta_offs, rcast->ray_thetas,
		rcast->rays[0], rcast->rays[1]);
	ft_free_p((void **)&rcast->rdata);
	return (exit_status);
}

static void	init_raydata_consts(t_cub *cub, t_rcast *rcast)
{
	t_rdata	*rd;

	rd = rcast->rdata - 1;
	while ((++rd - rcast->rdata) < SCN_WIDTH)
	{
		rd->idx = (rd - (rcast->rdata));
		rd->rcast = rcast;
		rd->pcx = &cub->hero.cell_x;
		rd->pcy = &cub->hero.cell_y;
		rd->px = &cub->hero.px;
		rd->py = &cub->hero.py;
		rd->p_dirx = cub->hero.dirx;
		rd->p_diry = cub->hero.diry;
		rd->rx = _mtx_index_fptr(rcast->rays[0], rd->idx, 0);
		rd->ry = _mtx_index_fptr(rcast->rays[1], rd->idx, 0);
	}
}

int	init_raycaster(t_cub *cub)
{
	t_rcast	*rcast;

	rcast = &cub->hero.rcast;
	rcast->cub = cub;
	rcast->map = &cub->map;
	rcast->theta_offs = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	rcast->ray_thetas = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	rcast->rays[0] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	rcast->rays[1] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	rcast->rdata = malloc(sizeof(t_rdata) * SCN_WIDTH);
	if (!rcast->theta_offs || !rcast->ray_thetas || !rcast->ray_thetas
		|| !rcast->rays[0] || !rcast->rays[1] || !rcast->rdata)
		return (raycaster_clear(rcast, EXIT_FAILURE));
	cub->hero.fov_lx = _mtx_index_fptr(rcast->rays[0], 0, 0);
	cub->hero.fov_ly = _mtx_index_fptr(rcast->rays[1], 0, 0);
	cub->hero.dirx = _mtx_index_fptr(rcast->rays[0], SCN_WIDTH / 2, 0);
	cub->hero.diry = _mtx_index_fptr(rcast->rays[1], SCN_WIDTH / 2, 0);
	cub->hero.fov_rx = _mtx_index_fptr(rcast->rays[0], SCN_WIDTH - 1, 0);
	cub->hero.fov_ry = _mtx_index_fptr(rcast->rays[1], SCN_WIDTH - 1, 0);
	init_raydata_consts(cub, rcast);
	update_fov(cub, FOV);
	return (0);
}
