/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 08:22:57 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/24 08:23:01 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**  get_folder_name by map_name
        av[1] =  from end (.cub) to backslash (maps/)
        folder_name duty to txtr.mapp.
*/

#include "../include/cub3d.h"


// transfere ref "A a,b,c,d" into xwalls[4]
char *get_ref_str(t_cub *cub, char *ref, int alt)
{
    //find txtr ref 
    char *tex_str;
    int x = 0;

    while(*cub->mx->full[x])
    {
        if (*cub->mx->full[x] != ref[alt + 2])
            x++;
        else 
            break;
    }
    // printf("::: SPACEX --id(%d) -- ref[%c] -- name{%s}\n", alt, ref[alt+2], cub->mx->full[x]);    
    tex_str = cub->mx->full[x];
    return (tex_str);       
}

t_cub	*get_tex_by_ref(t_cub *cub, int id, const char *tex_str)
{
    /*
        OPTIMIZED :: struct t_matrx *mx link into *.cub
    */
	char	*t;
    const char    *aux;
    int     alt;
    alt = 0;
	printf("______ HERE GET_BY_REF__[%d]___name  \n", id);
    // printf("_______[%c]___name :", tex_str[0]);
   
	/* go get xwalls now : where */
    /*  
        get     : str* to deploy  == tex_str ***
                :   {"A  a,b,c,d"} == xwalls[4] 
                :
                : set each lower case to a path to replace tex_str
                :   alt value = array pos;

        set     : assign  lower_char ref with full_path to tex_str
                :   array pos char ++;

        ex: alt = 0 , get {"a tex/...png"}
    */
    // tex_str == {"A a,b,c,d"}
    // xwalls = tex_str;
    while (alt <= 3)
    {
        if (!cub->tex.walls[alt])
        {
            aux =  get_ref_str(cub, (char *)tex_str, alt);
            // printf("_FULL__%s\n", aux);
            t = ft_substr(aux, 2, ft_strlen(aux)-2);
            printf("REF[%d] txtr_id(%c) TEX_PATH__%s\n", alt, tex_str[alt+2], t);
            cub->tex.walls[alt] = mlx_load_png(t);
            if (!cub->tex.walls[alt])
                return (report_mlx_tex_load_failed(t));
            cub->tex_id++;
            alt++;
        }
        else
        {
            ft_eprintf("Error\n\t- Trying to load texture id %d twice.\n", id);
            return (NULL);
        }	
    }
	return (cub);
}

char *t_name_set(const char *dir_path, char *d_name)
{
    char *full_name;

    full_name = ft_strjoin(dir_path, d_name);
    return (full_name);
}



const char *get_folder_name(char *full_path)
{
    char *folder_name;
    int strt;
    int end;

    strt = 0;
    end = 0;
    // printf(" FULL name:%s\n", full_path); 
    while(full_path[strt] != '\0')
    {
        if (full_path[strt] == '.')
        {
            end = strt;
            break;
        }
        ++strt;
    }
    while(*full_path)
    {
        if (full_path[strt] != 47)
            --strt;
        else if (full_path[strt] == 47)
            break;
    }
    folder_name = ft_substr(full_path, strt, (end - strt));
    folder_name = ft_strjoin ("tex", folder_name);
    folder_name = ft_strjoin (folder_name, "/");
    printf("GET_FLD_NAME:: %s\n",folder_name); 
    return ((const char *)folder_name);
}
