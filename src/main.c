/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:07:26 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/28 13:17:17 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	cub_clear(t_cub *cub, int exit_status)
{
	printf("CUB CLEAR AT EXIT\n");
	if (cub->mlx)
		mlx_terminate(cub->mlx);
	return (exit_status);
}

void	on_close(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	printf("CLOSE REQUEST RECEIVED !\n");
	mlx_close_window(cub->mlx);
}

void	cub_key_handler(mlx_key_data_t event, void *param)
{
	//	t_cub	*cub;

	//	cub = (t_cub *)param;
	if (event.key == MLX_KEY_ESCAPE && event.action == MLX_PRESS)
		on_close(param);
}

void	on_cursor_move(double xpos, double ypos, void *param)
{
	t_cub	*cub;
	double	dx;
	double	dy;

	cub = (t_cub *)param;
	(void)cub;
	dx = xpos - cub->scn_midx;
	dy = ypos - cub->scn_midy;
	cub->ori += dx * ROT_FACTOR;
	printf("Cursor moved : pos (x, y) : (%lf, %lf), delta (dx, dy) : (%lf, %lf), ori : %f\n", xpos, ypos, dx, dy,
		cub->ori);

	// CAN'T TOUCH THIS, naaaaah na na na tst tst na na tst tst na na.
	mlx_set_mouse_pos(cub->mlx, cub->scn_midx, cub->scn_midy);
}

// On pourra intégrer ces lignes là dans le parsing ou qqc du genre, mais on va en avoir besoin. 
int	cub_init_core_data(t_cub *cub)
{
	cub->scn_midx = SCN_WIDTH / 2;
	cub->scn_midy = SCN_HEIGHT / 2;
	return (0);
}

int	main(int argc, char **argv)
{
	t_cub		cub;
	t_minimap	*mini;
//	mlx_image_t	*img;
		
	(void)argc;
	(void)argv;
	ft_memclear(&cub, sizeof(cub));

	// FONCTION DE PARSING VIENT ICI !!
	// INIT INPUT	
	mini = NULL;
	map_checker(init_mini(mini),cub, argv[1]);
	// if (parsing_func_de_fou_debile(&cub, argc, argv) < 0)
	//	return (EXIT_FAILURE);
	

	printf("Try init mlx \n");
	
	// Window Width, Height, title, is_resizable. (option possible pour la window : full screen mode)
	cub.mlx = mlx_init(SCN_WIDTH, SCN_HEIGHT, "(cub)^3.D", 0);
	if (!cub.mlx)
		return (EXIT_FAILURE);
	printf("Init mlx SUCCESSFUL\n");


	// INIT DATA
	cub_init_core_data(&cub);



	
	// INIT CURSOR SETTINGS
	mlx_set_mouse_pos(cub.mlx, cub.scn_midx, cub.scn_midy);
	mlx_set_cursor_mode(cub.mlx, MLX_MOUSE_HIDDEN);
	mlx_focus(cub.mlx);

	
	// INIT HOOKS
	mlx_cursor_hook(cub.mlx, &on_cursor_move, &cub);
	mlx_key_hook(cub.mlx, &cub_key_handler, &cub);
	mlx_close_hook(cub.mlx, &on_close, &cub);
	

	printf("Starting mlx loop\n");
	mlx_loop(cub.mlx);
	printf("mlx loop stopped !\n");
	return (cub_clear(&cub, EXIT_SUCCESS));
}
