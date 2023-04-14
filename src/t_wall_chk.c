/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/29 13:03:57 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"


//intru
int *color_split(t_cub *cub, char **txtr, int id)
{
    char	**color;

    color = ft_split(txtr[1], ',');
    if (id == 4)
        cub->tex.color[0] = str_to_color(ft_atoi(color[0]),
            ft_atoi(color[1]), ft_atoi(color[2]), 0xff);
    else if (id == 5)
        cub->tex.color[1] = str_to_color(ft_atoi(color[0]),
            ft_atoi(color[1]), ft_atoi(color[2]), 0xff);
 
}
//

// int t_cell_chk(t_map *m)
// {
//     int   o_cells;

//     o_cells = -1;
//     m->pos_y = 0;
//     while (m->pos_y < m->height) 
//     {
//         m->pos_x = 0;
//         while (m->pos_x < m->width)
//         { 
//             o_cells = ft_in_set((&m->tab[m->pos_x][m->pos_y]), (const char *)MAP_CHARS);
//              if (o_cells == 0)
//                 t_o_cell(m, m->pos_x, m->pos_y);
//             else if (o_cells > 1)

//         }
//     }
// }

static t_map *t_o_cell(t_map *m, int m_x, m_y)
{
    while(m_x != 0 && m_y != 0 && m_x < m->width && m_y < m->height)
    {
        if(m_x - 1)
            if (ft_in_set(&m->tab[m->pos_x - 1][m->pos_y], (const char *)MAP_CHARS) == -1)
                return (m->flg_chk = 1);
        if(m_x + 1)
            if(ft_in_set(&m->tab[m->pos_x + 1][m->pos_y], (const char *)MAP_CHARS) == -1)
                return (m->flg_chk = 1);
        if(m_y - 1)
            if (ft_in_set(&m->tab[m->pos_x][m->pos_y - 1], (const char *)MAP_CHARS) == -1)
                return (m->flg_chk = 1);
        if(m_y + 1)
            if (ft_in_set(&m->tab[m->pos_x][m->pos_y + 1], (const char *)MAP_CHARS) == -1)
                return (m->flg_chk = 1);
        return (m->flg_chk = 0);
    }
    return (m->flg_chk = 1);
}

//
static t_map *t_hero_cell(t_map *m, int m_x, int m_y)
{
    int     o_cells;

    o_cells = ft_in_set(&m->tab[m_x][m_y], (const char *)MAP_CHARS);
    if (o_cells > 5)
        return (m->flg_chk = 1);
    else 
    {
        m->hero_side = o_cells;
        m->hero_x = m->pos_x;
        m->hero_y = m->pos_y;     
        m->flg_chk = 2;               
    }
    return(m);
}
//
t_map	    *wall_check(t_map *m)
{
    int   o_cells;
  
    o_cells = -1;
    printf("\tDEBUG: WALL CHK start (%d , %d ):\n", m->width, m->height); 
    m->pos_y = 0;
    while (m->pos_y <= m->height && m->flg_chk != 1) 
    {
        m->pos_x = 0;
        while (m->pos_x <= m->width && m->flg_chk != 1)
        { 
            o_cells = ft_in_set((&m->tab[m->pos_x][m->pos_y]), (const char *)MAP_CHARS);
            if (o_cells == 0)
                t_o_cell(m, m->pos_x, m->pos_y);
            else if (o_cells > 1)
                t_hero_cell(m, m->pos_x, m->pos_y);
            m->pos_x++;
        }
        m->pos_y++;
    }

    return (m);
}
