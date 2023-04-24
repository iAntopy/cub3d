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
//

#include "../include/cub3d.h"
#include <dirent.h> 

/// Malloc str:id(txtr_ref)  str:path(txtr_path)
char *xwalls_builder(t_matrx *mx)
{
    int     len;
    char    *id;
    char    *path;
       // char   *xwalls[4];
        // t_box   *xbox;
        // xbox = NULL;
    id = malloc(sizeof(char) + 1);
    id[0] = (char)mx->ref;
    id[1] = '\0';
    id = ft_strjoin(id, " ");

    len = ft_strlen(mx->id_path);
    path = malloc(sizeof(char *) * (len + 1));
    path = ft_strdup(mx->id_path);
        // printf("simple try_out = %s ::\n", id);
        // printf("alter _try_out = %s ::\n", path);  
        // printf("ultra _try_out [%s] ==> {%s}.\n", id, path); 
    path = ft_strjoin(id, path);
    // printf("SUPER (%d)try_out path[0] = {%c} ==> full {%s}\n", i + 1, path[0], path); 
        // xbox->xwalls[i][0] = id;
        // xbox->xwalls[i][1] = path;
        // xbox->xwalls[i] = xwalls[i];
    return (path);
}

/// NEW Malloc'd struct to be fill : mx->full
t_matrx *e_mtrx_link(t_matrx *mx)//, char *full_path)
{ 
    /*        de is Pointer for directory entry */
    struct dirent *de;  
    char *name_path;
    int i;

    i = 0;
    printf("LINK_Open %s directory... \n", mx->fld_path); 
    DIR *dr = opendir(mx->fld_path); 
    while ((de = readdir(dr)) != NULL)
    {   
        if ((ft_strchr_set(de->d_name, "png") != NULL) && (i <= mx->xnum))
        {
            name_path = t_name_set(mx->fld_path, de->d_name);
            mx->id_path = name_path;
            mx->ref = (i + 97);
            /* INSERT XWALLS BUILDER HERE*/
            mx->full[i] = xwalls_builder(mx);
            printf("%d)- MX>> REF:[id:%c]  PATH:{%s} \n", i + 1, mx->full[i][0], mx->full[i]);
            ++i;
        } 
    }
    closedir(dr);     
    return (mx); 
}

/// NEW Only count tx_num : read first time
int e_mtrx_count(t_matrx *mx, char *full_path)
{ 
    /*   de is Pointer for directory entry */
    struct dirent *de;  
    // const char* dir_path;

    // int tx_num = 0;    
     /*    set folder path name from av[1],*/ 
    mx->fld_path = get_folder_name(full_path);
    // printf("COUNT_Open %s directory... \n", mx->fld_path); 
        /*        opendir() returns a pointer of DIR type.*/  
    DIR *dr = opendir(mx->fld_path); 
    if (dr == NULL) 
    { 
        printf("Could not open current directory\n"); 
        return 0; 
    } 
    while ((de = readdir(dr)) != NULL)
    {   
        /*     find 'png' ended file*/
        if ((ft_strchr_set(de->d_name, "png") != NULL) && (mx->xnum < 27 ))
            mx->xnum++;
        else if (mx->xnum >= 27)  
        {
            printf("Too Many Textures , 52 is max for NOW !\n");
            break ;
        }       
    }
    closedir(dr);     
    return (mx->xnum); 
}

/// Now pre_read folder +  Malloc + post_read linked
int  e_list_txtr(t_cub *cub, char *full_path)
{ 
    t_matrx *mx;

    mx = malloc(sizeof(t_matrx) * 1);
    if (!mx)
        return (-1);
    mx->xnum = e_mtrx_count(mx, full_path);
    mx->full = malloc(sizeof(char *) * (mx->xnum + 1));
    mx = e_mtrx_link(mx);
    cub->mx = mx;

    return (0); 
}
