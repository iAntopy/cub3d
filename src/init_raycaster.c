/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:39:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/11 11:45:34 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	get_is_wall(t_map *map, int cx, int cy)
{
	return (map->collision_map[cy * map->width + cx]);
}

// Returns a float ptr to the x world coordinate of the NW corner of the
// (cx, cy) cell coordinate. fptr + 1 is the pointer to the y world coordinate.
float	*get_grid_coords(t_map *map, int cx, int cy)
{
	return (map->grid_coords[cy] + (cx << 1));
}
/*
static void	print_rays(t_mtx *xs, t_mtx *ys)
{
	mtx_print(xs);
	mtx_print(ys);
}
*/
void	print_map(t_map *map)
{
	int	i;
	char	**l;

	l = map->tab - 1;
	printf("printing map with w, h : %d, %d\n", map->width, map->height);
	while (*(++l))
	{
		i = -1;
		while (++i < map->width)
		{
			if ((*l)[i] == '\0')
				printf(" ");
			else
				printf("%c", (*l)[i]);
		}
		printf("\n");
	}
}

/*
static void	print_grid_coords(t_map *map)
{
	int		i;
	int		j;
	float	*fptr;

	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			fptr = get_grid_coords(map, j, i);
			printf("(%.3f, %.3f), ", *fptr, *(fptr + 1));
		}
		printf("\n");
	}
}
*/

void	print_collision_map(t_map *map)
{
	int	i;
	int	j;

	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (get_is_wall(map, j, i))
				printf("1");
			else
				printf("0");
		}
		printf("\n");
	}
}

typedef struct s_ray_intersect_data
{
	t_cub	*cub;
	int	*cell;
	int	*cside;
	float	*collisions;
	float	*dists;
	float	*texr;
//	float	*fish;
	float	**grid_coords;
	int		idx;

	int		sx;//	starting cell x	
	int		sy;//	starting cell y
	float	px;//	hero pos x
	float	py;//	hero pos y
	float	rx;//	ray vector delta x
	float	ry;//	ray vector delta y
//	int	dir[2];
	int		dirx;//	simple direction of ray. dx < 0 -> -1, dy >= 0 = 1
	int		diry;//	simple direction of ray. dy < 0 -> -1, dy >= 0 = 1

	int		c_offx;//	x offset of cell to check in collision map
	int		c_offy;//	y offset of cell to check in collision map

//	int		cx;//	current cell x
//	int		cy;//	current cell y
//	float	ix;//	current intersect x
//	float	iy;//	current intersect y
//	float	vdx;//	current vertical intersect delta from px;
//	float	vdy;//	current vertical intersect delta from py;
//	float	hdx;//	current horizontal intersect delta from px;
//	float	hdy;//	current horizontal intersect delta from py;
	float	p_dirx;//	x part of player's directional vector
	float	p_diry;//	y part of player's directional vector

	//	line func data Y = ax + b
	float	a;//	ray slope
	float	inv_a;//a inverse == 1/a;
	float	b;//	ray y offset
	
//	float	*dist;//	ptr to distance array where to put distance results

}	t_rayint;

int	build_collision_map(t_map *map)
{
	char	*colls;
	int	i;
	int	j;

	printf("build collision map : entered. malloc total cells : %d\n", map->total_cells);
	colls = NULL;
	if (!ft_malloc_p(sizeof(char) * map->total_cells, (void **)&colls))
		return (-1);
	printf("build collision map : malloced\n");
	
	i = -1;
	while (++i < map->height)
	{
		printf("build collision map : loopy %d\n", i);
		j = -1;
		while (++j < map->width)
			colls[i * map->width + j] = (map->tab[i][j] == '1');
	}
	printf("build collision map : out loopy\n");
	map->collision_map = colls;
	return (0);
}

int	build_grid_coords_map(t_map *map)
{
	int		i;
	int		j;
	float	**gcoords;

	printf("build grid coords entered \n");
	gcoords = NULL;
	if (!ft_malloc_p(sizeof(float *) * (map->height + 1), (void **)&gcoords))
		return (-1);
	printf("build grid coords : going on loopy\n");
	gcoords[map->height] = NULL;
	i = -1;
	while (++i < map->height)
	{
		if (!ft_malloc_p(2 * sizeof(float) * map->width, (void **)&gcoords[i]))
			return (-1);
		j = -1;
		while (++j < map->width)
		{
			gcoords[i][j << 1] = j * CELL_WIDTH;
			gcoords[i][(j << 1) + 1] = i * CELL_WIDTH;
		}
	}
	printf("build grid coords : going off loopy\n");

	map->grid_coords = gcoords;
	return (0);
}

/*
int	load_map(t_cub *cub, char *map_file)
{
	static char	*lines[10000];
	int		nl = 0;
	int		fd;
	int		cur_len;
	int		max_len;
	int		i;

	printf("Entered load map \n");
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		return (-1);
	max_len = 0;
	lines[nl] = get_next_line(fd);
	while (lines[nl])
	{
		cur_len = ft_strchr_set(lines[nl], "\n\0") - lines[nl];
		if (cur_len > max_len)
			max_len = cur_len;
		lines[++nl] = get_next_line(fd);
	}
	cub->map.width = max_len;
	cub->map.height = nl;
	cub->map.total_cells = max_len * nl;
	cub->map.width_px = max_len * CELL_WIDTH;
	cub->map.height_px = nl * CELL_WIDTH;
	cub->map.total_area = cub->map.width_px * cub->map.height_px;
	printf("map : width : %d\n", cub->map.width);
	printf("map : height : %d\n", cub->map.height);
	printf("map : area : %d\n", cub->map.total_area);
	if (!ft_malloc_p(sizeof(char *) * (nl + 1), (void **)&cub->map.tab))
		return (-1);
	cub->map.tab[nl] = NULL;
	while (nl--)
	{
		printf("map : loopy %d calloc %d len\n", nl, max_len + 1);
		ft_calloc_p(sizeof(char) * (max_len + 1), (void **)&cub->map.tab[nl]);
		ft_memcpy(cub->map.tab[nl], lines[nl], ft_strchr_set(lines[nl], "\n\0") - lines[nl]);
		ft_free_p((void **)&lines[nl]);
		i = -1;
		printf("map : replacing spaces\n");
		while (cub->map.tab[nl][++i] == ' ')
			cub->map.tab[nl][i] = '\0';
	}
	printf("map : printing \n");
	print_map(&cub->map);
	
	build_collision_map(cub);
	printf("map : print collision map : \n");
	print_collision_map(cub);
	
//	cub->map.grid_coords = mtx_create_empty(cub->map.total_cells, 2, DTYPE_F);
	
	printf("map : building grid coords map\n");
	build_grid_coords_map(cub);
//	printf("map : printing grid coords\n");
//	print_grid_coords(&cub->map);
	printf("EXIT LOAD MAP \n");
	return (0);
}
*/

static  void	raycast_init_single_vect(t_cub *cub, t_rayint *ri, int vi)
{
	ri->idx = vi;
	ri->rx = _mtx_index_f(cub->hero.rays[0], vi, 0);
	ri->ry = _mtx_index_f(cub->hero.rays[1], vi, 0);
	ri->c_offx = (ri->rx >= 0);
	ri->c_offy = (ri->ry >= 0);
	ri->dirx = (2 * ri->c_offx) - 1;
	ri->diry = (2 * ri->c_offy) - 1;
	ri->cell[0] = ri->sx;
	ri->cell[1] = ri->sy;
//	ri->cx = ri->sx;
//	ri->cy = ri->sy;
	ri->a = ri->ry / ri->rx;
	ri->inv_a = 1 / ri->a;
	ri->b = ri->py - (ri->a * ri->px);
//	printf("\n@----- Raycaster init ray %d with data -----@\n", vi);
//	printf("	- (dx, dy) : (%f, %f)\n", ri->rx, ri->ry);
//	printf("	- (c_offx, c_offy) : (%d, %d)\n", ri->c_offx, ri->c_offy);
//	printf("	- (dir_x, dir_y) : (%d, %d)\n", ri->dirx, ri->diry);
//	printf("	- (cx, cy) : (%d, %d)\n", ri->cx, ri->cy);
//	printf("	- (a, b) : (%f, %f)\n", ri->a, ri->b);
//	printf("@-------------------------------------------@\n\n");
}

// Returns 1 if wall was reached
int	raycast_find_cell_intersect(t_rayint *ri)
{
//	float	i_vert[2];//	x, y intersect of ray with next vertical cell segment.
//	float	i_hori[2];//	x, y intersect of ray with next horizontal cell segment.
//	float	axies[0];
	float	*axies;
	float	intersects[2];
	float	distv;
	float	disth;
//	float	*col_ptr;
//	int		test;
//	float	temp;
	int	is_hori;

//	printf("raycaster single ray check : \n");
	axies = ri->grid_coords[ri->cell[1] + ri->c_offy] + ((ri->cell[0] + ri->c_offx) << 1);// same as call to get_grid_coord ... but inline !
//	axies[0] = (*ri->cell + ri->c_offx) * CELL_WIDTH;
//	axies[1] = (ri->cell[1] + ri->c_offy) * CELL_WIDTH;
	intersects[1] = ri->a * (*axies) + ri->b;//		y = ax + b;
	(*intersects) = (axies[1] - ri->b) * ri->inv_a;//	x = (y - b) / a;
//	printf("(cx, cy) : (%d, %d), ray %d, axies : (%.3f, %.3f), intersects : (%.3f, %.3f), vert vs hero colision : (%f, %f)\n", ri->cx, ri->cy, ri->idx, axies[0], axies[1], intersects[0], intersects[1], axies[0] * axies[0] + intersects[1] * intersects[1], intersects[0] * intersects[0] + axies[1] * axies[1]);
	distv = (axies[0] - ri->px) * ri->p_dirx + (intersects[1] - ri->py) * ri->p_diry;
	disth = (intersects[0] - ri->px) * ri->p_dirx + (axies[1] - ri->py) * ri->p_diry;
//	ri->vdx = fabsf((*axies) - ri->px);
//	ri->vdy = fabsf(intersects[1] - ri->py);
//	ri->hdx = fabsf((*intersects) - ri->px);
//	ri->hdy = fabsf(axies[1] - ri->py);


// printf("(cx, cy) : (%d, %d), ray %d, axies : (%.3f, %.3f), intersects : (%.3f, %.3f), vert vs hero colision : (%f, %f)\n", ri->cx, ri->cy, ri->idx, axies[0], axies[1], intersects[0], intersects[1], ri->vdx + ri->vdy, ri->hdx + ri->hdy);
//	printf("(vdx, vdy) : (%f, %f), (hdx, hdy) : (%f, %f)\n", ri->vdx, ri->vdy, ri->hdx, ri->hdy);
//	if ((axies[0] * axies[0] + intersects[1] * intersects[1]) < (intersects[0] * intersects[0] + axies[1] * axies[1]))
//	if (((axies[0] - ri->px) + (intersects[1] - ri->py)) < (intersects[0] + axies[1]))
//	printf("previous cx, cy : (%d, %d)\n", ri->cx, ri->cy);
	/// BRANCHLESS 
/*
	test = ((ri->hdx + ri->hdy) < (ri->vdx + ri->vdy));
	ri->cy += test & ri->diry;
	ri->cx += (!test) & ri->dirx;
	ri->ix = *(axies + ((intersects - axies) & ((size_t)0 - test)));
	ri->iy = *(intersects + 1 + ((axies - intersects) & ((size_t)0 - test)));
*/

//	if (test)
//		printf("horizontal axis hit at (%f, %f)\n", ri->ix, ri->iy);
//	else
//		printf("vertical axis hit at (%f, %f)\n", ri->ix, ri->iy);

//	printf("current cx, cy : (%d, %d)\n", ri->cx, ri->cy);

//////// BRANCHLESS
//	is_hori = ((ri->hdx + ri->hdy) < (ri->vdx + ri->vdy));
//	ri->cell[is_hori] += ri->dir[is_hori];//*(&ri->dirx + is_hori);

//////// BRANCHFULL
//	if ((ri->hdx + ri->hdy) < (ri->vdx + ri->vdy))
	if (disth < distv)//(ri->hdx + ri->hdy) < (ri->vdx + ri->vdy))
	{
		ri->cell[1] += ri->diry;
		is_hori = 1;
	}
	else
	{
		*ri->cell += ri->dirx;
		is_hori = 0;
	}

//	if (is_hori)
//	{
//		printf("move y by %d\n", ri->diry);
//		ri->cell[1] += ri->diry;
//		ri->cy += ri->diry;
//		ri->ix = intersects[0];
//		ri->iy = axies[1];
//		is_hori = 0;
//	}
//	else
//	{
//		printf("move x by %d\n", ri->dirx);
//		ri->cell[0] += ri->dirx;
//		ri->cx += ri->dirx;
//		ri->ix = axies[0];
//		ri->iy = intersects[1];
//		is_hori = 1;
//	}


//	col_ptr = _mtx_index_fptr(ri->cub->hero.collisions, ri->idx, 0);
//	*(col_ptr++) = ri->ix;
//	*col_ptr = ri->iy;
//	return (get_is_wall(&ri->cub->map, ri->cx, ri->cy));
	if (get_is_wall(&ri->cub->map, ri->cell[0], ri->cell[1]))
	{
//		printf("ray idx %d hit wall at cell (%d, %d)\n\n", ri->idx, ri->cx, ri->cy);
//		printf("delta from \n\n", ri->idx, ri->cx, ri->cy);
//		col_ptr = _mtx_index_fptr(ri->cub->hero.collisions, ri->idx, 0);
//		*(++ri->cwall) = ri->cx;
//		*(++ri->cwall) = ri->cy;
//		*(++ri->collisions) = ri->ix;
//		*(++ri->collisions) = ri->iy;
//		*(++ri->cside) = is_hori + ((is_hori && ri->)<< 1);
		
		if (is_hori)
		{
//			printf("ray %d, hori hit collision : (%f, %f), axies[0] : %f, tex raw delta : %f\n",
//				ri->idx, intersects[0], axies[1], axies[0] - (CELL_WIDTH * ri->c_offx),
//				intersects[0] - (axies[0] - (CELL_WIDTH * ri->c_offx)));//, *(++ri->fish));

			*(++ri->collisions) = intersects[0];
			*(++ri->collisions) = axies[1];

			//// Distance fonction version
			//*(++ri->dists) = sqrtf(ri->hdx * ri->hdx + ri->hdy * ri->hdy) * (*(++ri->fish));
			//*(++ri->dists) = ri->hdx * fabsf(*ri->cub->hero.dirx) + ri->hdy * fabsf(*ri->cub->hero.diry);
			//*(++ri->dists) = (intersects[0] - ri->px) * (*ri->cub->hero.dirx) + (axies[1] - ri->py) * (*ri->cub->hero.diry);
			*(++ri->dists) = disth;
			

			*(++ri->cside) = 1 + (ri->c_offy << 1);
			*(++ri->texr) = (intersects[0] - (axies[0] - CELL_WIDTH * ri->c_offx)) * ri->cub->inv_cw;
		}
		else
		{
//			printf("ray %d, hori hit collision : (%f, %f), axies[0] : %f, tex raw delta : %f\n",
//				ri->idx, intersects[0], axies[1], axies[0] - (CELL_WIDTH * ri->c_offx),
//				intersects[0] - (axies[0] - (CELL_WIDTH * ri->c_offx)));//, *(++ri->fish));

			*(++ri->collisions) = axies[0];
			*(++ri->collisions) = intersects[1];
			

			//// Distance fonction version
			//*(++ri->dists) = sqrtf(ri->vdx * ri->vdx + ri->vdy * ri->vdy) * (*(++ri->fish));
			//*(++ri->dists) = ri->vdx * fabsf(*ri->cub->hero.dirx) + ri->vdy * fabsf(*ri->cub->hero.diry);
			//*(++ri->dists) = (axies[0] - ri->px) * (*ri->cub->hero.dirx) + (intersects[1] - ri->py) * (*ri->cub->hero.diry);
			*(++ri->dists) = distv;
			

			*(++ri->cside) = (ri->c_offx << 1);
			*(++ri->texr) = (intersects[1] - (axies[1] - CELL_WIDTH * ri->c_offy)) * ri->cub->inv_cw;
		}
		if ((*ri->cside == W_SIDE) || (*ri->cside == S_SIDE))
			*ri->texr = 1 - *ri->texr;
//		printf("raycaster : wall height = near_proj (%f) / dist found (%f) = %f\n", ri->cub->near_proj_factor,
//			(*ri->dists), ri->cub->near_proj_factor / (*ri->dists));
		*(++ri->texr) = ri->cub->near_proj_factor / (*ri->dists);// wall height on screen.
		ri->cell += 2;

		
		//// TEXTURE RATIOS 
//		W_SIDE -> 1 - ((intersects[1] - axies[1]) / CELL_WIDTH);
//		N_SIDE -> (intersects[0] - axies[0]) / CELL_WIDTH;
//		E_SIDE -> (intersects[1] - axies[1]) / CELL_WIDTH;
//		S_SIDE -> 1 - (intersects[0] - axies[0]) / CELL_WIDTH;

//		*(++ri->dists) = ri->ix - ri->px;//ri->vdx + (ri->hdx - ri->vdx) * test;
//		*ri->dists *= *ri->dists;
//		temp = ri->iy - ri->py; //ri->vdy + (ri->hdy - ri->vdy) * test;
//		*ri->dists += temp * temp;
		return (1);
	}
	return (0);
}

// Assumes vectors are initialised correctly.
int	raycast_all_vectors(t_cub *cub)
{
	t_rayint	ri;
	int			vi;// vector index from [0-SCN_WIDTH[

	ri.cub = cub;
	ri.cell = (int *)cub->hero.coll_walls->arr;// DO NOT OFFSET BY -1 !!!
	ri.cside = (int *)cub->hero.coll_sides->arr - 1;
	ri.collisions = (float *)cub->hero.collisions->arr - 1;
	ri.dists = (float *)cub->hero.distances->arr - 1;
	ri.texr = (float *)cub->hero.tex_infos->arr - 1;
//	ri.fish = (float *)cub->hero.fisheye_correctors->arr - 1;
	ri.grid_coords = cub->map.grid_coords;
	ri.sx = cub->hero.cell_x;
	ri.sy = cub->hero.cell_y;
	ri.px = cub->hero.px;
	ri.py = cub->hero.py;
	ri.p_dirx = *cub->hero.dirx;
	ri.p_diry = *cub->hero.diry;
//	collision_occured = 0;
	vi = -1;
	while (++vi < SCN_WIDTH)
	{
//		printf("vi : %d\n", vi);
		raycast_init_single_vect(cub, &ri, vi);
		while (!raycast_find_cell_intersect(&ri))
			continue ;
	}
//	__mtx_isqrtf(cub->hero.distances->arr, SCN_WIDTH);
//	printf("printing all collisions after raycasting : \n");
//	mtx_print(cub->hero.coll_walls);
//	mtx_print(cub->hero.coll_sides);
//	mtx_print(cub->hero.collisions);
//	mtx_print(cub->hero.distances);
//	mtx_print(cub->hero.tex_infos);
	return (0);
}

// If player rotates call this function.
void	update_rays(t_cub *cub)
{
	printf("update rays\n");
//	printf("ori : %f, ray_thetas[0] before : %f\n", cub->hero.ori, _mtx_index_f(cub->hero.ray_thetas, 0, 0));
	_mtx_addf_pscalar(cub->hero.theta_offsets, cub->hero.ori, cub->hero.ray_thetas);
//	printf("ray_thetas[0] after : %f\n", _mtx_index_f(cub->hero.ray_thetas, 0, 0));
	mtx_linspace_update(cub->hero.ray_thetas, 
		cub->hero.ori - cub->hfov, cub->hero.ori + FOV_HF, 1);
//	printf("rays[0][0] before : %f\n", _mtx_index_f(cub->hero.rays[0], 0, 0));
///	printf("rays[0][1] before : %f\n", _mtx_index_f(cub->hero.rays[1], 0, 0));
//	printf("rays[1][0] before : %f\n", _mtx_index_f(cub->hero.rays[0], 0, 0));
//	printf("rays[1][1] before : %f\n", _mtx_index_f(cub->hero.rays[1], 0, 0));
	mtx_cos(cub->hero.ray_thetas, cub->hero.rays[0]);
	mtx_sin(cub->hero.ray_thetas, cub->hero.rays[1]);
//	printf("rays[0][0] after : %f\n", _mtx_index_f(cub->hero.rays[0], 0, 0));
//	printf("rays[0][1] after : %f\n", _mtx_index_f(cub->hero.rays[1], 0, 0));
//	printf("rays[1][0] after : %f\n", _mtx_index_f(cub->hero.rays[0], 0, 0));
//	printf("rays[1][1] after : %f\n", _mtx_index_f(cub->hero.rays[1], 0, 0));
//	printf("fisheye correctors : \n");
//	mtx_print(cub->hero.fisheye_correctors);
	printf("player direction vector : [%f, %f]\n", *cub->hero.dirx, *cub->hero.diry);
	raycast_all_vectors(cub);
}

// If Zoom level (fov) changes call this function.
// fov in radians
// near_z is the distance from the player to the projection plan.
// near_proj_factor is a const as long as fov stays the same. Divide this 
//	factor by the rays length to get draw height.
void	update_fov(t_cub *cub, float fov)
{
	printf("update fov\n");
	cub->fov = fov;// field of view
	cub->hfov = 0.5f * fov;// half fov
	cub->near_z = (float)cub->scn_midx / tanf(cub->hfov);
	cub->near_proj_factor = CELL_WIDTH * cub->near_z;
	cub->skymap_fov_to_texture = cub->fov * cub->skymap_radial_width;
	update_floorcaster_params(cub);
//	printf("tanf(half fov) = %f\n", tanf(cub->hfov));
//	printf("(0.5f * (float)SCN_WIDTH(800)) / tanf(hfov) = %f\n", (0.5f * 800.0f) / tanf(cub->hfov));
//	printf("fov : %f, half_fov : %f, near_z : %f, near_proj_factor : %f\n",
//		cub->fov, cub->hfov, cub->near_z, cub->near_proj_factor);
	mtx_linspace_update(cub->hero.theta_offsets, -cub->hfov, cub->hfov, 1);
//	mtx_cos(cub->hero.theta_offsets, cub->hero.fisheye_correctors);
//	printf("fisheye correctors : \n");
//	mtx_print(cub->hero.fisheye_correctors);
	update_rays(cub);	
}

int	init_raycaster(t_cub *cub)
{
	t_hero	*hero;

	printf("Init raycaster :\n");
	hero = &cub->hero;
	hero->theta_offsets = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	hero->ray_thetas = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
//	hero->fisheye_correctors = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	hero->rays[0] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	hero->rays[1] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	hero->coll_walls = mtx_create_empty(SCN_WIDTH, 2, DTYPE_I);
	hero->coll_sides = mtx_create_empty(SCN_WIDTH, 1, DTYPE_I);
	hero->collisions = mtx_create_empty(SCN_WIDTH, 2, DTYPE_F);
	hero->distances = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
		
	// tex_infos :	row[0] : ratio of pixel column from left to right,
	// 		row[1] : total height of 
	hero->tex_infos = mtx_create_empty(SCN_WIDTH, 2, DTYPE_F);
	if (!hero->theta_offsets || !hero->ray_thetas
		|| !hero->rays[0] || !hero->rays[1] 
		|| !hero->coll_walls || !hero->coll_sides || !hero->collisions
		|| !hero->distances || !hero->tex_infos)
		return (-1);

	hero->dirx = _mtx_index_fptr(hero->rays[0], SCN_WIDTH / 2, 0);
	hero->diry = _mtx_index_fptr(hero->rays[1], SCN_WIDTH / 2, 0);
	update_fov(cub, FOV);
	printf("all matrix buffers created \n");
//	mtx_print(hero->theta_offsets);
//	mtx_print(hero->ray_thetas);
//	mtx_print(hero->fisheye_correctors);
//	print_rays(hero->rays[0], hero->rays[1]);
	return (0);
}
