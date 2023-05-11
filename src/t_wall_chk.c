/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/11 18:23:20 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	clr_legend_strct(t_box box)
{
	while (box.xform[box.xnum])
	{
		free(box.xform[box.xnum]);
		box.xnum--;
	}
	free(*box.xform);
}


t_map	*check_hero_found(t_map *m)
{
	if (m->pos_x <= 0 || m->pos_y <= 0)
	{
		report_err("No player charater found in map.");
		m->flg_chk = 1;
		return (m);
	}
	m->flg_chk = 2;
	m->hero_x = m->pos_x;
	m->hero_y = m->pos_y;
	m->m[m->pos_y][m->pos_x] = '0';
	printf("player x[%d]", m->pos_x);
	printf("player y[%d]\n\n", m->pos_y);
	return (m);
}

t_cub	*wall_check(t_cub *cub, t_map *m)
{
	int		o_cells;
	// t_objx 	*objx;
	// int 	id;

	
	// printf("Wall_chk META  %d::: \n", cub->box.meta);
	// objx = (t_objx *)malloc(sizeof(t_objx) * cub->box.meta + 1);
	// cub->box.objx = objx;
	
	// id = 0;
	// o_cells = -1;
	m->pos_y = 0;
	while (m->pos_y < m->height)
	{
		m->pos_x = 0;
		while (m->pos_x < m->width)
		{
			o_cells = ft_in_set((m->m[m->pos_y][m->pos_x]),
					(const char *)cub->box.chrs);
			if (o_cells < 0 && m->m[m->pos_y][m->pos_x] != '\0')
				m->m[m->pos_y][m->pos_x] = 'A';
			else if (o_cells == (int_strlen(cub->box.chrs) - 1))
				m = check_hero_found(m);
			// else if (o_cells < cub->box.meta && o_cells != -1)
			// {
			// 	cub->box.objx = get_pos(cub, m, o_cells, id); 	
			// 	id++;
			// 	cub->box.objx++;
			// }
			m->pos_x++;
		}
		m->pos_y++;
	}
	return (cub);
}

