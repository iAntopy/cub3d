/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:33:38 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/03 01:04:44 by iamongeo         ###   ########.fr       */
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
# include "mtxlib.h"

# define SCN_WIDTH 20// 800
# define SCN_HEIGHT 640
# define ROT_FACTOR 0.012271846 // (2*PI / 512), soit 1 512e de tour pour chaque deltaX de souris (Temporaire, à teester)

# define CELL_WIDTH 64
# define M_TAU 6.283185307179586f// 2 * pi

# define FOV 2.0943951023931953f// 120 degrees : 2.0943951023931953f, 90 degrees : 1.5707963267948966f
# define FOV_HF 1.0471975511965976f// 120 degrees : 2.0943951023931953f, 90 degrees : 1.5707963267948966f

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
	int		cell_x;
	int		cell_y;
	float	px;
	float	py;
	float	ori;
	t_mtx	*thetas;	// from mlx_linspace() based on player orientation. malloced first, changed in place thereafter.
	t_mtx	*rays[2];	// first ptr is the cosine array from linspace thetas, second is sin array from thetas.
	t_mtx	*collisions;// intersections with walls in x y coords;
	t_mtx	*distances;	// 1D vect, len nb of rays, with distances to collisions
//	t_mtx	*rays;		// rays cast based on angle_thetas.
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

/// RAYCASTER /////////////////
int	init_raycaster(t_cub *cub);

/// DDA ALGO //////////////////

/// ERROR HANDLING ////////////
int	report_mlx_init_error(void);


#endif
