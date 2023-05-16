/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_instanciator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 06:25:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/15 21:02:36 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_objx	*find_relative(t_cub *cub, t_objx *ob, int nb_meta)
{
	t_objx	*other;
	int		i;
	
	printf("Recurcive find relative entered. Searching for rel : %c\n", ob->relativ);
	if (!ob->relativ)
		return (NULL);
	i = -1;
	while (++i < nb_meta)
	{
		other = cub->box.objx[i];
		if (other->name == ob->relativ)
			return (other);
	}
	return (NULL);	
}

static t_oinst	*instanciate_specific_obj(t_cub *cub, t_objx *ob, int nb_meta)
{
	float	pos[2];
	
	if (ob->obj_id < 0 || ob->wobj || ob->o_type < 1 || NB_OBJ_TYPES < ob->o_type
			|| ob->alleg < 1 || 3 < ob->alleg)
			return (NULL);
	
	pos[0] = (float)ob->opos[0] * CELL_WIDTH + (CELL_WIDTH >> 1);
	pos[1] = (float)ob->opos[1] * CELL_WIDTH + (CELL_WIDTH >> 1);
	printf("inst one Check PASSED\n");
	printf("Creating obj inst at pos (%d, %d)\n", ob->opos[0], ob->opos[1]);
	ob->obj_id = create_obj_instance(cub, pos, ob->o_type, ob->alleg, NULL);
	ob->wobj = get_obj(cub, ob->obj_id);
	printf("MAP INSTANCIATOR CREATED OBJ : %p\n", ob->wobj);
	ob->rel_ref = find_relative(cub, ob, nb_meta);
	return (ob->wobj);
}

int	instanciate_map_objects(t_cub *cub)
{
	int	nb_meta;
	int	i;

	printf("\nStarting map instanciation \n");
	if (!cub || !cub->box.objx)
		return (-1);
	nb_meta = cub->box.meta;
	printf(" Check passed, nb_meta : %d\n", nb_meta);
	i = -1;
	while (++i < nb_meta)
		instanciate_specific_obj(cub, cub->box.objx[i], nb_meta);

	printf("Linking map instances :\n");
	i = -1;
	while (++i < nb_meta)
		link_portal_instances(cub->box.objx[i]->wobj,
			cub->box.objx[i]->rel_ref->wobj);
	printf("Linking map instances DONE\n");
		
	i = -1;
	while (++i < nb_meta)
		ft_free_p((void **)(cub->box.objx + i));
	ft_free_p((void **)&cub->box.objx);
	printf("Map instanciation DONE. WOW !\n\n\n\n");
	return (0);
}

/*
static t_oinst	*instanciate_specific_obj(t_cub *cub, t_objx *ob, int nb_meta);


static t_oinst	*find_relative(t_cub *cub, t_objx *ob, int nb_meta)
{
	t_objx	*other;
	int	i;
	
	printf("Recurcive find relative entered. Searching for rel : %c\n", ob->relativ);
	if (!ob->relativ)
		return (NULL);
	i = -1;
	while (++i < nb_meta)
	{
		other = cub->box.objx[i];
		if (other->name == ob->relativ)
		{
			if (other->wobj)
			{
				printf("Found already init obj inst\n");
				return (other->wobj);
			}
			else
			{
				printf("Found NOT init obj inst. Initializing\n");
				return (instanciate_specific_obj(cub, other, nb_meta));
			}
		}
	}
	return (NULL);	
}

static t_oinst	*instanciate_specific_obj(t_cub *cub, t_objx *ob, int nb_meta)
{
	printf("inst spec obj STARTED. obj id %d, obj name %c, ob wobj %p, type %d, alleg %d, pos (%d, %d), rel %c\n",
		ob->obj_id, ob->name, ob->wobj, ob->o_type, ob->alleg, ob->opos[0], ob->opos[1], ob->relativ);
	if (ob->obj_id < 0 || ob->wobj || ob->o_type < 1 || NB_OBJ_TYPES < ob->o_type
			|| ob->alleg < 1 || 3 < ob->alleg)
			return (NULL);

	printf("inst one Check PASSED\n");
	printf("Creating obj inst at pos (%d, %d)\n", ob->opos[0], ob->opos[1]);
	ob->opos[0] = ob->opos[0] * CELL_WIDTH + (CELL_WIDTH >> 1);
	ob->opos[1] = ob->opos[1] * CELL_WIDTH + (CELL_WIDTH >> 1);
	ob->obj_id = -1;
	ob->obj_id = create_obj_instance(cub, ob->opos, ob->o_type, ob->alleg,
		find_relative(cub, ob, nb_meta));
	ob->wobj = get_obj(cub, ob->obj_id);
	printf("MAP INSTANCIATOR CREATED OBJ : %p\n", ob->wobj);
	return (ob->wobj);
}

int	instanciate_map_objects(t_cub *cub)
{
	int	nb_meta;
	int	i;

	printf("\nStarting map instanciation \n");
	if (!cub || !cub->box.objx)
		return (-1);
	nb_meta = cub->box.meta;
	printf(" Check passed, nb_meta : %d\n", nb_meta);
	i = -1;
	while (++i < nb_meta)
		instanciate_specific_obj(cub, cub->box.objx[i], nb_meta);
	i = -1;
	while (++i < nb_meta)
		ft_free_p((void **)(cub->box.objx + i));
	ft_free_p((void **)&cub->box.objx);
	printf("Map instanciation DONE. WOW !\n\n\n\n");
	return (0);
}
*/