
#include "../include/cub3d.h"







void	build_img(t_cub *cub, t_map *map, t_paint *img)
{
		img->txtr[0] =  mlx_load_png("tex/mask/wall_x_0.png");
        img->txtr[1] =  mlx_load_png("tex/mask/wall_x_1.png");
        img->txtr[2] =  mlx_load_png("tex/mask/wall_x_2.png");
        img->txtr[3] =  mlx_load_png("tex/mask/wall_x_3.png");



		// img->mask[0]  =  mlx_load_png(cub, "tex/n_side.xpm");
        // img->mask[1]  =  mlx_load_png(cub, "tex/n_side.xpm");
        // img->mask[2]  =  mlx_load_png(cub, "tex/n_side.xpm");

		
}
