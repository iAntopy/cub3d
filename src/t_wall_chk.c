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
                // printf("\tDEBUG: WALL CHK start O_cells = %d char {%c}\n", o_cells, (char)m->tab[m->pos_x][m->pos_y]); 
                // printf("chek_pos o_cell \t REFS:[%d] [x:%d][y:%d]\n",  o_cells, m->pos_x, m->pos_y);

                if(m->pos_x - 1)
                {
                    if (ft_in_set(&m->tab[m->pos_x - 1][m->pos_y], (const char *)MAP_CHARS) == -1)
                    {
                        // printf("\tDEBUG: WALL CHK inner x-1 [%d][%d]\n", m->pos_x - 1, m->pos_y);  
                        m->flg_chk = 1;
                        return (m);
                    }
                }
                if(m->pos_x + 1)
                {
                    if(ft_in_set(&m->tab[m->pos_x + 1][m->pos_y], (const char *)MAP_CHARS) == -1)
                    {
                        // printf("\tDEBUG: WALL CHK inner x+1 [%d][%d]\n", m->pos_x +1, m->pos_y);  
                        m->flg_chk = 1;
                        return (m);
                    }
                }
                if(m->pos_y - 1)
                {
                    if (ft_in_set(&m->tab[m->pos_x][m->pos_y - 1], (const char *)MAP_CHARS) == -1)
                    {
                        // printf("\tDEBUG: WALL CHK inner y-1 [%d][%d]\n", m->pos_x, m->pos_y - 1);  
                        m->flg_chk = 1;
                        return (m);
                    }
                }
                if(m->pos_y + 1)
                {
                    if (ft_in_set(&m->tab[m->pos_x][m->pos_y + 1], (const char *)MAP_CHARS) == -1)
                    {
                        // printf("\tDEBUG: WALL CHK inner y+1 [%d][%d]\n", m->pos_x, m->pos_y + 1);
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
                // printf("\t::hero_x[%d]: hero_y[%d] \n", hero_x, hero_y);
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
        printf("\t::hero_x[%d]: hero_y[%d] \n", m->pos_x, m->pos_y);
        printf("DEBUG: WALL CHK SUCCEED o_cells [%d]!!!\n", m->hero_side);
    }
    else 
        m->flg_chk = 1;
    
        // printf("DEBUG:!!! FAIL CHK WALL [%d] o_cells x[%d] y[%d]!!!\n", o_cells, m->pos_x, m->pos_y);	
    return (m);
}
