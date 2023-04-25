/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 08:22:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/24 08:22:28 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <dirent.h> 

/* Malloc str:id(txtr_ref)  str:path(txtr_path)*/
t_box *xwalls_builder(t_cub *cub, char **raw, int nb)
{
    t_matrx *pset;
    int     fill;
    int     len;
    int id;

    char ref;
    char rectt;
    
    id = 0;
    (void)ref;
    len = 0;
    if (!(pset = malloc(sizeof(t_matrx *) * cub->box->pset)))
        return (NULL);

    printf("XWLLS PRESET {%s} max len = %d \n", raw[nb], cub->box->pset);
    while(len < cub->box->pset)
    {
        rectt = raw[nb][0];
        printf("START RAW NB[%d]=> REF %c \n", len, raw[nb][0]);
        id = ft_in_set(raw[nb][0], (const char *)MAP_UCHR);
		if (id < 0 || raw[nb][1] != ' ')
			return (NULL);
        //assing it to box->xform
        fill = -1;
        while(fill++ < 3)
        {
            ref = raw[nb][fill + 2];
            id = ft_in_set(raw[nb][fill + 2], MAP_LCHR);
            if (id != -1)
            {
                printf("BUILDER recett{%c} REF(%c) => ID:[%d]\n", rectt, ref, id);
                cub->tex_id++;
                pset->xwalls[fill] =  cub->box->xform[id];
            }
        }
        len++;
        nb++;
    }
    return (cub->box);
}

// XFORM from RAW
t_box *e_mtrx_link(t_box *box, mlx_texture_t *form, char **raw)
{
    char    *tex_path;
    char    *tex_name;
    int i;
    char *idx;
    mlx_texture_t	*xform[box->xnum];

    i = 0;

    printf("- - LINK - \n\n");
    (void)form;
    idx = malloc(sizeof(char) * (box->xnum + 1));
    idx[box->xnum] = '\0';
    while (i < box->xnum)
    {   
        // printf("LINK ref {%d} \n", (unsigned char)raw[i][0]);
        if( raw[i][0] > 32)
        {
            tex_name = ft_substr(raw[i], 0, 1);
            tex_path = ft_substr(raw[i], 2, ft_strlen(raw[i])- 2);
            printf("LINK legend name(%s) ::", tex_name);
            printf(" path {%s} \n", tex_path);
            // box->idx = ft_strjoin(box->idx,tex_name);
            xform[i] = mlx_load_png(tex_path);
            if (!xform[i])
                return (report_mlx_tex_load_failed(tex_path));
        } 
        ++i;
    }
    printf("- - LINK - IDX<%p>\n", xform[i-1]);
    box->xform = xform;
    return (box); 
}

// XNUM COUNT
int e_mtrx_count(char **raw)
{ 
    int i;
    int xnum;

    i = -1;
    xnum = 0;
        // mx->fld_path = get_folder_name(full_path);
        // printf("COUNT_Open fld path %s file name... \n", mx->fld_path); 
    while(*raw && raw[++i])    
    {
        /*     find 'png' ended file*/
        if (ft_strchr_set(raw[i], ".png") != NULL) 
        {
            printf("COUNT legende [%d] into_Raw {%s} ...\n", i, raw[i]); 
            xnum++;
        }
    }
    printf("LINE READ from RAW= %d \n", i);
    // printf("LEGEND enum XNUM = %d \n", xnum);
    return (xnum); 
}

/// Now pre_read folder +  Malloc + post_read linked
t_cub  *e_list_txtr(t_cub *cub, t_map *map)
{ 
    t_box   *box;
	mlx_texture_t	*form;
      

    // form = NULL;
    box = malloc(sizeof(t_box) * 1);
    if (!box)
        return (NULL);
    box->xnum = 0;    //png ended file
    box->xnum = e_mtrx_count(map->raw);
    form = malloc(sizeof(mlx_texture_t *) * (box->xnum));
    if (!form)
    {
        printf("SHIT!!! XNUM = %d \n", box->xnum);
        return (NULL);
    }
    printf("XNUM = %d \n", box->xnum);
    cub->box = e_mtrx_link(box, form, map->raw);
    printf("- - eLIST - IDX<%p>\n", cub->box->xform[0]);
    
    return (cub); 
}
/*  Ref. Tex. By assign.
            get a   txtr_name -> assign it to a char.
                enum : how many txtr to mapp.
                dict : bind unsigned char ptr to txtr->path
                link : load_png_to_txtr ==> ref by unsign_char 
    
    Result  :   LowerCase ('a-z')   = txtr. Mapping (path_name for now!)
            :   UpperCase ('A-Z')   = wall txtr. matrix (tmx)  A = [a,b,c,d] :[West,North,East,Sud]
            :   Numeric   ('0-9')   = floor casting type { 0 = basic }
            :   Meta ('<,>,/,?,\,|,=,+,!,@,#,$,%,&') = reserve for object or spec.

*/
