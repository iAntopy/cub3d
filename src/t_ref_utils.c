/*
**  get_folder_name by map_name
        av[1] =  from end (.cub) to backslash (maps/)
        folder_name duty to txtr.mapp.
*/

#include "../include/cub3d.h"

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
    // printf(" strt:%d  end:%d  name:%s\n", strt, end, full_path); 
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
    folder_name = ft_strjoin ("./tex", folder_name);
    folder_name = ft_strjoin (folder_name, "/");
    // printf(" strt:%d  end:%d  name:%s\n", strt, end, folder_name); 
    return ((const char *)folder_name);
}