/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_wall_chk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:23:11 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/18 20:52:45 by gehebert         ###   ########.fr       */
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

// typedef struct s_objx
	// {
	// 	char 			name;		// '#' 
	// 	int				obj_id;		 // enrg. id	
	// 	int 			opos[2];	// relativ pos (reltv. obj_id)
	// 	t_oinst			*wobj;		// world object instance
	// 	///							/// get_pos has split job...
		
	// 	int				o_type;		// model_type
	// 	int 			alleg;		// allegence _txtr
	// 	char			relativ;	// char obj_id
	// 	struct s_objx	*rel_ref;	 // ptr to its relative's objx ptr;
// }	t_objx;


// t_objx	*get_ref(t_cub *cub, t_objx *objx, int id)
	// {
	// 	char ext_ref;
	// 	// int ext_id;

	// 	ext_ref = cub->box.chrs[id];

	// 	printf("_(ext_ref[%c] ::chrs{%s}::\n\n", ext_ref, cub->box.chrs);
	// 	// ext_ref = cub->box.objx[id]->relativ;
	// 	// ext_id = ft_in_set(ext_ref, (const char *)cub->box.chrs); 
	// 	// *ptr objx[id] // objx->[chrs[id]] ....  ref by header_file
	// 	// objx->rel_ref = cub->box.objx[ext_id];
		
	// 	// objx->relativ =   ext_ref;//cub->box.chrs[id];
	// 	//m->raw[o_cells][4];			// objx->relativ =  *objx[0]->'#' 	
	// 	// 	enum type alleg (0-3)  
	// 		// objx->alleg = ALI_TORRENT;			// 	objx->alleg = m->raw[o_cells][2] - 48;		
	// 	return (objx);	
// }

// /// add new id_member
		// 	objx = (t_objx *)malloc(sizeof(t_objx ) * 1);    
		// 	idx = *ft_substr(cub->box.chrs, o_cells, 1);
			
		// 	objx->name = idx;//cub->box.chrs[o_cells];		 // '#' char name 
		// 	objx->obj_id = id;		          
		// 	if (o_cells < cub->box.n_lvls)
		// 		objx->o_type = 2;
		// 	else if (o_cells < (cub->box.n_prts + cub->box.n_lvls))
		// 		objx->o_type = 1;
		// 	else
		// 		objx->o_type = 3;
		// 	// printf("META_ID[%d]typ[%d](Name//Alleg//Reltv)::{%c}::",objx->obj_id, objx->o_type, objx->name);
		// 	objx->alleg = m->raw[o_cells][2] - 48;		    // txtr_ref
		// 	objx->relativ =  m->raw[o_cells][4];
		// 	// printf("[%d]::{%c}",  objx->alleg, objx->relativ);
		// 	objx->opos[0] = m->pos_x;
		// 	objx->opos[1] = m->pos_y;
		// 	// printf("_(x[%d], y[%d])::\n\n", 

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

			// {
			// 	printf("*** PLAYER {%c}***\n", idx);
			// }
			// {
			// 	printf("*** LEVER[%d]::{%c}***\n", head, idx);	
			// }
			// {
			// 	printf("*** PORTAL[%d]::{%c}***\n", head, idx);	
			// }
			// {
			// 	printf("*** FIREBALL[%d]::{%c}***\n", head, idx);	
			// }
			// {
			// 	printf("*** FIREPIT[%d]::{%c}***\n", head, idx);	
			// }
			// {
			// 	printf("*** SPEC[%d]::{%c}***\n", head, idx);	
			// }
	head = (ft_in_set(idx, (const char *)MAP_MCHR)); 
	if (head == 18)
		objx->o_type = 5;
	else if (head < 4)
		objx->o_type = 1;
	else if (head < 12)
		objx->o_type = 2;
	else if (head > 11 &&  head < 15)
		objx->o_type = 3;
	else if (head > 14 &&  head < 18)
		objx->o_type = 4;
	else if (head > 17)
		objx->o_type = 6;
	
	objx->opos[0] = m->pos_x;		/// main event! 
	objx->opos[1] = m->pos_y;

	objx->relativ =  m->raw[o_cells][4];	
	objx->alleg = m->raw[o_cells][2] - 48; //ALI_TORRENT;			

	if (objx->o_type == 5)//0 && objx->o_type < 7)
		objx->alleg = ALI_TORRENT;
		
	
	printf("META_ID[%d]typ[%d](Name//Alleg//Reltv)::{%c}::",objx->obj_id, objx->o_type, objx->name);
	printf("[%d]::{%c}\n",  objx->alleg, objx->relativ);
	printf("_(x[%d], y[%d])::\n\n", objx->opos[0], objx->opos[1]);	
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
	// m->m[m->pos_y][m->pos_x] = '0';
	// printf("player x[%d]", m->pos_x);
	// printf("player y[%d]\n\n", m->pos_y);
	return (m);
}

t_cub	*wall_check(t_cub *cub, t_map *m)
{
	t_objx 	**objx;
	// const char	*chr_name;
	int		o_cells;
	int 	id;
	
	printf("Wall_chk META  %d::: \n", cub->box.meta);
	objx = (t_objx **)malloc(sizeof(t_objx *) * cub->box.meta + 1);
	
	id = 0;
	// chr_name = cub->box.chrs;
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
				objx[id] = get_pos(cub, m, o_cells, id);
				// p_list_objx(cub->box.objx , id, 0); 
				id++;
				
			}
			else if (o_cells < cub->box.meta && o_cells != -1)
			{
				
				objx[id] = get_pos(cub, m, o_cells, id);
				id++;
			}
			m->pos_x++;
		}
		m->pos_y++;
	}
	cub->box.objx = objx;
	// int ox = 0;
	// p_list_objx(cub->box.objx , ox, cub->box.meta); // objx, id, how-many to list
	return (cub);
}

