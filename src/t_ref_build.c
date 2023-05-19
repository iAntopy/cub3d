/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 21:34:03 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/18 19:54:32 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"


t_matrx	*gset_builder(const char *path, int txtr_nb)// t_box *box)
{
	t_matrx *gset;
	char *arr_name; 
	char *name;
	int i;

	gset = (t_matrx *)malloc(sizeof(mlx_texture_t ) * txtr_nb);	
	if (!gset)
		return (NULL);
	i = 0;
	printf("GSET txtr >>>> *%s Model >>> \n", path);
	while (i < txtr_nb )
	{

		name = ft_strjoin(path, ft_itoa(i));
		arr_name = ft_strjoin((const char *)name, ".png");
		
		printf(">>>> GSET: [%d]  >>>{%s}", i, arr_name);
		gset->xwalls[i] = mlx_load_png(arr_name);
		if (!gset->xwalls[i])
			return (report_mlx_tex_load_failed(arr_name));
 		printf(">> >> ptr : %p i: %d\n", gset->xwalls[i] ,i);
		i++;
		// free(name);
		// free(arr_name);
	}
	return (gset);
}


t_cub	*dual_builder(t_cub *cub, int i, char *t_name)
{
	char	**tex_set;

    if (cub->box.n_dual > i)
    {		
		tex_set = ft_split_space(t_name);
	
		cub->dual[i].xwalls[0] = mlx_load_png(tex_set[0]);
		if (!cub->dual[i].xwalls[0])
			return (report_mlx_tex_load_failed(tex_set[0]));
        if (tex_set[1])/// not open_sky
        {	
			cub->dual[i].xwalls[1] = mlx_load_png(tex_set[1]);
			if (!cub->dual[i].xwalls[1])
				return (report_mlx_tex_load_failed(tex_set[1]));
			printf("DUAL[%d] (xwall[1]) >> ptr : %p \n", i, &cub->dual[i].xwalls[1]);
        }   

    }
    return (cub);
}

t_cub	*meta_builder(t_cub *cub, t_box *box, char *t_name, t_objs *objs)
{

	
	if (ft_in_set(t_name[0], (const char *)MOD_LEV) != -1)		//// lever
	{
		printf(">>>>	MODEL : LVLS [%d] >>>\n", box->n_lvls);
		if (box->n_lvls == 0)
			init_lever_model(objs);
		box->n_lvls++;
	}
	else if (ft_in_set(t_name[0], (const char *)MOD_PORT) != -1)
	{
		printf(">>>>	MODEL : PORTAL [%d] >>>\n", box->n_prts);
		if (box->n_prts == 0)
			init_portal_model(objs);
		box->n_prts++;
	}
	else if (ft_in_set(t_name[0], (const char *)MOD_FIRE) != -1)
	{
		printf(">>>>	MODEL : FB [%d] >>>\n", box->n_fbll);
		if (t_name[0] == '*' && box->n_fbll == 0)
		{
			init_fireball_model(objs);
			box->n_fbll++;
		}
		if (t_name[0] == '#' && box->n_fpit == 0)
		{
			init_firepit_model(objs);
			box->n_fpit++;
		}
	}
	else  if (ft_in_set(t_name[0], (const char *)MOD_SPEC) != -1)	
	{
		printf(">>>>	MODEL : PLAYER [%d] >>>\n", box->n_plyr);
		if (box->n_plyr == 0)
			init_player_model(objs);
		box->n_plyr++;
	}
	// printf(">>>> TEST >>> [%c]\n", t_name[0]);   			
    return (cub);
}

/// MAPX BUILDER
t_cub	*mapx_builder(t_map *m, t_cub *cub)
{
	int			k;
	int			p_box;
	const char	*chrs;
	int 		grim;
	int 		max;
	chrs = cub->box.chrs;
	printf("MAPX:start ..., chrs : %s\n", chrs);
	grim = cub->box.meta + cub->box.n_dual;
	max = (int)ft_strlen(chrs) - 1;
	k = -1;
	m->mx = (t_matrx ***)calloc(sizeof(t_matrx **), m->height);
	while (++k < m->height)
		m->mx[k] = (t_matrx **)calloc(sizeof(t_matrx *), m->width);
	m->pos_y = 0;
	while (m->pos_y < m->height)
	{
		m->pos_x = 0;
		while (m->pos_x < m->width)
		{
			p_box = ft_in_set((m->m[m->pos_y][m->pos_x]), chrs);
			if (p_box != -1)
			{
				if ((p_box < max - (cub->box.pset - 2)) && (p_box > max - (cub->box.pset + cub->box.n_dual - 1)))
				{
					m->mx[m->pos_y][m->pos_x] = &cub->pset[p_box - grim];
				}
				if ((p_box <= max - (cub->box.pset + 1)) && (p_box - cub->box.meta >= 0)) /// floor + ceil
				{
					m->mx[m->pos_y][m->pos_x] = &cub->dual[p_box - cub->box.meta];
				}
				if (p_box == max)
				{
					cub->box.meta++;
					// p_list_objx(cub->box.objx , p_box, 0); 					
				}	
			}
			m->pos_x++;
		}
		m->pos_y++;
	}
	printf(" ... MAPX:exit\n\n");
	return (cub);
}

//			if (p_box < cub->box.meta )
	//			{
	//				printf("MapX >> (%d, %d)>> p_box[%d]: ptr:%p\n", m->pos_y, m->pos_x, p_box, &cub->pset[p_box]);
	//				printf("FOUND IT ");
	//				p_list_objx(cub->box.objx , p_box, 0); 
	//			}
// printf("MAPX>> dual[%d]->xwalls[0] : %p\n", p_box - cub->box.meta, &cub->dual[p_box - cub->box.meta].xwalls[0]);
					// printf("new pset %p, xwalls[0] : %p\n", m->mx[m->pos_y][m->pos_x], m->mx[m->pos_y][m->pos_x]->xwalls[0]);
