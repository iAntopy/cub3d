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



// XFORM from RAW
t_box *e_mtrx_link(t_box *box, char **raw)
{
    char    *tex_path;
    char    *tex_name;
    int i;

    i = 0;
    box->pnum = 0;
    // printf("- - LINK - \n\n");
    box->xform = (mlx_texture_t **)malloc(sizeof(mlx_texture_t *) * box->xnum);
    if(!box->xform )
        return (NULL);    
    while (i < box->xnum)
    {   
        if(raw[i][0] > 32)
        {
            tex_name = ft_substr(raw[i], 0, 1);
            tex_path = ft_substr(raw[i], 2, ft_strlen(raw[i])- 2);
            if (ft_in_set(raw[i][0], (const char *)MAP_NCHR) != -1)
                box->pnum++;
            box->xform[i] = mlx_load_png(tex_path);
            if (!box->xform[i])
                return (report_mlx_tex_load_failed(tex_path));
        } 
        ++i;
    }
    return (box); 
}

// XNUM COUNT
t_box   *e_mtrx_count(t_map *map, t_box *box)
{ 
    int i;
    // int len;

    i = -1;
    box->chrs_len = 0;
    while(*map->raw && map->raw[i++])    
    {
        /*  add map_char ref count len */
        if (map->raw[i][0] > 32 && map->raw[i][1] == 32)
            ++box->chrs_len;
        /*     find 'png' ended file*/
        if (ft_strchr_set(map->raw[i], ".png") != NULL) 
            box->xnum++;
    }
    // box->chrs = malloc(sizeof(char) * box->chrs_len + 2);
    // box->chrs[box->chrs_len] = '\0';
    // i = 0;
    // len = 0;
    // while (len < box->chrs_len)
    // {
    //     if (map->raw[i][0] > 32 && map->raw[i][1] == ' ')
    //         box->chrs[len++] = map->raw[i][0];
    // }
    printf(">> LINE MADE to CHRS {%d}\n", box->chrs_len);
    return (box); 
}
        
            // printf("COUNT legende [%d] into_Raw {%s} ...\n", i, raw[i]); 
        

/// Now pre_read folder +  Malloc + post_read linked
t_cub  *e_list_txtr(t_cub *cub, t_box *box, t_map *map)
{ 
      
    box->xnum = 0;    //png ended file
    box = e_mtrx_count(map, box);
    printf("XNUM = %d \n", box->xnum);
    cub->box = *e_mtrx_link(box, map->raw);
    // printf("- - eLIST - IDX<%p>\n", cub->box.xform[0]);
    
    return (cub); 
}

/*  Ref. Tex. By assign.    VER.1
            get a   txtr_name -> assign it to a char.
                enum : how many txtr to mapp.
                dict : bind unsigned char ptr to txtr->path
                link : load_png_to_txtr ==> ref by unsign_char 
    
    Result  :   LowerCase ('a-z')   = txtr. Mapping (path_name for now!)
            :   UpperCase ('A-Z')   = wall txtr. matrix (tmx)  A = [a,b,c,d] :[West,North,East,Sud]
            :   Numeric   ('0-9')   = floor casting type { 0 = basic }
            :   Meta ('<,>,/,?,\,|,=,+,!,@,#,$,%,&') = reserve for object or spec.

*/
