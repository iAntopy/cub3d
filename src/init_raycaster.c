/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:39:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/17 20:40:52 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	raycaster_clear(t_rcast *rcast, int exit_status)
{
	mtx_clear_list(5, rcast->theta_offs, rcast->fwd_rayspan, rcast->ray_thetas,
		rcast->rays[0], rcast->rays[1]);
	ft_free_p((void **)&rcast->rdata);
	ft_free_p((void **)&rcast->prtl_proj);
	return (exit_status);
}

static void	init_rdata_consts(t_cub *cub, t_rcast *rc, t_rdata *rd, t_pdata *pd)
{
	int	i;

	i = -1;
	while (++i < SCN_WIDTH)
	{
		rd[i].idx = i;
		rd[i].inv_cw = cub->inv_cw;
		rd[i].near_proj_factor = &cub->near_proj_factor;
		rd[i].pcx = &cub->hero.ply_obj->cx;//cell_x;
		rd[i].pcy = &cub->hero.ply_obj->cy;//cell_y;
		rd[i].px = &cub->hero.ply_obj->px;//px;
		rd[i].py = &cub->hero.ply_obj->py;//px;
		rd[i].p_dirx = cub->hero.dirx;
		rd[i].p_diry = cub->hero.diry;
		rd[i].rx = _mtx_index_fptr(rc->rays[0], i, 0);
		rd[i].ry = _mtx_index_fptr(rc->rays[1], i, 0);
		pd[i].rdata = rd + i;
		pd[i].fwd_len = _mtx_index_fptr(rc->fwd_rayspan, i, 0);
	}
}

// Depends on/Assumes cub->hero.ply_obj is initialized
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
	rcast->fwd_rayspan = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	rcast->rdata = malloc(sizeof(t_rdata) * SCN_WIDTH);
	rcast->prtl_proj = malloc(sizeof(t_pdata) * SCN_WIDTH);
	if (!rcast->theta_offs || !rcast->ray_thetas || !rcast->ray_thetas
		|| !rcast->rays[0] || !rcast->rays[1] || !rcast->rdata
		|| !rcast->prtl_proj)
		return (raycaster_clear(rcast, EXIT_FAILURE));
	cub->hero.fov_lx = _mtx_index_fptr(rcast->rays[0], 0, 0);
	cub->hero.fov_ly = _mtx_index_fptr(rcast->rays[1], 0, 0);
	cub->hero.dirx = _mtx_index_fptr(rcast->rays[0], SCN_WIDTH / 2, 0);
	cub->hero.diry = _mtx_index_fptr(rcast->rays[1], SCN_WIDTH / 2, 0);
	cub->hero.fov_rx = _mtx_index_fptr(rcast->rays[0], SCN_WIDTH - 1, 0);
	cub->hero.fov_ry = _mtx_index_fptr(rcast->rays[1], SCN_WIDTH - 1, 0);
	init_rdata_consts(cub, rcast, rcast->rdata, rcast->prtl_proj);
	update_fov(cub, FOV);
	return (0);
}
