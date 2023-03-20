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

// void	assigne_img(t_cub *cub, t_map *map, t_img *img)
// {
	// 	img->w_side = extract_img(cub, "tex/w_side.xpm");
	// 	img->n_side = extract_img(cub, "tex/n_side.xpm");
	// 	img->e_side = extract_img(cub, "tex/e_side.xpm");
	// 	img->s_side = extract_img(cub, "tex/s_side.xpm");
		
	// 	// img->floor = extract_img(cub, "tex/floor.xpm");
	// 	// img->ciel = extract_img(cub, "tex/coin.xpm");
	// 	// img->exit = extract_img(mlx, "img/exit.xpm");
	// 	// img->wall = extract_img(mlx, "img/rock.xpm");
	// 	// img->tile = extract_img(mlx, "img/tile.xpm");
// }