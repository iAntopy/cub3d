/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:18:35 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/08 21:45:58 by iamongeo         ###   ########.fr       */
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
# include <pthread.h>

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft.h"
# include "../lib/mtxlib/includes/mtxlib.h"

# define SCN_WIDTH  1024
# define SCN_HEIGHT 780

# define MMP_WIDTH  150
# define MMP_HEIGHT 150

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
# define MAP_CHARS "0AB@"
# define MAP_LCHR "abcdefghijz"
# define MAP_NCHR "0123456789"
# define MAP_UCHR "ABCDEFGHIJ"

# define PLAYER_HEIGHT 32

# define NB_DRAW_THREADS 2

# define PROJ_COLOR 0xffbcbbb0
# define LENS_EFFECT_RAD FOV90//(M_PI * 0.5)

enum	e_sides
{
	W_SIDE = 0,
	N_SIDE = 1,
	E_SIDE = 2,
	S_SIDE = 3
};

typedef struct s_raycaster_data		t_rcast;
typedef struct s_cub3d_core_data	t_cub;
typedef struct s_ray_collision_data	t_rdata;
typedef struct s_objects_list_elem	t_oinst;
typedef void				(*t_draw_func)(t_cub *, t_rdata *);


/// PARSING ///////////////////
typedef struct s_matrx
{
	mlx_texture_t	*xwalls[4];
}		t_matrx;

typedef struct s_box
{	
	char 			*chrs;
	int				chrs_len;
	int 			xnum;	
	int				pnum;
	int				pset;
	mlx_texture_t	**xform;
	mlx_texture_t	*sky;	
}	t_box;

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

	t_matrx	***mx;
	char	**raw;
	char	**m;	// test mapping map
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
	int				color[2];
//	mlx_texture_t	*walls[4];
	mlx_texture_t	*skymap;	// yessss
	mlx_texture_t	*floor;		// yessss
	char			**rgbx;
	int				open_sky;
}	t_tex;

typedef struct s_ray_collision_data
{
//	External ref
//	t_rcast	*rcast;

//	Struct constants;
	int		idx;
	float		inv_cw;
	float		*near_proj_factor;
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

typedef struct s_portal_projection_data
{
	t_rdata	*rdata;
	
//	Struct const
	float	*fwd_len;// sin(theta offset for this ray). used in portal projection to find ray collision on obj

//	Init data;
	float		odist;//distance to obj;
	float		px;//	init as player px, switches to ray intersect with obj, offset to link portal during proj
	float		py;//	init as player py, switches to ray intersect with obj, offset to link portal during proj
	int		cx;//	init as player cx, switches to cell x of px, offset to link portal during proj
	int		cy;//	init as player cy, switches to cell y of px, offset to link portal during proj
	int		tgt_px;//	x coord ray collision with object
	int		tgt_py;//	y coord ray collision with object
	int		tgt_cx;//	cell x of collision with object
	int		tgt_cy;//	cell y of collision with object
	float		rx;
	float		ry;

	int		dx;//	x offset of cell to check in collision map
	int		dy;//	y offset of cell to check in collision map
	int		cincr_x;//	direction of cell move for vertical axis collision. Either 1 or -1.
	int		cincr_y;//	direction of cell move for horizontal axis collision. Either 1 or -1.
	float	a;//	ray slope
	float	inv_a;//a inverse == 1/a;
	float	b;//	ray y offset

	
//	Resulting data
	int		side;// collision side. Can be compared to side enums.
	float	hitx;// collision world coord x;
	float	hity;// collision world coord y;
	float	dist;// collision distance to projection plane.
	float	tex_ratio;// ratio of hit on wall from left to right. Used to find drawn texture column.
	float	tex_height;// texture height on projection screen. Can be greater then SCN_HEIGHT.
}	t_pdata;

typedef struct s_raycaster_data
{
	t_cub		*cub;
	t_map		*map;
	t_mtx		*theta_offs;// Angle offsets for each angle from 0. Malloced mtx.
	t_mtx		*ray_thetas;// Angles for each ray. Malloced mtx.
	t_mtx		*fwd_rayspan;// sin of all theta_offs. updated in update_fov.
	t_mtx		*rays[2];// X, Y part for each ray vector. index 0 are Xs, 1 are Ys Malloced mtx.
	t_rdata		*rdata;//	malloced array of struct with collision data. len SCR_WIDTH
	t_pdata		*prtl_proj;//	idem but used excusively for raycasting portal projections
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

typedef struct s_draw_thread_profil
{
	t_cub			*cub;
	pthread_mutex_t	start_lock;
	pthread_mutex_t	end_lock;
	t_draw_func		draw_func;
	int				start_lock_isinit;
	int				end_lock_isinit;
	pthread_t		id;
	int				isidle;
	int				stop_request;
}	t_thdraw;

//	Models are initialised only once at start. A pointer to a model is required
//	for each drawable object created. They are a constante definition of a model type.
//	eg.: a flying bullet, ennemy grunt ... They don't hold particular instance data such as position
//	or distance. Only generic global information about a model type.
typedef struct s_object_model
{
	char			*model_name;//	For debug info and logging purposes.
	int				type_enum;
	int				width;// Width of object in world coords.
	int				half_w;
	float				h_to_w;// height to width ratio
	float				w_to_h;// height to width ratio
	int				height;// Height of object in world coords (set auto).
	int				half_h;
//	uint32_t		bypass_clr;// exterior color around the portal that should nor be drawn.

	int				nb_texs;// Max nb of textures for this particular model.
						// Multi textures used for animation or to simulate object orientation.
	mlx_texture_t	*texs[8];//	Array of pointers to model textures. Max 8 textures for animation if necessary.

	/// OPTIONAL FIELDS //////
	int				dmg;
}	t_omdl;

enum	e_object_types
{
	OBJ_NULL,
	OBJ_ACTIVATE,
	OBJ_DEACTIVATE,
	OBJ_PORTAL
};

typedef struct s_objects_list_elem
{
	t_omdl	*type;
	int		_id;//	unique obj id. Objects are deletable by id.
	
	int		tex_idx;
	
	float		px;//	Position X
	float		py;//	Position Y

	/// VARS SET AT RENDER TIME ////////////
	float		ox;//	obj delta x from player
	float		oy;//	obj delta y from player
//	float		ux;//	obj unit delta x from player
//	float		uy;//	obj unit delta y from player
	float		dist;//	distance from player

	float		ox_left;//	obj delta x left edge of obj, perpendicular to [ox, oy] vect
	float		oy_left;//	obj delta y left edge of obj, perpendicular to [ox, oy] vect
	float		ox_right;//	obj delta x right edge of obj, perpendicular to [ox, oy] vect
	float		oy_right;//	obj delta y right edge of obj, perpendicular to [ox, oy] vect

//	float		divergent_lens_ra
	
	int		isactive;
	// PORTAL SPECIFIC
	t_oinst		*link;

	struct s_objects_list_elem	*next;
}	t_oinst;

//	Container keeping all drawable object types as pointers to linked lists.
//	Each object type has its own linked list struct type. Renderer will 
//	go through each list and draw the objects on screen in any order 
//	and check weither it is in FOV first. If true, will check for each screen 
//	column it occupies (depending on object width and distance) if the object's 
//	distance to screen is smaller then the rays distance (depth buffer, cub.hero.rcast.rdata[<column idx>].dist, length = SCN_WIDTH).
//	If true, draw object's texture column on screen and update the distance in 
//	depth buffer (rdata[i].dist). Each subsequant texture column being drawn checks 
//	weither something has already been drawn in front of it. 
//	Objects from this list should be able to be added to their list and removed and free
//	
//	There should be a MAX_OBJ_DIST defined to bailout of a draw early if obj is to far.
typedef struct s_drawable_objects
{
	/// OBJECT MODELS (constant) /////////////////////////
	t_omdl	portal;//	Portal object model;
	
	/// MUTABLE LINKED LISTS OF DRAWABLE OBJECT INSTANCES ///////
	//t_oclct	*collectibles;
	//t_oenmi	*ennemies;
	t_oinst	*instances;
}	t_objs;

// struct of parameters used by render_walls()
typedef struct s_renderer_column_params
{
	mlx_image_t		*layer;
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
//	mlx_image_t	*proj_layer;
	mlx_image_t	*objs_layer;
	mlx_image_t	*mmap_layer;
	float		*dbuff;//	 depth buffer for drawable world entities. 
	float		*dpbuff;//	 depth buffer for portal projection entities.
	char		*isproj;//	 bool buffer SCN_WIDTH x SCN_HEIGHT indicating if pxl is portal projection
	float		*near_z_dists;// Array of distances to every column of the projected
				// plane (near_z). See floorcaster. 
	float		*floor_factors;// Pre-calc parametric multipliers for all pixels
			// below scn_midy. Every drawn pixels on screen 
			// is mapped to a multiplier (see floor_caster.c)
			// that stretches rx and ry to find the floor pixel it hits.
//	int			*sky_base_toffs;
//	int			*sky_toffs;
	float		flrw_to_cw;
	float		flrh_to_cw;
	float		sky_radial_width;	// const sky texture width * inv_two_pi
	float		sky_fov_to_tex;// FOV60 * sky_radial_width;
	float		sky_ht_to_midy;//	tex_height / (SCN_HEIGHT / 2)
	int			sky_yoffsets[SCN_HEIGHT >> 1];
	int			sky_ori_offset;
	int			requires_update;
}	t_rdr;

typedef struct s_cub3d_core_data
{
	/// MLX42 DATA
	mlx_t			*mlx;
//	mlx_image_t		*imgz;	// maybe for mini_map
	mlx_image_t		*color;

	/// TEMP VARS FOR TESTING AND DEBUG ONLY ///////////////////
//	mlx_texture_t	*floor_tex;
//	mlx_texture_t	*sky_tex;

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
	t_objs			objs;
	t_thdraw		draw_threads[NB_DRAW_THREADS];
	t_matrx			*pset;
	t_box			box;
}	t_cub;

//int	load_map(t_cub *cub, char *map_file);
int		build_collision_map(t_map *map);
void	print_collision_map(t_map *map);
int		build_grid_coords_map(t_map *map);
void	print_map(t_map *map);

/// MAP_CHECKER ///////////////
//map_parse
t_cub			*wall_check(t_cub *cub,t_map *map);
t_map			*init_map(t_map *map);
int				map_checker(t_cub *cub, t_map *map, char *file);
int				tex_parse(t_cub *cub, t_map *map);
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
void			render_floor_sky(t_cub *cub, t_rdata *rd);
void			render_floor_sky_proj(t_cub *cub, uint32_t *pbuff, t_pdata *pd, int *pframe);
void			render_objects(t_cub *cub);//, t_rdata *rd);
//void			render_sky(t_cub *cub, t_rdata *rd);
void			mlx_set_color_in_rows(mlx_image_t *img, int start, int end, int col);
void			cub_put_pixel(mlx_image_t *img, int x, int y, int col);
uint32_t		get_tex_pixel(mlx_texture_t *tex, int x, int y);
void			clear_image_buffer(mlx_image_t *img);
//void			render_scene(t_cub *cub);

/// FLOORCASTING ///////////////
int				init_floorcaster(t_cub *cub);
void			update_floorcaster_params(t_cub *cub);
float			get_floorcaster_param(t_cub *cub, int x, int y);
int				clear_floorcaster(t_cub *cub);

/// SKYCASTING //////////////
int				init_skycaster(t_cub *cub);
/*
void			update_sky_base_toffs(t_cub *cub, int *base_toffs, int *toffs);
void			update_sky_toffs(t_cub *cub, int *base_toffs, int *toffs);
int				clear_skycaster(t_cub *cub);
*/

/// DRAW THREADS API
int				init_draw_threads(t_cub *cub, t_thdraw *threads);
int				order_draw_call(t_cub *cub, t_thdraw *threads);
void			stop_draw_threads(t_thdraw *threads);

/// OBJECT MANAGEMENT SYSTEM ////////
int				init_obj_framework(t_cub *cub);
void			clear_obj_framework(t_cub *cub);
int				create_obj_instance(t_cub *cub, int *pos, int type_enum, void *param);
int				destroy_oinst_by_id(t_cub *cub, int id);
t_oinst			*get_oinst_by_id(t_cub *cub, int id);
int				destroy_oinst_by_type(t_cub *cub, int type_enum);
void			destroy_all_obj_instances(t_cub *cub);

/// OBJECT ACTIVATION FUNCS /////////
int				activate_portal(t_oinst *obj, int deactivate);

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

/// TESTING TXTR_DICT
t_matrx			*pset_maker(t_cub *cub, char **raw, int queue, int len);
t_box 			*xwalls_builder(t_cub *cub, char **raw);
t_cub			*chsr_feed(t_cub *cub);
t_box	 		*e_mtrx_link(t_box *box, char **raw);
t_cub			*e_mtrx_count(t_cub *cub);
t_cub			*e_list_txtr(t_cub *cub, t_box *box, t_map *map);
t_cub			*mx_struct(t_map *m, t_cub *cub);
void			clr_legend_strct(t_box box);

#endif
