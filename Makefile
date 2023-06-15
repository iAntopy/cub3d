# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 20:40:05 by iamongeo          #+#    #+#              #
#    Updated: 2023/06/14 20:27:03 by gehebert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MSRC_FLS	:=		main.c 			\
					event_handlers.c	\
					error_handling.c 	\
					init_raycaster.c	\
					update_raycaster.c	\
					renderer.c			\
					renderer_utils.c	\
					player_controls.c	\
					prebuilt_map_tools.c	\
					t_color_shift.c		\
					t_color_creat.c 	\
					t_map_parse.c 		\
					t_map_tool.c 		\
					t_wall_chk.c		\
					t_empty_line.c		\
					tex_parse.c

# Ajouter tous les .c dans source ici ligne par ligne suivi d'un backslash
BSRC_FLS	:=		main.c 			\
				event_handlers.c	\
				event_handlers2.c	\
				error_handling.c 	\
				error_handling2.c 	\
				init_raycaster.c	\
				floor_caster.c		\
				sky_caster.c		\
				update_raycaster.c	\
				renderer.c			\
				renderer_draw_walls.c	\
				renderer_draw_sky.c		\
				renderer_draw_floor.c	\
				renderer_draw_floor_proj.c	\
				renderer_draw_walls_proj.c	\
				renderer_draw_objs.c	\
				renderer_draw_objs_proj.c	\
				renderer_draw_objs_utils.c	\
				renderer_utils.c	\
				player_controls.c	\
				prebuilt_map_tools.c	\
				t_color_shift.c		\
				t_color_creat.c 	\
				t_map_parse.c 		\
				t_minimap2.c		\
				t_map_tool.c 		\
				t_ref_build.c 		\
				t_obj_model.c 		\
				t_obj_model_extend.c	\
				t_wall_chk.c		\
				t_empty_line.c		\
				tex_parse.c			\
				t_ref_name.c		\
				t_ref_extend.c		\
				map_instanciator.c	\
				map_instanciator_utils.c	\
				draw_threads.c		\
				object_framework_manager.c		\
				object_deleters_and_getter.c	\
				object_controls.c	\
				object_controls2.c	\
				object_activators.c	\
				object_actions.c	\
				object_actions2.c	\
				object_linkers.c	\
				object_instanciators.c	\
				object_instanciators2.c	\
				spawnpoint_funcs.c		\
				simple_vector_funcs.c	\
				object_utils.c			\
				collision_managment.c


MSRCS	:= $(addprefix m_src/, $(MSRC_FLS))
BSRCS	:= $(addprefix b_src/, $(BSRC_FLS))

MOBJS	:= $(MSRCS:.c=.o)
BOBJS	:= $(BSRCS:.c=.o)
# CFLAGS	:= -Wextra -Wall -Werror -pthread  -g -O1 -fsanitize=leak
CFLAGS	:= -Wextra -Wall -Werror -pthread -g 
# CFLAGS	:= -Wextra -Wall -Werror -pthread -ffast-math -O3

LDFLAGS	:= -fsanitize=address 


#CC		:= clang
CC		:= gcc

GLFWDIR	:= lib/glfw
LIBGLFW	:= $(GLFWDIR)/build/src/libglfw3.a
BLDGLFW	:= $(GLFWDIR)/build

MLXDIR	:= lib/MLX42
LIBMLX	:= $(MLXDIR)/build/libmlx42.a
BLDMLX	:= $(MLXDIR)/build

LFTDIR	:= lib/libft
LIBFT	:= $(LFTDIR)/libft.a

MTXDIR	:= lib/mtxlib
LIBMTX	:= $(MTXDIR)/libmtx.a

SUBMOD_SRC := $(GLFWDIR)/src $(MLXDIR)/src $(LFTDIR)/libft.h $(MTXDIR)/src
EXT_INCL := -I $(MLXDIR)/include -I $(GLFWDIR)/include 

INCL	:= -I $(LFTDIR) -I $(MTXDIR)/includes $(EXT_INCL)

$(MOBJS):	SPEC_INCL := -I m_include/
$(BOBJS):	SPEC_INCL := -I b_include/

BASE_LIBS := -ldl -pthread -lm
PROJ_LIBS := $(LIBMTX) $(LIBMLX) $(LIBGLFW) $(LIBFT)
LIBS	:= $(PROJ_LIBS) $(BASE_LIBS)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
        LIBS += -framework OpenGL -framework Cocoa -framework IOKit
endif

NAME	:= cub3D

all: $(NAME)

$(SUBMOD_SRC):
	@echo "Submodule init"
	git submodule init
	git submodule update --remote --merge

$(BLDGLFW):
	cmake -S $(GLFWDIR) -B $(BLDGLFW)

$(LIBGLFW): $(BLDGLFW)
	@echo "make glfw"
	make -C $(BLDGLFW)

$(BLDMLX): $(LIBGLFW)
	cmake -S $(MLXDIR) -B $(BLDMLX)

$(LIBMLX): $(BLDMLX)
	make -C $(BLDMLX) -j4

$(LIBFT):
	make -C $(LFTDIR)

$(LIBMTX):
	make -C $(MTXDIR)

%.o: %.c 
	$(CC) $(CFLAGS) $(INCL) $(SPEC_INCL) -o $@ -c $<

# $(MOBJS) : $(MSRCS)
# 	$(CC) $(CFLAGS) $(MINCL) -c $< -o $@
	
# $(BOBJS) : $(BSRCS)
# 	$(CC) $(CFLAGS) $(BINCL) -c $< -o $@
	
#$(NAME): git_submodule $(LIBMTX) $(LIBMLX) $(LIBFT) $(OBJS)
$(NAME): $(SUBMOD_SRC) $(PROJ_LIBS) $(MOBJS)
	echo $(MOBJS)
	@echo "Linking executable"
	$(CC) $(MOBJS) $(LIBS) $(LDFLAGS) $(INCL) $(MINCL) -o $(NAME)

bonus: $(SUBMOD_SRC) $(PROJ_LIBS) $(BOBJS)
	@echo "Linking executable"
	$(CC) $(BOBJS) $(LIBS) $(LDFLAGS) $(INCL) $(BINCL) -o $(NAME)

clean:
	@rm -f $(MOBJS) $(BOBJS)
#	@rm -rf $(MLXDIR)/build
#	@rm -rf $(GLFWDIR)/build

fclean: clean
	@rm -f $(NAME)

re: fclean bonus

.PHONY: all, clean, fclean, re, libmlx

