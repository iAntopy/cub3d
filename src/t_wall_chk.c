/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/31 20:36:51 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	clr_legend_strct(t_box box)
{
	while (box.xform[box.xnum])
	{
		if (box.xform[box.xnum])
			free(box.xform[box.xnum]);
		box.xnum--;
	}
	free(*box.xform);
}


/// get_pos , not get_pos,and adress, and pedigree ... to be sub_div...
t_objx	*get_pos(t_cub *cub, t_map *m, int o_cells, int id)
{
	t_objx *objx;
	
	char 	idx;
	int 	head;

	objx = (t_objx *)malloc(sizeof(t_objx ) * 1);    

	idx = *ft_substr(cub->box.chrs, o_cells, 1);
	objx->obj_id = id;		       	//	OR _Index_ (obj_int_id) 'i' ... "maybe both!"
	objx->name = idx;				//	call _Name_ should be (obj_char_name) '#' 

	head = (ft_in_set(idx, (const char *)MAP_MCHR));
	if (head != -1)
	{
		// printf("get_pos: objx type attribution : head %d\n", head);  
		// printf("get_pos: objx type attribution : type %d\n", objx->o_type); 
		if (head < 4)
			objx->o_type = OBJ_LEVER;
		else if (head < 12)
			objx->o_type = OBJ_PORTAL;
		else if (head < 15)
			objx->o_type = OBJ_FIREPIT;
		else if (head < 18)
			objx->o_type = OBJ_FIREBALL;
		else if (head < 21)
			objx->o_type = OBJ_PLAYER;
		else
			objx->o_type = OBJ_FLAG;
		
		
		objx->opos[0] = m->pos_x;		/// main event! 
		objx->opos[1] = m->pos_y;

		objx->relativ =  m->raw[o_cells][4];	
		objx->alleg = m->raw[o_cells][2] - 48; //ALI_TORRENT;			

		if (objx->o_type == OBJ_PLAYER)
			objx->alleg = ALI_TORRENT;	
		// printf("META_ID[%d]_typ[%d]_name{%c}__",objx->obj_id, objx->o_type, objx->name);
		// printf("Alg[%d]__Rel{%c}__",  objx->alleg, objx->relativ);
		// printf("(x[%d],y[%d])((head:%d))\n\n", objx->opos[0], objx->opos[1], head);	
	}
	if (m->pos_x <= 0 || m->pos_y <= 0)
	{
		report_err("No META char found in map.");
		m->flg_chk = 1;
		return (objx);
	}
	return (objx);
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
	return (m);
}

t_cub	*wall_check(t_cub *cub, t_map *m)
{
	t_objx 	**objx;
	int		o_cells;
	int 	id;
	
	printf("Wall_chk META  %d::: \n", cub->box.meta);
	objx = (t_objx **)malloc(sizeof(t_objx *) * cub->box.meta + 1);
	
	id = -1;
	o_cells = -1;
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
			{	
				m = check_hero_found(m);
				id++;
				objx[id] = get_pos(cub, m, o_cells, id);
				
			}
			else if (o_cells < cub->box.meta - 1 && o_cells > -1)
			{
				id++;
				objx[id] = get_pos(cub, m, o_cells, id);
			}
			m->pos_x++;
		}
		m->pos_y++;
	}
	cub->box.objx = objx;
	return (cub);
}
				// printf("WALLCHK_ID[%d] O_CELL[%d]\n", id, o_cells);
				// printf("WALLCHK_ID[%d] O_CELL[%d] {%c}\n", id, o_cells, cub->box.chrs[o_cells]);
				// printf("WALLCHK_ID[%d] O_CELL[%d] {%c}\n", id, o_cells, cub->box.chrs[o_cells]);
