
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

// int main(void) 
static e_link_tx(unsigned char ref, char *tx_name, de->d_name)
{

}

int     e_list_txtr(void)
{ 
   /* de is Pointer for directory entry */
    struct dirent *de;  
    const char* dir_path = "./tex/ext";
    int tx_num = 0;
    /*opendir() returns a pointer of DIR type.*/  
    DIR *dr = opendir(dir_path); 

    if (dr == NULL)  /* opendir returns NULL if couldn't open directory */
    { 
        printf("Could not open current directory\n"); 
        return 0; 
    } 

    while ((de = readdir(dr)) != NULL){
        tx_num++;
        printf("%d)- %s\n", tx_num, de->d_name); 
        if (!ft_strcmp(de->d_name, ".cmp"))
        {
            if(tx_num < 27)
                e_link_tx(tx_num + 65, de->d_name);
            else if(tx_num < 53)
                e_link_tx(tx_num + 97, de->d_name);
            else if (tx_num > 52)  
                printf("Too Many Textures , 52 is max for NOW !\n");
        }

    }
    closedir(dr);     
    return (tx_num); 
}
