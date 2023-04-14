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

int     *color_split(char **txtr, int id);
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

t_map	    *wall_check(t_map *m)
{

  int   o_cells;
  int   hero_x;
  int   hero_y;
  
    o_cells = -1;
    hero_x = -1;
    hero_y = -1;
    printf("\tDEBUG: WALL CHK start (%d , %d ):\n", m->width, m->height); 
    m->pos_y = 0;
    while (m->pos_y < m->height) 
    {
        m->pos_x = 0;
        while (m->pos_x < m->width)
        {                
            o_cells = ft_in_set((&m->tab[m->pos_x][m->pos_y]), (const char *)MAP_CHARS);
            if (o_cells == 0)
            {
                if(m->pos_x - 1)
                {
                    if (ft_in_set(&m->tab[m->pos_x - 1][m->pos_y], (const char *)MAP_CHARS) == -1)
                    {
                        m->flg_chk = 1;
                        return (m);
                    }
                }
                if(m->pos_x + 1)
                {
                    if(ft_in_set(&m->tab[m->pos_x + 1][m->pos_y], (const char *)MAP_CHARS) == -1)
                    {
                        m->flg_chk = 1;
                        return (m);
                    }
                }
                if(m->pos_y - 1)
                {
                    if (ft_in_set(&m->tab[m->pos_x][m->pos_y - 1], (const char *)MAP_CHARS) == -1)
                    {
                        m->flg_chk = 1;
                        return (m);
                    }
                }
                if(m->pos_y + 1)
                {
                    if (ft_in_set(&m->tab[m->pos_x][m->pos_y + 1], (const char *)MAP_CHARS) == -1)
                    {
                        m->flg_chk = 1;
                        return (m);
                    }
                }                
            }
            else if (o_cells > 1)
            {
                if (ft_in_set(&m->tab[m->pos_x][m->pos_y], (const char *)MAP_CHARS) > 5)
                {
                    m->flg_chk = 1;    
                    return (m);
                }

                m->hero_side = o_cells;
                hero_x = m->pos_x;
                hero_y = m->pos_y;     
                printf("\tDEBUG: HERO FOUND  side ref[%d]", m->hero_side);   
                m->flg_chk = 2;               
            }
            m->pos_x++;    
        }
        m->pos_y++;
    }
    if (m->flg_chk == 2)
    {
        m->pos_x = hero_x;
        m->pos_y = hero_y;      
    }
    else 
        m->flg_chk = 1; 
    return (m);
}
