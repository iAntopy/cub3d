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
// char *xwalls_builder(t_matrx *mx)
// {
    //     int     len;
    //     char    *id;
    //     char    *path;
    
    //     id = malloc(sizeof(char) + 1);
    //     id[0] = (char)mx->ref;
    //     id[1] = '\0';
    //     id = ft_strjoin(id, " ");
    //     len = ft_strlen(mx->id_path);
    //     path = malloc(sizeof(char *) * (len + 1));
    //     path = ft_strdup(mx->id_path);
    //     path = ft_strjoin(id, path);
    //     // printf("SUPER (%d)try_out path[0] = {%c} ==> full {%s}\n", i + 1, path[0], path); 
    //     return (path);
// }

    // while (alt <= 3)
    // {
    //     if (!cub->tex.walls[alt])
    //     {
    //         cub->tex_id++;
    //         t =  get_ref_str(cub, (char *)tex_str, alt);
    //         // t = ft_substr(aux, 2, ft_strlen(aux)-2);
    //         printf("REF[%d] txtr_id(%c) TEX_PATH__%s\n", cub->tex_id, tex_str[alt + 2], t);
    //         cub->tex.walls[alt] = mlx_load_png(t);
    //         if (!cub->tex.walls[alt])
    //             return (report_mlx_tex_load_failed(t));
    //         alt++;
    //     }

// get box and xform then fill it out!
t_box *e_mtrx_link(t_box *box, mlx_texture_t *form, char **raw)
{ 
    /*        de is Pointer for directory entry */
    char    *tex_path;
    char    *tex_name;
    int i;
    mlx_texture_t	*xform[box->xnum];

    i = 0;
    printf("- - LINK - \n\n");
    (void)form;
    while (i < box->xnum)
    {   
        // printf("LINK ref {%d} \n", (unsigned char)raw[i][0]);
        if( raw[i][0] > 32)
        {
            tex_name = ft_substr(raw[i], 0, 1);
            tex_path = ft_substr(raw[i], 2, ft_strlen(raw[i])- 2);
            printf("LINK legend name(%s) ::", tex_name);
            printf(" path {%s} \n", tex_path);

            xform[i] = mlx_load_png(tex_path);
            // printf("POST:LINK texture [%ld] \n", *box->xform);
            if (!xform[i])
                return (report_mlx_tex_load_failed(tex_path));
        } 
        ++i;
    }
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
    t_matrx *mx;
    t_box   *box;
	mlx_texture_t	*form;
      

    // form = NULL;
    mx = malloc(sizeof(t_matrx) * 1);
    if (!mx)
        return (NULL);
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
    // cub->box->form = form;
    cub->box = e_mtrx_link(box, form, map->raw);
    cub->mx = mx;
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
