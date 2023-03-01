/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:33:38 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/28 22:44:48 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CUB3D_H__
# define __CUB3D_H__

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>

# include "MLX42/MLX42.h"
# include "libft.h"

# define SCN_WIDTH 800
# define SCN_HEIGHT 640
# define ROT_FACTOR 0.012271846 // (2*PI / 512), soit 1 512e de tour pour chaque deltaX de souris (Temporaire, à teester)

# define CELL_WIDTH 64
# define 

enum	e_sides
{
	SIDE_W = 0,
	SIDE_N = 1,
	SIDE_E = 2,
	SIDE_S = 3
};

typedef struct s_map_data
{
	int	width;
	int	height;
	int	total_cells;
	int	width_px;
	int	height_px;
	char	**map;		// 2D array (could be 1D, char * instead)
	char	*collisions;	// 1D array 
}	t_map;

// All 4 elem arrays of textures organized as W, N, E, S, according to the side they represent.
// Potentially, animated wall textures could be in a 2D array[side][anim_iteration] up to n nb of frames in the animation.
typedef struct s_texture_data
{
	mlx_texture_t	*walls[4];	// pointers returned from mlx_load_png(path)
	mlx_texture_t	*skymap;	// yessss
}	t_tex;

typedef struct s_main_character_data
{
	int	cell_x;
	int	cell_y;
	float	px;
	float	py;
	float	ori;
	t_mtx	*angle_thetas;	// from mlx_linspace() based on player orientation. malloced first, changed in place thereafter.
	t_mtx	*rays;		// rays cast based on angle_thetas.
}	t_hero;

typedef struct s_cub3d_core_data
{
	mlx_t	*mlx;

	int	scn_midx;	// mid screen x coordinate
	int	scn_midy;	// mid screen y coordinate

	t_map	map;
	t_tex	tex;
	t_hero	hero;
}	t_cub;


/// PARSING ///////////////////

/// DDA ALGO //////////////////

/// ERROR HANDLING ////////////
int	report_mlx_init_error(void);


#endif
