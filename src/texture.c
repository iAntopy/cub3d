// #include "../include/textur.h"
#include "../include/cub3d.h"


// void	print_floor(t_map *m)
	// {
	// 	int	j;
	// 	int	i;

	// 	i = -1;
	// 	while (++i < m->h / 36)
	// 	{
	// 		j = -1;
	// 		while (++j < m->w / 36)
	// 		{				
	// 			mlx_put_image_to_window(m->mlx.mlx, m->mlx.win, m->img.floor, j * 36, i * 36);
	// 		}
	// 	}
// }

// void	print_map_next(t_map *m, int i, int j)
// {
	// 	if (m->map[i][j] == 'E')
	// 		mlx_put_image_to_window(
	// 		m->mlx.mlx, m->mlx.win, m->img.exit, j * 36, i * 36);
	// 	else if (m->map[i][j] == 'C')
	// 		mlx_put_image_to_window(
	// 		m->mlx.mlx, m->mlx.win, m->img.coin, j * 36, i * 36);
// }

// void	print_img(t_map *map, t_cub cub)
// {
	// 		int	j;
	// 		int	i;

	// 		i = -1;
	// 		// print_floor(m);
	// 		while (++i < 4)
	// 		{
	// 			if (i <= 4)
	// 			{	

	// 				// Try to load texture from  file
	// 				cub.texture = mlx_load_png(map->file);
	// 				if (!cub.texture)
	// 					error("B. You are trying but no png to tex.\n", map);

	// 					// set img to be display
	// 				cub.img = mlx_texture_to_image(cub.mlx, cub.texture);
	// 				if (!cub.img)
	// 					error("C. You are trying to open img but no img.\n", map);
					
	// 				// assigne img to window
	// 				if (mlx_image_to_window(cub.mlx, cub.img, 0, 0) < 0)
	// 					error("D. You are trying to too hard to open img into window.\n", map);		
					
	// 				// mlx_put_image_to_window(m->mlx.mlx, m->mlx.win,	m->img.player, m->y, m->x);
	// 					// j = -1;
	// 					// while (++j < m->w / 36)
	// 					// {
	// 					// 	if (m->map[i][j] == '1' && (i == 0 || j == 0
	// 					// 	|| i == (m->h / 36) - 1 || j == (m->w / 36) - 1))
	// 					// 		mlx_put_image_to_window(
	// 					// 		m->mlx.mlx, m->mlx.win, m->img.wall, j * 36, i * 36);
	// 					// 	else if (m->map[i][j] == '1')
	// 					// 		mlx_put_image_to_window(
	// 					// 		m->mlx.mlx, m->mlx.win, m->img.tile, j * 36, i * 36);
	// 					// 	else
	// 					// 		print_map_next(m, i, j);
	// 					// }
	// 				// }
// }

// void	*extract_img(t_cub *cub, char *filepath)
	// {
	// 	int		w;
	// 	int		h;
	// 	void	*img;

	// 	w = 0;
	// 	h = 0;
	// 	img = mlx_xpm_file_to_image(cub, filepath, &w, &h);
	// 	if (w == 0 || h == 0)
	// 	{
	// 		perror("Error\nfile_to_image");
	// 		exit (EXIT_FAILURE);
	// 	}
	// 	return (img);
// }

void	build_img(t_tex *tex)//t_paint *img)
{
	
		tex->txtr[0] =  mlx_load_png("tex/mask/wall_x_0.png");
        tex->txtr[1] =  mlx_load_png("tex/mask/wall_x_1.png");
        tex->txtr[2] =  mlx_load_png("tex/mask/wall_x_2.png");
        tex->txtr[3] =  mlx_load_png("tex/mask/wall_x_3.png");
        tex->txtr[img[0]] =  mlx_load_png("tex/mask/wall_x_a.png");
        tex->txtr[img[1]] =  mlx_load_png("tex/mask/wall_x_b.png");
        tex->txtr[img[2]] =  mlx_load_png("tex/mask/wall_x_c.png");


		// img->mask[0]  =  mlx_load_png(cub, "tex/n_side.xpm");
        // img->mask[1]  =  mlx_load_png(cub, "tex/n_side.xpm");
        // img->mask[2]  =  mlx_load_png(cub, "tex/n_side.xpm");

		
}