/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:39:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/04 01:25:34 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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

static void	print_rays(t_mtx *xs, t_mtx *ys)
{
	mtx_print(xs);
	mtx_print(ys);
}

static void	print_map(t_map *map)
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

static void	print_collision_map(t_cub *cub)
{
	int	i;
	int	j;

	i = -1;
	while (++i < cub->map.height)
	{
		j = -1;
		while (++j < cub->map.width)
		{
			if (get_is_wall(&cub->map, j, i))
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
	int		idx;

	int		sx;//	starting cell x	
	int		sy;//	starting cell y
	float	px;//	hero pos x
	float	py;//	hero pos y
	float	rx;//	ray vector delta x
	float	ry;//	ray vector delta y
	int		dirx;//	simple direction of ray. dx < 0 -> -1, dy >= 0 = 1
	int		diry;//	simple direction of ray. dy < 0 -> -1, dy >= 0 = 1

	int		c_offx;//	x offset of cell to check in collision map
	int		c_offy;//	y offset of cell to check in collision map

	int		cx;//	current cell x
	int		cy;//	current cell y
	float	ix;//	current intersect x
	float	iy;//	current intersect y
	float	vdx;//	current vertical intersect delta from px;
	float	vdy;//	current vertical intersect delta from py;
	float	hdx;//	current horizontal intersect delta from px;
	float	hdy;//	current horizontal intersect delta from py;

	//	line func data Y = ax + b
	float	a;//	ray slope
	float	inv_a;//a inverse == 1/a;
	float	b;//	ray y offset
	
//	float	*dist;//	ptr to distance array where to put distance results

}	t_rayint;

int	build_collision_map(t_cub *cub)
{
	t_map	*map;
	char	*colls;
	int	i;
	int	j;

	map = &cub->map;
	if (!ft_malloc_p(sizeof(char) * map->total_cells, (void **)&colls))
		return (-1);
	
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
			colls[i * map->width + j] = (map->tab[i][j] == '1');
	}
	map->collision_map = colls;
	return (0);
}

int	build_grid_coords_map(t_cub *cub)
{
	int		i;
	int		j;
	float	**gcoords;

	if (!ft_malloc_p(sizeof(float *) * (cub->map.height + 1), (void **)&gcoords))
		return (-1);
	gcoords[cub->map.height] = NULL;
	i = -1;
	while (++i < cub->map.height)
	{
		if (!ft_malloc_p(2 * sizeof(float) * cub->map.width, (void **)&gcoords[i]))
			return (-1);
		j = -1;
		while (++j < cub->map.width)
		{
			gcoords[i][j << 1] = j * CELL_WIDTH;
			gcoords[i][(j << 1) + 1] = i * CELL_WIDTH;
		}
	}
	cub->map.grid_coords = gcoords;
	return (0);
}

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
	return (0);
}

static  void	raycast_init_single_vect(t_cub *cub, t_rayint *ri, int vi)
{
	ri->idx = vi;
	ri->rx = _mtx_index_f(cub->hero.rays[0], vi, 0);
	ri->ry = _mtx_index_f(cub->hero.rays[1], vi, 0);
	ri->c_offx = (ri->rx >= 0);
	ri->c_offy = (ri->ry >= 0);
	ri->dirx = (2 * ri->c_offx) - 1;
	ri->diry = (2 * ri->c_offy) - 1;
	ri->cx = ri->sx;
	ri->cy = ri->sy;
	ri->a = ri->ry / ri->rx;
	ri->inv_a = 1 / ri->a;
	ri->b = ri->py - (ri->a * ri->px);
	printf("\n@----- Raycaster init ray %d with data -----@\n", vi);
	printf("	- (dx, dy) : (%f, %f)\n", ri->rx, ri->ry);
	printf("	- (c_offx, c_offy) : (%d, %d)\n", ri->c_offx, ri->c_offy);
	printf("	- (dir_x, dir_y) : (%d, %d)\n", ri->dirx, ri->diry);
	printf("	- (cx, cy) : (%d, %d)\n", ri->cx, ri->cy);
	printf("	- (a, b) : (%f, %f)\n", ri->a, ri->b);
	printf("@-------------------------------------------@\n\n");
}

// Returns 1 if wall was reached
int	raycast_find_cell_intersect(t_rayint *ri)
{
//	float	i_vert[2];//	x, y intersect of ray with next vertical cell segment.
//	float	i_hori[2];//	x, y intersect of ray with next horizontal cell segment.
	float	*axies;
	float	intersects[2];
	float	*col_ptr;
	int		test;

	axies = get_grid_coords(&ri->cub->map, ri->cx + ri->c_offx, ri->cy + ri->c_offy);//(ri->cx + ri->c_offx) * CELL_WIDTH;// TODO: Change for indexing in reference coords map;
	intersects[1] = ri->a * axies[0] + ri->b;//		y = ax + b;
	intersects[0] = (axies[1] - ri->b) * ri->inv_a;//	x = (y - b) / a;
//	printf("(cx, cy) : (%d, %d), ray %d, axies : (%.3f, %.3f), intersects : (%.3f, %.3f), vert vs hero colision : (%f, %f)\n", ri->cx, ri->cy, ri->idx, axies[0], axies[1], intersects[0], intersects[1], axies[0] * axies[0] + intersects[1] * intersects[1], intersects[0] * intersects[0] + axies[1] * axies[1]);
	ri->vdx = fabsf(axies[0] - ri->px);
	ri->vdy = fabsf(intersects[1] - ri->py);
	ri->hdx = fabsf(intersects[0] - ri->px);
	ri->hdy = fabsf(axies[1] - ri->py);
//	printf("(cx, cy) : (%d, %d), ray %d, axies : (%.3f, %.3f), intersects : (%.3f, %.3f), vert vs hero colision : (%f, %f)\n", ri->cx, ri->cy, ri->idx, axies[0], axies[1], intersects[0], intersects[1], ri->vdx + ri->vdy, ri->hdx + ri->hdy);
//	printf("(vdx, vdy) : (%f, %f), (hdx, hdy) : (%f, %f)\n", ri->vdx, ri->vdy, ri->hdx, ri->hdy);
//	if ((axies[0] * axies[0] + intersects[1] * intersects[1]) < (intersects[0] * intersects[0] + axies[1] * axies[1]))
//	if (((axies[0] - ri->px) + (intersects[1] - ri->py)) < (intersects[0] + axies[1]))
	test = ((ri->vdx + ri->vdy) > (ri->hdx + ri->hdy));
//	printf("previous cx, cy : (%d, %d)\n", ri->cx, ri->cy);
	ri->cy += test & ri->diry;
	ri->cx += (!test) & ri->dirx;
	ri->ix = *(axies + ((intersects - axies) & ((size_t)0 - test)));//* test));// * test));
	ri->iy = *(intersects + 1 + ((axies - intersects) & ((size_t)0 - test)));//* test));
//	printf("current cx, cy : (%d, %d)\n", ri->cx, ri->cy);
	/*
	if ((ri->vdx + ri->vdy) > (ri->hdx + ri->hdy))
	{
//		printf("move y by %d\n", ri->diry);
		ri->cy += ri->diry;
		ri->ix = intersects[0];
		ri->iy = axies[1];
	}
	else
	{
//		printf("move x by %d\n", ri->dirx);
		ri->cx += ri->dirx;
		ri->ix = axies[0];
		ri->iy = intersects[1];
	}
	*/
//	col_ptr = _mtx_index_fptr(ri->cub->hero.collisions, ri->idx, 0);
//	*(col_ptr++) = ri->ix;
//	*col_ptr = ri->iy;
//	return (get_is_wall(&ri->cub->map, ri->cx, ri->cy));
	if (get_is_wall(&ri->cub->map, ri->cx, ri->cy))
	{
		printf("ray idx %d hit wall at cell (%d, %d)\n\n", ri->idx, ri->cx, ri->cy);
		col_ptr = _mtx_index_fptr(ri->cub->hero.collisions, ri->idx, 0);
		*(col_ptr++) = ri->ix;
		*col_ptr = ri->iy;
		return (1);
	}
	return (0);
}

// Assumes vectors are initialised correctly.
int	raycast_all_vectors(t_cub *cub)
{
	t_rayint	ri;
	int			vi;// vector index from [0-SCN_WIDTH[
//	int			collision_occured;

	ri.cub = cub;
	ri.sx = cub->hero.cell_x;
	ri.sy = cub->hero.cell_y;
	ri.px = cub->hero.px;
	ri.py = cub->hero.py;
//	collision_occured = 0;
	vi = -1;
	while (++vi < SCN_WIDTH)
	{
		raycast_init_single_vect(cub, &ri, vi);
		while (!raycast_find_cell_intersect(&ri))
			continue ;
	}
	printf("printing all collisions after raycasting : \n");
	mtx_print(cub->hero.collisions);
	return (0);
}

void	update_rays(t_cub *cub)
{
	mtx_linspace_update(cub->hero.thetas, 
		cub->hero.ori - FOV_HF, cub->hero.ori + FOV_HF, 1);
	mtx_cos(cub->hero.thetas, cub->hero.rays[0]);
	mtx_sin(cub->hero.thetas, cub->hero.rays[1]);
}

int	init_raycaster(t_cub *cub)
{
	t_hero	*hero;
	
	hero = &cub->hero;
	hero->thetas = mtx_linspace(hero->ori - FOV_HF, hero->ori + FOV_HF, SCN_WIDTH, 1);
	hero->rays[0] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	hero->rays[1] = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
	if (!hero->thetas)
		return (-1);
	hero->collisions = mtx_create_empty(SCN_WIDTH, 2, DTYPE_F);
	hero->distances = mtx_create_empty(SCN_WIDTH, 1, DTYPE_F);
//	mtx_print(hero->thetas);
//	mtx_display_info(hero->thetas);

	mtx_cos(hero->thetas, hero->rays[0]);
	mtx_sin(hero->thetas, hero->rays[1]);

//	mtx_print(hero->rays[0]);
//	mtx_display_info(hero->rays[0]);
//	mtx_print(hero->rays[1]);
//	mtx_display_info(hero->rays[1]);
	print_rays(hero->rays[0], hero->rays[1]);
	return (0);
}
