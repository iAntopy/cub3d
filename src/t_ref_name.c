
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


// static struct t_matrx *e_link_tx(unsigned char ref, char *tx, int tx_num)
// void e_link_tx(t_matrx *mx)
// {
//     // while(mx != NULL )
//     // {
//         printf("MX->ref[%c] MX->id_path {%s} \n", mx->ref, mx->id_path);
//         // ++mx;
//     // }
// }

/// NEW Malloc struct to be fill : read second time
t_matrx *e_mtrx_link(t_matrx *mx, char *full_path, int tx_num)
{ 
    /*        de is Pointer for directory entry */
    struct dirent *de;  
    const char* dir_path;
    char *name_path;
    int i;

    i = 0;
    dir_path = get_folder_name(full_path);
    printf("LINK_Open %s directory... \n", dir_path); 

    DIR *dr = opendir(dir_path); 
    while ((de = readdir(dr)) != NULL)
    {   
        if ((ft_strchr_set(de->d_name, "png") != NULL) && (i <= tx_num))
        {
            name_path = t_name_set(dir_path, de->d_name);
            mx->id_path = name_path;
            mx->ref = (i + 97);
            printf("%d)- ref[%c] = {%s} \n", i + 1, mx->ref, mx->id_path); 
            ++i;
            ++mx;
        } 
    }
    closedir(dr);     
    return (mx); 
}


/// NEW Only count tx_num : read first time
static int     e_mtrx_count(char *full_path)
{ 
    /*   de is Pointer for directory entry */
    struct dirent *de;  
    const char* dir_path;

    int tx_num = 0;    
    /*    set folder path name from av[1],*/ 
    dir_path = get_folder_name(full_path);
    printf("COUNT_Open %s directory... \n", dir_path); 

    /*        opendir() returns a pointer of DIR type.*/  
    DIR *dr = opendir(dir_path); 
    if (dr == NULL) 
    { 
        printf("Could not open current directory\n"); 
        return 0; 
    } 
    while ((de = readdir(dr)) != NULL)
    {   
        /*     find 'png' ended file*/
        if ((ft_strchr_set(de->d_name, "png") != NULL) && (tx_num < 27 ))
            ++tx_num;
        else if (tx_num >= 27)  
        {
            printf("Too Many Textures , 52 is max for NOW !\n");
            break ;
        }       
    }
    closedir(dr);     
    return (tx_num); 
}
/// Now pre_read folder +  Malloc + post_read linked
int     e_list_txtr(char *full_path)
{ 
    t_matrx *mx;
    int tx_num;
   
    tx_num = e_mtrx_count(full_path);
    mx = malloc(sizeof(t_matrx) * tx_num + 1);
    if (!mx)
        return (0);
    mx = e_mtrx_link(mx, full_path, tx_num);
    // e_link_tx(mx);
    // printf("MX->refREF[%c] MX->id_path {%s} \n", mx->ref, mx->id_path);
        

    /* */
    // }
    // closedir(dr);     
    return (tx_num); 
}
