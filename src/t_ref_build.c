/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 21:34:03 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/03 01:47:06 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"


t_matrx	*gset_builder(const char *path, int txtr_nb)// t_box *box)
{
	t_matrx *gset;
	char *arr_name; 
	char *name;
	int i;

	gset = (t_matrx *)malloc(sizeof(t_matrx));	
	if (!gset)
		return (NULL);
	i = 0;
	printf("GSET %d txtrs >>>> *%s Model >>> \n", txtr_nb, path);
	while (i < txtr_nb)
	{
		name = ft_strjoin(path, ft_itoa(i));
		arr_name = ft_strjoin((const char *)name, ".png");
		
		// printf(">>>> GSET: [%d]  >>>{%s}", i, arr_name);
		gset->xwalls[i] = mlx_load_png(arr_name);
		// printf("gset->xwalls[%d] : %p\n", i, gset->xwalls[i]);
		if (!gset->xwalls[i])
			return (report_mlx_tex_load_failed(arr_name));
 		// printf(">> >> ptr : %p i: %d, tex ptr : %p\n", gset->xwalls[i] ,i, gset->xwalls[i]);
		i++;
		// free(name);
		// free(arr_name);
	}
	// printf("Returning from gset builder avec gset = %p\n", gset);
	return (gset);
}


t_cub	*dual_builder(t_cub *cub, int i, char *t_name)
{
	char	**tex_set;

    if (cub->box.n_dual > i)
    {		
		tex_set = ft_split_space(t_name);
		cub->dual[i].xwalls[1] = NULL;
	
		cub->dual[i].xwalls[0] = mlx_load_png(tex_set[0]);
		if (!cub->dual[i].xwalls[0])
			return (report_mlx_tex_load_failed(tex_set[0]));
		printf("DUAL[%d] (xwall[0]) >> ptr : %p \n", i, &cub->dual[i].xwalls[0]);
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
	int head;

	head = ft_in_set(t_name[0], (const char *)MAP_MCHR);
	printf(">>>> TEST >>> [%c] ::: head <%d>\n", t_name[0], head);   

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
	else if (head > 11 && head < 18)//= ft_in_set(t_name[0], (const char *)MOD_FIRE) == 0))
	{
		printf(">>>> ref[%d]	MODEL : FBALL [%d] >>>\n", head, box->n_fbll);
		if (head > 11 && box->n_fbll == 0)
		{
			printf(">>>>	MODEL : FB[%d] >>>\n", box->n_fbll);
			init_fireball_model(objs);
			box->n_fbll++;
		}
		if (head < 15 && box->n_fpit == 0)
		{
			printf(">>>>	MODEL : FPIT [%d] >>>\n", box->n_fpit);
			init_firepit_model(objs);
			box->n_fpit++;
		}
	}
	else  if (ft_in_set(t_name[0], (const char *)MOD_SPEC) != -1)	
	{
		printf(">>>>	MODEL : PLAYER [%d] >>>\n", box->n_plyr);
			// if (box->n_plyr == 0)
			// 	init_player_model(objs);
			// box->n_plyr++;
	}
			
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
	max = (int)ft_strlen(chrs) - 1;
	grim = max - cub->box.pnum - cub->box.n_dual; // debut dual
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
				
			printf("BUILDING MAPX :: p_box id : %d, grim : %d\n", p_box, grim);
			if (p_box != -1)
			{				
				if ((p_box >= grim  && (p_box < max))) // wall  // 
				{
					printf("BUILDING MAPX :: pset p_box (%d) - grim (%d): %p\n", p_box, grim,
						&cub->pset[p_box - grim]);
					m->mx[m->pos_y][m->pos_x] = &cub->pset[p_box - grim];
				}
				else if ((p_box < grim) && p_box > (grim - cub->box.n_dual - 1)) /// floor + ceil // debut dual
				{
					m->mx[m->pos_y][m->pos_x] = &cub->dual[p_box - cub->box.meta + 1];		
				}
				else if ((p_box < cub->box.meta - 1) && p_box > -1)
				{
					printf("MapX META X {chrs{%c}}>> (%d, %d)>> p_box[%d]: ptr:%p\n", (chrs[p_box]), m->pos_y, m->pos_x, p_box, &cub->pset[p_box]);
					m->mx[m->pos_y][m->pos_x] = m->mx[m->pos_y][m->pos_x - 1];
				}
				if (p_box == max)
					m->mx[m->pos_y][m->pos_x] =  m->mx[m->pos_y][m->pos_x - 1];	
			}
			m->pos_x++;
		}
		m->pos_y++;
	}
	printf(" ... MAPX:exit \n\n");
	return (cub);
}
	// printf("p_box : %d, chrs : %s\n", p_box, chrs);
		// printf("MapX --- chrs{%c} p_box [%d] : N' grim [%d] max [%d]\n", (chrs[p_box]), p_box, grim, max);
		// printf("MapX CHECK --- (W,min)grim + dual = [%d] : (F,max) max - DUAL [%d]\n",  grim + cub->box.n_dual, max - cub->box.n_dual);
		// printf("MapX WALLS >> (%d, %d)>> [box - grim[%d]]: ptr:%p\n", m->pos_y, m->pos_x, (p_box - grim), &cub->pset[p_box]);
		// printf("MapX WALLS >> {{%c}}(%d, %d)>> [p_box: %d] {p_box-grim:%d}: \n",chrs[p_box], m->pos_y, m->pos_x,  p_box, (p_box - grim));
		// printf("MapX FLOOR{chrs{%c}}>> (%d, %d)>> p_box[%d]: ptr:%p\n", (chrs[p_box]), m->pos_y, m->pos_x, p_box, &cub->pset[p_box]);
		// printf("MapX FLOOR{chrs{%c}}>> (%d, %d)>> p_box[%d]:  (box-meta:%d)\n", (chrs[p_box]), m->pos_y, m->pos_x, p_box, (p_box - cub->box.meta + 1));
		// {
		// 		printf("MapX META X {chrs{%c}}>> (%d, %d)>> p_box[%d]: ptr:%p\n", (chrs[p_box]), m->pos_y, m->pos_x, p_box, &cub->pset[p_box]);
	// 	}
