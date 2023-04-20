
/*
    Ref. Tex. By assign.
        get a   txtr_name -> assign it to a char.
                enum : how many txtr to mapp.
                dict : bind unsigned char ptr to txtr->path
                link : load_png_to_txtr ==> ref by unsign_char 
                result :  

*/
//


#include <stdio.h> 
#include <dirent.h> 

#include "../include/cub3d.h"

t_ref *init_dict(t_ref *dict)
{
    dict->tx_id = 0;
    dict->tx_num = 0;
	dict->tx_name = NULL;
    // dict->tx_ref = 0;
    // dict->link[dict->id];
    return (dict);
}

static t_ref *e_dict_txtr(unsigned char ref, char *name, t_ref *dict)
{
    dict->tx_name = name;
    dict->tx_ref = ref;
    printf("%d)-", dict->tx_id); 
    printf(" %c = %s \n", dict->tx_ref, dict->tx_name);//, dict->tx_ptr);
    dict->tx_id++;
    return (dict);
}

// static t_ref *e_union_path(const char *path, char *name, t_ref *dict)
// {
//         // char *tx_name;
//         // tx_name = NULL;
//         dict->tx_name = ft_strjoin((char *)path, name);
//         // dict->tx_ptr = 
//         // printf("%d)-", dict->tx_id); 
//         // printf(" %s \n", dict->tx_name);//, dict->tx_ptr);
//         return (dict);
// }


int  e_link_txtr(void)
{ 
   /* de is Pointer for directory entry */
    struct dirent *de;  
    const char* dir_path = "./tex/ext/";
    t_ref *dict;
    dict = NULL;

    /* init struct data quiering */
    dict = init_dict(dict);

    /*opendir() returns a pointer of DIR type.*/  
    DIR *dr = opendir(dir_path); 
    if (dr == NULL)  /* opendir returns NULL if couldn't open directory */
    { 
        printf("Could not open current directory\n"); 
        return 0; 
    } 
    /* as long as ...*/
    while ((de = readdir(dr)) != NULL)
    {
        if (strcmp(de->d_name, ".png"))
        {  
            dict->tx_num++;
            // dict = e_union_path(dir_path, de->d_name, dict);
            dict->tx_name = ft_strjoin(dir_path, de->d_name);
            printf("IM IN\n"); 

            if (dict->tx_num < 27 && (strcmp(de->d_name, ".png")))
                dict = e_dict_txtr(dict->tx_num + 96, dict->tx_name, dict);
            else if (dict->tx_num > 26 && (strcmp(de->d_name, ".png")))
                dict = e_dict_txtr(dict->tx_num + 64 - 26, dict->tx_name, dict);
            else if (dict->tx_num >= 52)  
            {
                printf("Too Many Textures , 52 is max for NOW !\n You are at %ld", dict->tx_num);
                break ;
            }
        }

    }
    closedir(dr);     
    return (dict->tx_num); 
}
