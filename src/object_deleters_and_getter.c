/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_deleters_and_getter.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:28:07 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/01 20:30:12 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_oinst	*get_obj(t_cub *cub, int id)
{
	t_oinst	*elem;

//	printf("get oinst for id %d\n", id);
	elem = cub->objs.instances;
	while (elem)
	{
		if (elem->_id == id)
		{
			printf("OBJ with id %d FOUND : %p\n", id, elem);
			return (elem);
		}
		elem = elem->next;
	}
//	printf("No object with id %d\n", id);
	return (NULL);
}

// Delete one specific object with id.
int	delete_oinst_by_id(t_cub *cub, int id)
{
	t_oinst	*elem;
	t_oinst	*tmp;

	elem = cub->objs.instances;
	if (!elem)
		return (-1);
	if (elem->_id == id)
	{
		cub->objs.instances = elem->next;
		return (ft_free_p((void **)&elem));
	}
	while (elem->next)
	{
		if (elem->next->_id == id)
		{
			tmp = elem->next;
			elem->next = elem->next->next;
			return (ft_free_p((void **)&tmp));
		}
		elem = elem->next;
	}
	return (-1);
}

// Delete all instances of a specific object type. eg.: delete all OBJ_PORTAL
int	delete_all_oinst_by_type(t_cub *cub, int type_enum)
{
	t_oinst	*elem;
	t_oinst	*tmp;

	elem = cub->objs.instances;
	if (!elem)
		return (-1);
	if (elem->type->type_enum == type_enum)
	{
		cub->objs.instances = elem->next;
		return (ft_free_p((void **)&elem));
	}
	while (elem->next)
	{
		if (elem->next->type->type_enum == type_enum)
		{
			tmp = elem->next;
			elem->next = elem->next->next;
			ft_free_p((void **)&tmp);
		}
		else
			elem = elem->next;
	}
	return (0);
}

// Destroys all object instances in world.
void	delete_all_obj_instances(t_cub *cub)
{
	t_oinst	*elem;
	t_oinst	*tmp;

	if (!cub->objs.instances)
		return ;
	elem = cub->objs.instances;
	while (elem)
	{
		tmp = elem->next;
		ft_free_p((void **)&elem);
		elem = tmp;
	}
	cub->objs.instances = NULL;
}
/*
// static t_omdl	*init_portal_model(t_objs *objs)
// {
// 	const char	*tex_path1 = "tex/ext/Portal1.png";
// 	const char	*tex_path2 = "tex/ext/Portal_void_whitealpha.png";
// 	mlx_texture_t	*tex;

// 	objs->portal.model_name = "Portal";
// 	objs->portal.type_enum = OBJ_PORTAL;
// 	objs->portal.nb_texs = 2;
// 	objs->portal.draw_offy = 0;
// 	printf("Init Portal model ; Try load  png\n");
// 	tex = mlx_load_png(tex_path1);
// 	objs->portal.texs[0] = tex;
// 	if (!objs->portal.texs[0])
// 		return (report_mlx_tex_load_failed((char *)tex_path1));
// //	objs->portal.texs[1] = tex;//mlx_load_png(tex_path2);
// 	objs->portal.texs[1] = mlx_load_png(tex_path2);
// 	if (!objs->portal.texs[1])
// 		return (report_mlx_tex_load_failed((char *)tex_path2));
// 	printf("Init Portal model ; png load SUCCESSFUL !\n");
// 	objs->portal.width = CELL_WIDTH;
// 	objs->portal.half_w = objs->portal.width >> 1;
// 	objs->portal.height = objs->portal.width * (tex->height / (float)tex->width);
// 	objs->portal.half_h = objs->portal.height >> 1;
// //	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
// 	printf("Portal object model initialized !\n");
// 	return (&objs->portal);
// }

// static t_omdl	*init_fireball_model(t_objs *objs)
// {
// 	//const char	*tex_path1 = "tex/fireball/tmp/1_0.png";
// 	const char	*tex_path1 = "tex/fireball/alpha_fireball.png";
// 	const char	*tex_path2 = "tex/fireball/tmp/1_1.png";
// 	mlx_texture_t	*tex;

// 	objs->fireball.model_name = "Fireball";
// 	objs->fireball.type_enum = OBJ_FIREBALL;
// 	objs->fireball.nb_texs = 2;
// 	objs->fireball.draw_offy = 0;
// 	printf("Init Fireball model ; Try load  png\n");
// 	tex = mlx_load_png(tex_path1);
// 	objs->fireball.texs[0] = tex;
// 	if (!objs->fireball.texs[0])
// 		return (report_mlx_tex_load_failed((char *)tex_path1));
// 	objs->fireball.texs[1] = mlx_load_png(tex_path2);
// 	if (!objs->fireball.texs[1])
// 		return (report_mlx_tex_load_failed((char *)tex_path2));
// 	printf("Init fireball model ; png load SUCCESSFUL !\n");
// 	printf("tex w h (%d, %d)\n", tex->width, tex->height);
// 	objs->fireball.width = 48;
// 	objs->fireball.half_w = objs->fireball.width >> 1;
// 	objs->fireball.height = objs->fireball.width * (tex->height / (float)tex->width);
// 	objs->fireball.half_h = objs->fireball.height >> 1;
// 	printf("fireball w h : (%d, %d), half w h (%d, %d)\n", 
// 		objs->fireball.width, objs->fireball.height, objs->fireball.half_w, objs->fireball.half_h);
// 	objs->fireball.speed = 1.0f;
// 	objs->fireball.dmg = 5;
// //	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
// 	printf("fireball object model initialized !\n");
// 	return (&objs->fireball);
// }


// static t_omdl	*init_firepit_model(t_objs *objs)
// {
// 	//const char	*tex_path1 = "tex/fireball/tmp/1_0.png";
// 	const char	*tex_path1 = "tex/fireball/alpha_firepit.png";
// //	const char	*tex_path2 = "tex/fireball/tmp/1_1.png";
// 	mlx_texture_t	*tex;

// 	objs->firepit.model_name = "Fireball";
// 	objs->firepit.type_enum = OBJ_FIREPIT;
// 	objs->firepit.nb_texs = 1;
// 	objs->firepit.draw_offy = 20;
// 	printf("Init Fireball model ; Try load  png\n");
// 	tex = mlx_load_png(tex_path1);
// 	objs->firepit.texs[0] = tex;
// 	if (!objs->firepit.texs[0])
// 		return (report_mlx_tex_load_failed((char *)tex_path1));
// 	printf("Init firepit model ; png load SUCCESSFUL !\n");
// //	printf("tex w h (%d, %d)\n", tex->width, tex->height);
// 	objs->firepit.width = CELL_WIDTH;
// 	objs->firepit.half_w = objs->firepit.width >> 1;
// 	objs->firepit.height = objs->firepit.width * (tex->height / (float)tex->width);
// 	objs->firepit.half_h = objs->firepit.height >> 1;
// //	printf("firepit w h : (%d, %d), half w h (%d, %d)\n", 
// //		objs->firepit.width, objs->firepit.height, objs->firepit.half_w, objs->firepit.half_h);
// //	objs->firepit.speed = 1.0f;
// //	objs->firepit.dmg = 5;
// //	objs->portal.bypass_clr = *tex->pixels;//((uint32_t *)tex->pixels)[(tex->width >> 1) + (tex->height >> 1) * tex->width];
// 	printf("firepit object model initialized !\n");
// 	return (&objs->firepit);
// }
*/
