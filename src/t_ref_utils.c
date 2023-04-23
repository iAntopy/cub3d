/*
**  get_folder_name by map_name
        av[1] =  from end (.cub) to backslash (maps/)
        folder_name duty to txtr.mapp.
*/

#include "../include/cub3d.h"


// transfere ref "A a,b,c,d" into xwalls[4]
// static char *get_ref_str(t_cub *cub,int alt, char *xwalls)
// {
//     //find txtr ref 
//     char *tex_str;

//     tex_str = 

// }

t_cub	*get_tex_by_ref(t_cub *cub, int id, char *tex_str)
{
    /*
        OPTIMIZED :: struct t_matrx *mx link into *.cub
            this the new fonct. to fit xwalls.
                needs   :   - matrx char ref. str[0]
                        :   - matrx array[4]. str[1]++
    */
	char	*t;
    // char    *xwalls;
    int     alt;
    alt = 0;
	printf("______ HERE GET_BY_REF__[%d]___name  \n", id);
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
    while (alt < 3)
    {
        // get alt pos++  into array ref

            //  tex_str = get_ref_str(alt, xwalls);
            // tex_str = get_ref_str(cub, alt, xwalls);
        if (!cub->tex.walls[alt])
        {
            while (*(++tex_str) && ft_isspace(*tex_str))
                continue ;
            t = tex_str;
            while (*tex_str && !ft_isspace(*tex_str))
                tex_str++ ;
            *tex_str = '\0';
            cub->tex.walls[alt] = mlx_load_png(t);
            if (!cub->tex.walls[alt])
                return (report_mlx_tex_load_failed(t));
            cub->tex_id++;
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