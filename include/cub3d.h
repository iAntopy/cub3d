/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:18:35 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/17 19:44:41 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <math.h>

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft.h"
# include "../lib/mtxlib/includes/mtxlib.h"

# define SCN_WIDTH  1024
# define SCN_HEIGHT 780

# define ROT_FACTOR 0.006135923f
# define CELL_WIDTH 64
# define M_TAU 6.283185307179586f

# define FOV120 2.0943951023931953f
# define FOV120_HF 1.0471975511965976f

# define FOV100 1.7453292519943295f
# define FOV100_HF 0.8726646259971647f

# define FOV90 1.5707963267948966f
# define FOV90_HF 0.7853981633974483f

# define FOV60 1.047197551196597746f
# define FOV60_HF 0.52359877559829887f

# define FOV20 0.349065850398865915f
# define FOV20_HF 0.17453292519943295f

# define FOV FOV60
# define FOV_HF FOV60_HF
# define FOV_MIN FOV20
# define FOV_MAX FOV60

# define TEXTURES			6
# define TEX_WEST			0
# define TEX_NORTH			1
# define TEX_EAST			2
# define TEX_SOUTH			3
# define TEX_SKY			4
# define TEX_FLOOR			5

# define CUBMAP_BUFMAX 100000
# define MAP_CHARS "01WNES"

enum	e_sides
{
	W_SIDE = 0,
	N_SIDE = 1,
	E_SIDE = 2,
	S_SIDE = 3
};
/*
/// PARSING ///////////////////
// coordonees standard
typedef struct  s_pos
{
	int		x;
	int		y;
}   t_pos;
*/

// collision_map : 1D array map where 1 is solid wall otherwise 0.
// grid_coords : top-left corner coordinate for grid indexed [cell_y][cell_x]
// file : map filename *.cub
// tab : char input parsed
// pos_x : aqui_X
// pos_y : aqui_y
// cases : total case all gabarit
// array : char* array 
typedef struct s_map_data
{
	int		width;
	int		height;
	int		total_cells;
	int		width_px;
	int		height_px;
	int		total_area;
	char	*collision_map;
	float	**grid_coords;

	// Germain specific Stuff
	char	*file;
	char	**tab;
	char	**txtr;
	char	**raw;
	int		pos_x;
	int		pos_y;
	int		lines_to_map;
	int		hero_side;
	int		hero_x;
	int		hero_y;
	int		flg_chk;
}	t_map;

// All 4 elem arrays of textures in order W, N, E, S.
// walls : array of mlx_texture_t ptr foreach side.
typedef struct s_texture_data
{
	mlx_texture_t	*walls[4];
	char			**rgbx;
	int				color[2];
//	char			*tex_n[4];// tex_name
}	t_tex;

typedef struct s_raycaster_data		t_rcast;
typedef struct s_cub3d_core_data	t_cub;

typedef struct s_ray_collision_data
{
//	External ref
	t_rcast	*rcast;

//	Struct constants;
	int		idx;
	int		*pcx;//	hero cell position in x
	int		*pcy;// hero cell position in y
	float	*px;// hero position x; Pointer to cub->hero.px.
	float	*py;// hero position y;
	float	*p_dirx;// player's directional vector in x;
	float	*p_diry;// player's directional vector in y;
	float	*rx;// ray vector direction in x for specific ray; Pointer to rcast->rays[0] at rays idx.
	float	*ry;// ray vector direction in y for specific ray; Pointer to rcast->rays[1] at rays idx.

//	Init data
	int		dx;//	x offset of cell to check in collision map
	int		dy;//	y offset of cell to check in collision map
	int		cincr_x;//	direction of cell move for vertical axis collision. Either 1 or -1.
	int		cincr_y;//	direction of cell move for horizontal axis collision. Either 1 or -1.
	float	a;//	ray slope
	float	inv_a;//a inverse == 1/a;
	float	b;//	ray y offset

//	Tracked data (final value is resulting data)
	int		cx;// tracked collision cell x; Init to hero.cell_x. Final value is collision cell x.
	int		cy;// tracked collision cell y; Init to hero.cell_y. Final value is collision cell y.
	
//	Resulting data
	int		side;// collision side. Can be compared to side enums.
	float	hitx;// collision world coord x;
	float	hity;// collision world coord y;
	float	dist;// collision distance to projection plane.
	float	tex_ratio;// ratio of hit on wall from left to right. Used to find drawn texture column.
	float	tex_height;// texture height on projection screen. Can be greater then SCN_HEIGHT.

}	t_rdata;

typedef struct s_raycaster_data
{
	t_cub		*cub;
	t_map		*map;
	t_mtx		*theta_offs;// Angle offsets for each angle from 0. Malloced mtx.
	t_mtx		*ray_thetas;// Angles for each ray. Malloced mtx.
	t_mtx		*rays[2];// X, Y part for each ray vector. index 0 are Xs, 1 are Ys Malloced mtx.
	t_rdata		*rdata;//	malloced array of struct with collision data. len SCR_WIDTH
}	t_rcast;

typedef struct s_main_character_data
{
	int		cell_x;
	int		cell_y;
	float	px;
	float	py;
	float	ori;//	player orientation in radian
	float	ori_factor;//	ori / 2pi, updated in cub_player_rotate
	float	*dirx;	// ptr to rays[0][SCN_WIDTH / 2], the x part of the player's directional vector.
	float	*diry;	// ptr to rays[1][SCN_WIDTH / 2], the y part of the player's directional vector.
	float	*fov_lx;// left most fov ray x
	float	*fov_ly;// left most fov ray y 
	float	*fov_rx;// right most fov ray x 
	float	*fov_ry;// right most fov ray y 
	t_rcast	rcast;
}	t_hero;

// struct of parameters used by render_walls()
typedef struct s_renderer_column_params
{
	mlx_image_t		*walls_layer;
	mlx_texture_t	*tex;
//	uint32_t		*init_pxls;// strat 2
	int				half_texh;// strat 1
	int				scn_height;
	int				half_height;// strat 1
	int				scn_start_y;
	float			ratio;
//	int			px_incry;
}	t_rcol;

typedef struct s_renderer
{
	mlx_image_t	*bg_layer;
	mlx_image_t	*walls_layer;
	int			requires_update;
}	t_rdr;

typedef struct s_cub3d_core_data
{
	/// MLX42 DATA
	mlx_t			*mlx;
//	mlx_image_t		*imgz;
	mlx_image_t		*color;
//	mlx_texture_t	*texr;

	int				tex_id;
	/// CONSTANT VALUES ////////////////////////////////////////
	int				scn_midx;	// mid screen x coordinate
	int				scn_midy;	// mid screen y coordinate
	float			inv_cw;		// inverse CELL_WIDTH. precalc const division for optimisation
	float			inv_sw;		// inverse SCN_WIDTH. precalc const used for skymap rendering.
	float			inv_two_pi;	// 1 / 2pi;
	/// FOV AND PROJECTION DATA ///////////////////////////////
	float			fov;// = fov;// field of view
	float			hfov;// = fov * 0.5f;// half fov
	float			near_z;// = (0.5f * (float)SCN_WIDTH) / tanf(cub->hfov);
	float			near_proj_factor;// = CELL_WIDTH * cub->near_z;
	/// SUBSECTIONS ////////////////////////////////////////////
	t_map			map;
	t_tex			tex;
	t_hero			hero;
	t_rdr			renderer;
}	t_cub;

/// PARSING ///////////////////

//int	load_map(t_cub *cub, char *map_file);
int		build_collision_map(t_map *map);
void	print_collision_map(t_map *map);
int		build_grid_coords_map(t_map *map);
void	print_map(t_map *map);

/// MAP_CHECKER ///////////////
//map_parse
t_map			*init_map(t_map *map);
int				map_checker(t_cub *cub, t_map *map, char *file);
int				tex_parse(t_cub *cub, t_map *map);
t_map			*wall_check(t_map *map);
//int				color_split(t_map *map, int id);
char			*skip_file_lines(t_map *map, int fd, int nb_lines);
//int				is_empty_line(char *line);
//map_tool
void			flush_empty_lines(char **raw);
int				error(char *error, t_map *map);
int				int_strlen(const char *s);
int				ft_in_set(char const c, char const *set);
int				ft_strfcmp(const char	*s1, const char	*s2, size_t n);
char			*ft_strncpy_i(char *dst, const char *src, \
size_t len, unsigned int idx);

t_cub			path_from_line(t_cub cub);

/// COLOR PARSE ////////////
int				str_to_color(int r, int g, int b, int t);
int				get_t(int trgb);
int				get_b(int trgb);
int				get_g(int trgb);
int				get_r(int trgb);
int				create_trgb(unsigned char t, unsigned char r, \
unsigned char g, unsigned char b);
unsigned char	get_ut(int trgb);
unsigned char	get_ur(int trgb);
unsigned char	get_ug(int trgb);
unsigned char	get_ub(int trgb);

/// EVENT HANDLERS ////////////
void			on_close(void *param);
void			on_keypress(mlx_key_data_t event, void *param);
void			on_scroll(double deltax, double deltay, void *param);
void			on_update(t_cub *param);
void			on_cursor_move(double xpos, double ypos, void *param);

/// RAYCASTER /////////////////
int				init_raycaster(t_cub *cub);
int				raycaster_clear(t_rcast *rcast, int exit_status);
void			update_rays(t_cub *cub);
void			update_fov(t_cub *cub, float fov);
int				is_wall(t_map *map, int cx, int cy);
int				get_is_cell_within_bounds(t_map *map, int cx, int cy);
//float	*get_grid_coords(t_map *map, int cx, int cy);

/// RENDERER /////////////////
int				init_renderer(t_cub *cub);
int				renderer_clear(t_cub *cub);
void			render_walls(t_cub *cub, t_rdata *rd);
void			mlx_set_color_in_rows(mlx_image_t *img, int start, int end, int col);
void			cub_put_pixel(mlx_image_t *img, int x, int y, int col);
void			clear_image_buffer(mlx_image_t *img);

//void			render_scene(t_cub *cub);

/// CHARACTER CONTROLS ////////
void			cub_player_rotate(t_cub *cub, float rot);
void			cub_player_move(t_cub *cub, float d_walk, float d_strafe);
void			cub_player_zoom(t_cub *cub, float dz);

/// ERROR HANDLING ////////////
int				report_err(char *msg);
int				report_err_strerror(char *msg);
int				report_mlx_init_error(void);
void			*report_mlx_tex_load_failed(char *tex);
int				report_malloc_error(void);

#endif
