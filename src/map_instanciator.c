/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_instanciator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 06:25:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/05/29 23:47:27 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/*
typedef struct s_objx
{
	char 			name;		// '#' 
	int				obj_id;		 // enrg. id	
	int				o_type;		// model_type
	int 			alleg;		// allegence _txtr
	int 			opos[2];	// relativ pos (reltv. obj_id)
	char			relativ;	// char obj_id
	struct s_objx	*rel_ref;	 // ptr to its relative's objx ptr;
	t_oinst			*wobj;		// world object instance
}	t_objx;
*/
static void	print_all_map_insts(t_objx **ob, int nb_meta)
{
	int	i;

	printf("\n*____________ (objx insts START) ____________*\n");
	i = -1;
	while (++i < nb_meta)
	{
		printf("| %d - %c - type %d - pos (%d, %d) - rel %c - wobj %p\n",
			ob[i]->obj_id, ob[i]->name, ob[i]->o_type, ob[i]->opos[0], ob[i]->opos[1],
			ob[i]->relativ, ob[i]->wobj);
	}
	printf("*____________ (objx insts END) ____________*\n\n");
}


static t_objx	*find_relative(t_cub *cub, t_objx *ob, int nb_meta)
{
	t_objx	*other;
	int		i;
	
	printf("Find relative entered. Searching for rel : %c\n", ob->relativ);
	if (!ob->relativ)
		return (NULL);
	i = -1;
	while (++i < nb_meta)
	{
		other = cub->box.objx[i];
		if (other->name == ob->relativ)
		{
			printf("found relative of %c (%d) to %c (%d)\n",
				ob->name, ob->obj_id, other->name, other->obj_id);
			return (other);
		}
	}
	return (NULL);	
}

static int	link_all_map_instances(t_objx **ob, int nb_meta)
{
	t_objx	*o;
	int		i;

	i = -1;
	while (++i < nb_meta)
	{
		o = ob[i];
		printf("looking at obj : %d %c\n", o->o_type, o->name);
		printf("| %d - %c - type %d - pos (%d, %d) - rel %c - wobj %p\n",
			o->obj_id, o->name, o->o_type, o->opos[0], o->opos[1],
			o->relativ, o->wobj);
		if (o->o_type == OBJ_PORTAL)
		{
			// printf("linking portal - name %c, type %d to name %c, type %d\n",6
			link_portal_instances(o->wobj, o->rel_ref->wobj);
		}
		else if (o->o_type == OBJ_LEVER)
		{
			printf("linking lever objx : %p -> rel : %p\n", o, o->rel_ref);
//			printf("linking lever to portal - name %c, type %d to name %c, type %d, wobj enum : %d, rel wobj enum : %d\n",
//				o->name, o->o_type, o->rel_ref->name, o->rel_ref->o_type,
//					o->wobj->type->type_enum, o->rel_ref->wobj->type->type_enum);
			link_lever_to_portal(o->wobj, o->rel_ref->wobj);
			printf("lever relative ptr : %p\n", o->wobj->relative);
		}
//		else if (ob->o_type == OBJ_FIREBALL)
//			link_fireball_to_player(o->wobj, (t_hero *)o->rel_ref->wobj);
//		else if (ob->o_type == OBJ_FIREBALL)
//			link_firepit_to_player(o->wobj, (t_hero *)o->rel_ref->wobj);
	}
	return (0);
}

static t_oinst	*instanciate_specific_obj(t_cub *cub, t_objx *ob, int nb_meta)
{
	float	pos[2];
	int		inst_id;
	
	printf("instanciate_specific_obj START \n");
	printf("Try creating obj inst at pos (%d, %d), name %c, type %d, alleg %d\n",
		ob->opos[0], ob->opos[1], ob->name, ob->o_type, ob->alleg);
	if (ob->obj_id < 0 || ob->o_type < 1 || NB_OBJ_TYPES < ob->o_type
			|| ob->alleg < 1 || 3 < ob->alleg)
			return (NULL);
	
	pos[0] = (float)ob->opos[0] * CELL_WIDTH + (CELL_WIDTH >> 1);
	pos[1] = (float)ob->opos[1] * CELL_WIDTH + (CELL_WIDTH >> 1);
	printf("inst one Check PASSED\n");
	printf("\nCreating obj inst at pos (%d, %d), name %c, type %d\n",
		ob->opos[0], ob->opos[1], ob->name, ob->o_type);
	if (ob->o_type == OBJ_PLAYER && cub->nb_players < MAX_PLAYERS)
	{
		inst_id = create_obj_instance(cub, pos, OBJ_SPAWNPOINT, ob->alleg, cub);
		inst_id = spawn_new_player(get_obj(cub, inst_id), 0);
		//inst_id = create_obj_instance(cub, pos, ob->o_type, ob->alleg, &cub->hero);
	}
	else
		inst_id = create_obj_instance(cub, pos, ob->o_type, ob->alleg, NULL);

	ob->wobj = get_obj(cub, inst_id);
	printf("Created obj id %d, ptr %p inst with type enum : %d\n", ob->wobj->_id, ob->wobj, ob->wobj->type->type_enum);
	printf("MAP INSTANCIATOR CREATED OBJ : %p\n", ob->wobj);
	ob->rel_ref = find_relative(cub, ob, nb_meta);
	return (ob->wobj);
}

int	instanciate_map_objects(t_cub *cub)
{
	int	nb_meta;
	int	i;

	printf("\nStarting map instanciation. cub->objs.lever.type_enum : %d \n", cub->objs.lever.type_enum);
	if (!cub || !cub->box.objx)
		return (-1);
	nb_meta = cub->box.meta;

	printf("objx before : meta = %d\n", nb_meta);
	print_all_map_insts(cub->box.objx, nb_meta);
	printf(" Check passed, nb_meta : %d\n", nb_meta);
	i = -1;
	while (++i < nb_meta)
	{
		printf("info i %d , cub->box.objx[i] %p\n", i, cub->box.objx[i] );
		instanciate_specific_obj(cub, cub->box.objx[i], nb_meta);
	}

	printf("\nLinking map instances :\n");
	link_all_map_instances(cub->box.objx, nb_meta);
	printf("Linking map instances DONE\n\n");

	printf("objx after : \n");
	print_all_map_insts(cub->box.objx, nb_meta);
		
	i = -1;
	while (++i < nb_meta)
		ft_free_p((void **)(cub->box.objx + i));
	ft_free_p((void **)&cub->box.objx);
	printf("Map instanciation DONE. WOW !\n\n\n\n");
	return (0);
}
