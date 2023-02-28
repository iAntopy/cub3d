/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:33:38 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/28 00:05:58 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CUB3D_H__
# define __CUB3D_H__

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft.h"

# define SCN_WIDTH 800
# define SCN_HEIGHT 640
# define ROT_FACTOR 0.012271846 // (2*PI / 512), soit 1 512e de tour pour chaque deltaX de souris (Temporaire, à teester)
# define MLX_ERROR 1


typedef struct	s_vector2D
{
	float	x;
	float	y;
}	t_vec2;
	
typedef struct	s_cub3d_core_data
{
	mlx_t	*mlx;

	int	scn_midx;	// mid screen x coordinate
	int	scn_midy;	// mid screen y coordinate

	t_vec2	pos;		// position
	float	ori;		// orientation
}	t_cub;


/// PARSING ///////////////////
// minimap 
typedef struct s_minimap
{
	char *file;		// map filename *.cub
	char **tab;		// char input parsed
	int color;	 	// px color
	int pos_x;		// aqui_X
	int pos_y;		// aqui_y
	int width;		// max_x_len
	int height;		// max_y_len
} 	t_minimap;


/// DDA ALGO //////////////////


/// ERROR HANDLING ////////////
int	report_mlx_init_error(void);

// MAP_CHECKER //////////
// map_parse

static	void	wall_check(t_minimap *m);
static	void	start_set(t_minimap *map, int flag);
static	void	transcribe(t_minimap *map, char *file);
void			map_checker(t_minimap *mini, char *file);
// mini_tool
t_minimap		*init_mini(t_minimap *mini);
int				error(char *error, t_minimap *mini);
int				int_strlen(const char *s);
void			print_map(t_minimap *m);
int				ft_strfcmp(const char	*s1, const char	*s2, size_t n);

#endif
