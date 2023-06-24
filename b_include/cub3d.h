/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:18:35 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/23 20:37:08 by iamongeo         ###   ########.fr       */
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

# define DEBUG 0
# define HD 0

/// SCN_WIDTH MUST BE > SCN_HEIGHT
# if HD
#  define SCN_WIDTH  1980
#  define SCN_HEIGHT 1080
# else
#  define SCN_WIDTH  1024
#  define SCN_HEIGHT 780
# endif

# define MMP_WIDTH  150
# define MMP_HEIGHT 150

# define ROT_FACTOR 0.006135923f
# define CELL_WIDTH 64
# define M_TAU 6.283185307179586f
# define M_INV_TAU 0.15915494309f

# define FOV120 2.0943951023931953f
# define FOV120_HF 1.0471975511965976f

# define FOV100 1.7453292519943295f
# define FOV100_HF 0.8726646259971647f

# define FOV90 1.5707963267948966f
# define FOV90_HF 0.7853981633974483f

# define FOV60 1.047197551196597746f
# define FOV60_HF 0.52359877559829887f

# define FOV45 0.785398163397448f
# define FOV45_HF 0.39269908169872414f

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

# define MAP_CHARS "0AB@"
# define LCHR "abcdefghijklmnopz"
# define NCHR "0123456789"
# define UCHR "ABCDEFGHIJ"
# define MCHR "!+-<>(){}_^=:;*#%@$&?"

# define MOD_LEV "!+-"
# define MOD_PORT "<>(){}_^"
# define MOD_FIRE "=:;*#%"
# define MOD_SPEC "@$&?"

# define ITOA "0123456789"

# define CUBMAP_BUFMAX 100000

# define PLAYER_HEIGHT 32

# define NB_DRAW_THREADS 6

# define PROJ_COLOR 0xffbcbbb0
# define TRANSPARENCY 0xafffffff

# define NB_OBJ_TYPES 5
# define FIREPIT_SPAWN_TICKS 200

# define MAX_PLAYERS 16

enum	e_sides
{
	W_SIDE = 0,
	N_SIDE = 1,
	E_SIDE = 2,
	S_SIDE = 3
};

enum	e_object_allegiance
{
	ALI_NEUTRAL,
	ALI_TORRENT,
	ALI_LEGION,
	ALI_ARMADA
};

enum	e_object_types
{
	OBJ_NULL,
	OBJ_LEVER,
	OBJ_PORTAL,
	OBJ_FIREPIT,
	OBJ_FIREBALL,
	OBJ_PLAYER,
	OBJ_SPAWNPOINT,
	OBJ_FLAG,
	OBJ_ACTIVATE,
	OBJ_DEACTIVATE
};

typedef struct s_object_model		t_omdl;
typedef struct s_raycaster_data		t_rcast;
typedef struct s_cub3d_core_data	t_cub;
typedef struct s_ray_collision_data	t_rdata;
typedef struct s_objects_list_elem	t_oinst;
typedef void						(*t_draw_func)(t_cub *cub);

/// PARSING ///////////////////
typedef struct s_matrx
{
	mlx_texture_t	*xwalls[8];
}		t_matrx;

typedef struct s_objx
{
	char			name;
	int				obj_id;
	int				opos[2];
	int				o_type;
	int				alleg;
	char			relativ;
	struct s_objx	*rel_ref;
	t_oinst			*wobj;
}	t_objx;

typedef struct s_box
{	
	mlx_texture_t	**xform;
	mlx_texture_t	*sky_tex;
	mlx_texture_t	*sky;
	int				pnum;

	char			*chrs;
	int				chrs_len;
	int				meta;
	int				xnum;

	int				n_dual;
	int				pset;

	int				open_sky;
	int				n_prts;
	int				n_lvls;
	int				n_fbll;
	int				n_fpit;
	int				n_plyr;
	int				nb_objx;

	t_objx			**objx;
	t_matrx			*gset;
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

	int		mmp_mid[2];
	int		mmp_ply_off[2];
	float	map_wld_x_rt;
	float	wld_map_x_rt;
	float	map_wld_y_rt;
	float	wld_map_y_rt;

	t_matrx	***mx;
	char	**raw;
	char	**m;
	int		pos_x;
	int		pos_y;
	int		lines_to_map;
	int		hero_side;
	int		hero_x;
	int		hero_y;
	int		flg_chk;
}	t_map;

typedef struct s_texture_data
{
	mlx_texture_t	*skymap;
}	t_tex;

typedef struct s_ray_collision_data
{
	int		idx;
	float	inv_cw;
	float	*near_proj_factor;
	int		*pcx;
	int		*pcy;
	float	*px;
	float	*py;
	float	*p_dirx;
	float	*p_diry;
	float	*rx;
	float	*ry;

	int		dx;
	int		dy;
	int		cincr_x;
	int		cincr_y;
	float	a;
	float	inv_a;
	float	b;

	int		cx;
	int		cy;

	int		side;
	float	hitx;
	float	hity;
	float	dist;
	float	tex_ratio;
	float	tex_height;

}	t_rdata;

typedef struct s_portal_projection_data
{
	t_rdata	*rdata;

	float	*fwd_len;

	float	px;
	float	py;
	int		cx;
	int		cy;

	float	b;

	int		side;
	float	hitx;
	float	hity;
	float	dist;
	float	odist;
	float	tex_ratio;
	float	tex_height;
}	t_pdata;

typedef struct s_raycaster_data
{
	t_cub		*cub;
	t_map		*map;
	t_mtx		*theta_offs;
	t_mtx		*ray_thetas;
	t_mtx		*fwd_rayspan;
	t_mtx		*rays[2];
	t_rdata		*rdata;
	t_pdata		*pdata;
}	t_rcast;

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
//	for each drawable object created. They are a constante definition of a model
//  type. eg.: a flying bullet, ennemy grunt ... They don't hold particular
//  instance data such as position or distance. Only generic global information
//  about a model type.
typedef struct s_object_model
{
	char			*model_name;
	int				type_enum;
	int				is_drawable;
	int				is_oriented;
	int				is_solid;
	int				width;
	int				half_w;
	int				height;
	int				half_h;
	float			proj_width;
	float			proj_height;
	int				offy;

	int				nb_texs;
	t_matrx			*gsets[4];

	float			speed;
	int				dmg;
}	t_omdl;

// returns 0 if possible and successful, otherwise -1.
typedef int							(*t_obj_act)(t_oinst *, t_cub *);

typedef struct s_objects_list_elem
{
	t_omdl						*type;
	int							_id;

	int							tex_idx;
	int							alleg;
	t_oinst						*spawnpoint;
	t_matrx						*gset;

	float						px;
	float						py;
	int							cx;
	int							cy;
	float						dx;
	float						dy;
	float						ori;

	float						target[2];
	float						speed;

	t_obj_act					action;

	float						ox;
	float						oy;
	float						dist;

	int							isactive;
	int							scheduled_for_deletion;
	int							counter;

	void						*relative;

	t_matrx						special_gset;

	struct s_objects_list_elem	*next;
}	t_oinst;

//	Container keeping all drawable object types as pointers to linked lists.
//	Each object type has its own linked list struct type. Renderer will 
//	go through each list and draw the objects on screen in any order 
//	and check weither it is in FOV first. If true, will check for each screen 
//	column it occupies (depending on object width and distance) if the object's 
//	distance to screen is smaller then the rays distance
//	(depth buffer, cub.hero.rcast.rdata[<column idx>].dist, length = SCN_WIDTH).
//	If true, draw object's texture column on screen and update the distance in 
//	depth buffer (rdata[i].dist). Each subsequant texture column being drawn
//	checks weither something has already been drawn in front of it. 
//	Objects from this list should be able to be added to their list and removed
//	and free.
//	
//	There should be a MAX_OBJ_DIST defined to bailout of a draw early
//	if obj is to far.
typedef struct s_drawable_objects
{
	t_omdl	player;
	t_omdl	spawnp;
	t_omdl	lever;
	t_omdl	portal;
	t_omdl	fireball;
	t_omdl	firepit;
	t_omdl	firepet;
	t_omdl	flag;
	t_oinst	*instances;
}	t_objs;

// struct of parameters used by render_walls()
typedef struct s_renderer_column_params
{
	mlx_image_t		*layer;
	mlx_texture_t	*tex;
	int				half_texh;
	int				scn_height;
	int				half_height;
	int				scn_start_y;
	int				scn_end_y;
	float			ratio;
}	t_rcol;

typedef struct s_main_character_data
{
	t_oinst		*ply_obj;
	float		*dirx;	
	float		*diry;	
	float		*fov_lx;
	float		*fov_ly;
	float		*fov_rx;
	float		*fov_ry;
	t_rcast		rcast;
}	t_hero;

typedef struct s_obj_intermediate_draw_data
{
	int				x;
	int				y;
	int				*pframe;
	t_pdata			*pdata;
	t_rdata			*rdata;
	t_pdata			*pd;
	t_rdata			*rd;

	t_oinst			*prtl;
	t_oinst			*link;
	t_oinst			*obj;
	float			*dbuff;
	uint32_t		*pbuff;
	char			*isproj;
	char			*ip;
	float			*dpbuff;
	float			*dp;
	float			ov[2];
	float			ppos[2];
	float			ratio;
	float			odist;
	float			pdist;
	mlx_texture_t	*tex;
	uint32_t		*pxls;
	uint32_t		*tp;
	uint32_t		cl;
	int				drawx;
	int				loffs[4];
	int				toffs[2];
	float			tincrs[2];
	int				dims[2];
	int				so;
	int				to;
	int				bincr;
	int				tys[SCN_WIDTH];
	int				lys[SCN_WIDTH];
}	t_objd;

typedef struct s_renderer
{
	mlx_image_t	*sky_layer;
	mlx_image_t	*bg_layer;
	mlx_image_t	*walls_layer;
	mlx_image_t	*proj_layer;
	mlx_image_t	*objs_layer;
	mlx_image_t	*mmap_layer;
	float		*dbuff;
	float		*dpbuff;
	char		*isproj;
	float		*near_z_dists;
	float		*floor_factors;
	t_oinst		*portal;
	int			bframe[4];
	int			pframe[4];

	float		flrw_to_cw;
	float		flrh_to_cw;
	float		sky_radial_width;
	float		sky_fov_to_tex;
	float		sky_ht_to_midy;
	int			sky_yoffsets[SCN_HEIGHT >> 1];
	int			sky_ori_offset;
	int			requires_update;
}	t_rdr;

typedef struct s_cub3d_core_data
{
	mlx_t			*mlx;
	mlx_image_t		*color;
	int				nb_players;
	int				player_ids[MAX_PLAYERS];
	int				scn_midx;	
	int				scn_midy;	
	float			inv_cw;	
	float			inv_sw;	
	float			inv_two_pi;	
	int				yoffs[SCN_HEIGHT];

	float			fov;
	float			hfov;
	float			near_z;
	float			near_proj_factor;

	t_map			map;
	t_tex			tex;
	t_hero			hero;
	t_rdr			renderer;
	t_objs			objs;
	t_thdraw		draw_threads[NB_DRAW_THREADS];
	t_matrx			*dual;
	t_matrx			*pset;
	t_box			box;
}	t_cub;

int				build_collision_map(t_map *map);
void			print_collision_map(t_map *map);
int				build_grid_coords_map(t_map *map);
void			print_map(t_map *map);

/// MAP_CHECKER ///////////////
t_cub			*wall_check(t_cub *cub, t_map *map, t_objx **objx);
t_cub			*mapx_alt_pos(t_map *m, t_cub *cub, int p_box);
t_objx			*init_objx(t_cub *cub, int o_cells, int id);
int				get_types(t_objx *objx, int head);

t_map			*init_map(t_map *map);
int				map_checker(t_cub *cub, t_map *map, char *file);
int				tex_parse(t_cub *cub, t_map *map);
char			*skip_file_lines(t_map *map, int fd, int nb_lines);
//map_tool
void			flush_empty_lines(char **raw);
int				error(char *error, t_map *map);
int				int_strlen(const char *s);
int				ft_in_set(char const c, char const *set);
int				ft_strfcmp(const char	*s1, const char	*s2, size_t n);
char			*ft_strncpy_i(char *dst, const char *src, \
size_t len, unsigned int idx);

t_cub			path_from_line(t_cub cub);
int				instanciate_map_objects(t_cub *cub);
float			objx_get_side_ori(t_objx *ob);

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
int				update_rays(t_cub *cub);
int				update_fov(t_cub *cub, float fov);
int				is_wall(t_map *map, int cx, int cy);
int				get_is_cell_within_bounds(t_map *map, int cx, int cy);
float			*get_grid_coords(t_map *map, int cx, int cy);

/// RENDERER /////////////////
int				init_renderer(t_cub *cub);
void			init_rdata_consts(t_cub *cub, t_rcast *rc, \
t_rdata *rd, t_pdata *pd);
int				renderer_clear(t_cub *cub, int exit_status);
void			render_walls(t_cub *cub);
void			render_floor_sky(t_cub *cub);
void			render_objects(t_cub *cub);
void			__render_sky(t_cub *cub);
extern void		__rdr_select_draw_texture(t_objd *od, t_oinst *obj);
extern void		__rdr_setup_draw_objs(t_cub *cub, t_objd *od, \
int *pframe, int offy);
extern void		__rdr_obj_draw_check(t_cub *cub, t_objd *od);
extern int		__rdr_obj_out_of_frame(t_objd *od);
extern void		__label_isproj(uint32_t *pb, char *ip, int *pf, int *pdims);
int				prtl_proj_vectors(t_pdata *pd, t_map *map, t_oinst *obj, int n);
void			__render_proj_sky(t_cub *cub, uint32_t *pbuff, int *pframe);
void			__render_proj_walls(t_cub *cub);
void			__render_proj_floor(t_cub *cub);
void			__render_proj_objects(t_cub *cub);
void			mlx_set_color_in_rows(mlx_image_t *im, int st, int ed, int col);
void			mlx_draw_square(mlx_image_t *im, int p[2], int s, uint32_t col);
void			cub_put_pixel(mlx_image_t *img, int x, int y, int col);
void			clear_image_buffer(mlx_image_t *img);
uint32_t		get_tex_pixel(mlx_texture_t *tex, int x, int y);

/// FLOORCASTING ///////////////
int				init_floorcaster(t_cub *cub);
void			update_floorcaster_params(t_cub *cub);
float			get_floorcaster_param(t_cub *cub, int x, int y);
int				clear_floorcaster(t_cub *cub);

/// SKYCASTING //////////////
int				init_skycaster(t_cub *cub);

/// DRAW THREADS API
int				init_draw_threads(t_cub *cub, t_thdraw *threads);
int				order_draw_call(t_thdraw *threads, int from, int to);
void			stop_draw_threads(t_thdraw *threads);

/// OBJECT MANAGEMENT SYSTEM ////////
int				get_new_obj_id(void);
int				obj_get_type(t_oinst *obj);
int				obj_get_width(t_oinst *obj);
int				obj_get_issolid(t_oinst *obj);
int				obj_get_isactive(t_oinst *obj);
t_oinst			*get_obj(t_cub *cub, int id);
int				*obj_type_alleg(int type, int alleg);
void			clear_obj_framework(t_cub *cub);
int				create_obj_instance(t_cub *cub, float *pos, int *type_alleg, \
void *param);
int				delete_oinst_by_id(t_cub *cub, int id);
int				delete_oinst_by_type(t_cub *cub, int type_enum);
void			delete_all_obj_instances(t_cub *cub);
void			delete_all_scheduled_for_deletion(t_cub *cub);
int				obj_schedule_for_deletion(t_oinst *obj);

int				link_portal_instances(t_oinst *prtl1, t_oinst *prtl2);
int				link_lever_to_portal(t_oinst *lever, t_oinst *prtl);
int				link_fireball_to_target(t_oinst *fball, t_oinst *target);
int				link_firepit_to_target(t_oinst *fball, t_oinst *target);

/// OBJECT INSTANCIATOR (DO NOT USE DIRECTELY ! USE create_obj_instance())
int				create_spawnp_instance(t_cub *cub, float *pos, int alleg);
int				create_player_instance(t_cub *cub, float *pos, int alleg, \
t_oinst *spawnp);
int				create_lever_instance(t_cub *cub, float *pos, int alleg, \
t_oinst *link);
int				create_portal_instance(t_cub *cub, float *pos, int alleg, \
t_oinst *link);
int				create_fireball_instance(t_cub *cub, float *pos, int alleg, \
t_oinst *link);
int				create_firepit_instance(t_cub *cub, float *pos, int alleg, \
t_oinst *link);

/// OBJECT CONTROLS
void			obj_move_rel(t_cub *cub, t_oinst *obj, float walk, float straf);
void			obj_move_abs(t_cub *cub, t_oinst *obj, float walk, float straf);
void			obj_rotate(t_cub *cub, t_oinst *obj, float rot);
void			obj_set_orientation(t_cub *cub, t_oinst *obj, float ori);
void			obj_set_position(t_cub *cub, t_oinst *obj, float px, float py);
void			obj_set_direction(t_cub *cub, t_oinst *obj, float dx, float dy);
void			obj_look_at(t_oinst *obj, t_oinst *target);
void			manage_collisions(t_cub *cub, t_oinst *ply, float *mv);

/// OBJECT ACTIVATION FUNCS /////////
void			commit_all_obj_actions(t_cub *cub);
int				activate_portal(t_oinst *obj, unsigned int new_status);
int				activate_fireball(t_oinst *obj, int new_state, t_oinst *target);
int				activate_firepit(t_oinst *obj, int new_state, t_oinst *target);
int				spawn_new_player(t_oinst *spawnp, int is_playable);
int				set_playable_obj(t_cub *cub, t_oinst *player);
int				respawn_player(t_oinst *player);

/// OBJECT ACTIONS CALLBACKS
int				__obj_action_player(t_oinst *obj, t_cub *cub);
int				__obj_action_spawnpoint(t_oinst *obj, t_cub *cub);
int				__obj_action_portal(t_oinst *obj, t_cub *cub);
int				__obj_action_fireball(t_oinst *obj, t_cub *cub);
int				__obj_action_firepit(t_oinst *obj, t_cub *cub);
int				__obj_action_lever(t_oinst *obj, t_cub *cub);

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
int				report_threads_err(t_thdraw *threads, char *err, int print_err);
int				report_obj_err(t_oinst *obj, char *msg);

/// MODEL ////////////////////
t_omdl			*init_player_model(t_objs *objs);
t_omdl			*init_spawnpoint_model(t_objs *objs);
t_omdl			*init_portal_model(t_objs *objs);
t_omdl			*init_lever_model(t_objs *objs);
t_omdl			*init_fireball_model(t_objs *objs);
t_omdl			*init_firepit_model(t_objs *objs);
t_omdl			*init_flag_model(t_objs *objs);

/// TESTING TXTR_DICT
t_matrx			*pset_maker(t_cub *cub, char **raw, int queue, int len);
t_box			*xwalls_builder(t_cub *cub, char **raw);

t_matrx			***init_mx(t_map *m);
t_matrx			*gset_builder(const char *path, int txtr_nb);
t_cub			*dual_builder(t_cub *cub, int i, char *t_name);
t_cub			*meta_builder(t_cub *cub, t_box *box, char *name, t_objs *objs);
t_cub			*mapx_builder(t_map *m, t_cub *cub);

int				xform_builder(t_cub *cub, char *t_name, char *t_path, int j);
t_cub			*e_mtrx_link(t_cub *cub, t_box *box, char **raw);
t_cub			*e_list_txtr(t_cub *cub, t_box *box, t_map *map);
t_cub			*e_mtrx_count(t_cub *cub);
void			p_list_objx(t_objx **objx, int id, int num);
char			*chrs_builder(t_cub *cub);
t_objx			*objx_init(t_objx *objx);
t_objx			*data_objx(t_cub *cub, t_box *box, char meta);
t_objx			*get_pos(t_cub *cub, t_map *m, int o_cells, int id);
t_objx			*get_ref(t_cub *cub, t_objx *objx, int id);
int				get_objx(t_objx **objx, char name, int num);
t_cub			*mx_struct(t_map *m, t_cub *cub);
int				clr_legend_strct(t_cub *cub);
int				xform_flush(t_cub *cub);

void			minimap_set_pos(t_cub *cub);
void			minimap_surround(t_cub *cub, int pos[2], int mmax[2]);
void			mlx_draw_mmap(t_cub *cub);
void			mlx_draw_player(t_cub *cub, t_map *map);
void			mlx_update_mmap(t_cub *cub, t_map *m);

/// UTILS
int				get_cell(float px, float py, int *cx, int *cy);
float			normalize_vec2(float *v, float *dist_p);
int				next_obj(t_oinst **obj_p);
void			find_vector_delta(float *from_, float *to_, float *res);

/// MINIMAP FUNCS
void			update_minimap(t_cub *cub);
int				chrs_checker(t_cub *cub, int j, char *tex_name, char *tex_path);
char			**name_builder(const char *path, int txtr_nb);
void			for_free(char **test);
void			solo_free(char *test);
#endif
