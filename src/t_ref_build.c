/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 21:34:03 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/11 20:46:54 by gehebert         ###   ########.fr       */
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
	return (gset);}


t_cub	*dual_builder(t_cub *cub, int i, char *t_name)
{
	char	**tex_set;

	cub->box.dual = (t_matrx *)malloc(sizeof(mlx_texture_t *) * cub->box.n_dual);
	if (!cub->box.dual)
		return (NULL);
    if (cub->box.n_dual > i)
    {	
		tex_set = ft_split_space(t_name);
        if (cub->box.open_sky == 0)/// not open_sky
        {
			printf("DUAL txtr >>ID [%d]\n", i);
			printf("::F:[%s] \tC:[%s]::\n", tex_set[0], tex_set[1]);

			cub->box.dual[i].xwalls[0] = mlx_load_png(tex_set[0]);
			if (!cub->box.dual[i].xwalls[0])
				return (report_mlx_tex_load_failed(tex_set[0]));

			cub->box.dual[i].xwalls[1] = mlx_load_png(tex_set[1]);
			if (!cub->box.dual[i].xwalls[0])
				return (report_mlx_tex_load_failed(tex_set[1]));
        }   
		// if (cub->box.open_sky != 0)/// is open_sky
        // {
		// 	printf("DUAL 'z' :Floor >>[%s]\n", tex_set[0]);
		// 	cub->box.dual[i].xwalls[0] = mlx_load_png(tex_set[0]);
		// 	if (!cub->box.dual[i].xwalls[0])
		// 		return (report_mlx_tex_load_failed(tex_set[0]));			
		// 	cub->box.sky = 	cub->box.dual[i].xwalls[0];
		// 	cub->tex.skymap = cub->box.sky;
		// }			

		printf("A DUAL (xwall[0]) >> ptr : %p i: %d\n", cub->box.dual[i].xwalls[0] ,i);
		printf("A DUAL (xwall[1]) >> ptr : %p i: %d\n", cub->box.dual[i].xwalls[1] ,i);

    }
	// free(tex_set[0]);
	// free(tex_set[1]);
	// free(tex_set);
    return (cub);
}


t_cub	*meta_builder(t_cub *cub, t_box *box, char *t_name, t_objs *objs)
{
		// t_objx *objx;

			// objx = malloc(sizeof(t_objx *) * 1);
			// if(!objx)
			// 	printf(">>>> { } NO OBJX { } >>> [%s]\n", t_name);	
			// cub->box.objx = objx;
			// if(!cub->box.objx)
			// 	printf(">>>> { }no box. OBJX { } >>> [%s]\n", t_name);	
			// printf(">>>> TEST >>> [%s]\n", t_name);
			// rawz = cub->map.raw;
			// cub->objs = objs;
			// if(box.meta > i)
			// {	
	
	if (t_name[0] == '*')              		   	///// fireball
	{
		printf(">>>>	MODEL : FB [%d] >>>\n", box->n_fbll);
		if (box->n_fbll == 0)
			objs->fball =  *init_fireball_model(objs);
		box->n_fbll++;
	}
	else  if ((t_name[0] == '!' || t_name[0] == '+') && box->n_lvls == 0)       		  		//// lever
	{
		printf(">>>>	MODEL : LVLR [%d] >>>\n", box->n_lvls);
		if (box->n_lvls == 0)
			objs->lever =  *init_lever_model(objs);
		box->n_lvls++;
	}
	else if (t_name[0] != '?' && t_name[0] != '@') //// portal
	{
		printf(">>>>	MODEL : PORTAL [%d] >>>\n", box->n_prts);
		if (box->n_prts == 0)
			objs->lever =  *init_portal_model(objs);
		box->n_prts++;
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
	
	chrs = cub->box.chrs;
	printf("MAPX:start ..., chrs : %s\n", chrs);
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
//			printf("m pos x, y : (%d, %d)\n", m->pos_x, m->pos_y);
//			printf("m pos char : (%c)\n", m->m[m->pos_y][m->pos_x]);
			p_box = ft_in_set((m->m[m->pos_y][m->pos_x]), chrs);
			if (p_box != -1 || p_box == (int)ft_strlen(chrs) - 1)
			{
				// printf("pset ptr (%d, %d): %p\n", m->pos_y, m->pos_x, &cub->pset[p_box]);
				m->mx[m->pos_y][m->pos_x] = &cub->pset[p_box];
				// printf("new pset %p, xwalls[0] : %p\n", m->mx[m->pos_y][m->pos_x], m->mx[m->pos_y][m->pos_x]->xwalls[0]);
				
			}
			m->pos_x++;
		}
		m->pos_y++;
	}
	printf(" ... MAPX:exit\n\n");
	return (cub);
}



///	frame for model to instance later
	// t_objx 	*model_builder(t_objx *objx, char *mdl_name, int nb_txtr, char *rawz)
	// {
	// 	// t_omdl 	*mdl;
	// 	char	*chr_name;

	// 	chr_name = ft_substr(rawz, 0, 1);	
	// 	// mdl = malloc(sizeof(t_omdl *) * 1);
	// 	// cub->box.objx->mdl = mdl;
	// 	printf(">> model_  mdl_name [%s] >>> chr{%c}\n", mdl_name, rawz[0]);
	// 	// cub->box.objx->model_name = rawz[0];
	// 	// cub->box.objx->
	// 	objx->model_name = chr_name;
	// 	if (nb_txtr == 4)
	// 		objx->type_enum = OBJ_PORTAL;
	// 	else if (nb_txtr == 2)
	// 		objx->type_enum = OBJ_ACTIVATE;
	// 	objx->nb_texs = nb_txtr;
		
	// 	// cub->box.objx->omdl->width = CELL_WIDTH; ; 
	// 	// cub->box.objx->omdl->half_w = cub->objs->portal.width >> 1;;
	// 	// cub->box.objx->omdl->height  = CELL_WIDTH;
	// 	// cub->box.objx->omdl->half_h = cub->objs->portal.height >> 1 ;
	// 	// cub->box.objx->mdl = mdl;
	// 	return (objx);
// }
