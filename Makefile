# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 20:40:05 by iamongeo          #+#    #+#              #
#    Updated: 2023/06/09 04:06:00 by iamongeo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# Ajouter tous les .c dans source ici ligne par ligne suivi d'un backslash
SRC_FLS	:=		main.c 			\
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


SRCS	:= $(addprefix src/, $(SRC_FLS))

OBJS	:= $(SRCS:.c=.o)
#CFLAGS	:= -Wextra -Wall -Werror -pthread -g -fsanitize=address
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
INCL	:= -I include/ -I $(LFTDIR)  -I $(MLXDIR)/include -I $(GLFWDIR)/include -I $(MTXDIR)/includes

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
	$(CC) $(CFLAGS) $(INCL) -o $@ -c $<

#$(NAME): git_submodule $(LIBMTX) $(LIBMLX) $(LIBFT) $(OBJS)
$(NAME): $(SUBMOD_SRC) $(PROJ_LIBS) $(OBJS)
	@echo "Linking executable"
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) $(INCL) -o $(NAME)

clean:
	@rm -f $(OBJS)
#	@rm -rf $(MLXDIR)/build
#	@rm -rf $(GLFWDIR)/build

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, libmlx

# #Ajouter tous les .c dans source ici ligne par ligne suivi d'un backslash
# SRC_FLS	:=		main.c 			\
# 			event_handlers.c	\
# 			error_handling.c 	\
# 			init_raycaster.c	\
# 			update_raycaster.c	\
# 			renderer.c			\
# 			renderer_utils.c	\
# 			player_controls.c	\
# 			prebuilt_map_tools.c	\
# 			t_color_shift.c		\
# 			t_color_creat.c 	\
# 			t_map_parse.c 		\
# 			t_map_tool.c 		\
# 			t_wall_chk.c		\
# 			t_empty_line.c		\
# 			tex_parse.c

# SRCS	:= $(addprefix src/, $(SRC_FLS))

# OBJS	:= $(SRCS:.c=.o)
# #CFLAGS	:= -Wextra -Wall -Werror  -g -fsanitize=address# -ffast-math -O3
# CFLAGS	:= -Wextra -Wall -Werror -g
# #CFLAGS	:= -Wextra -Wall -Werror -ffast-math -O3

# #LDFLAGS	:= -fsanitize=address

# #CC		:= clang
# CC		:= gcc

# GLFWDIR	:= lib/glfw
# LIBGLFW	:= $(GLFWDIR)/build/src/libglfw3.a
# BLDGLFW	:= $(GLFWDIR)/build

# MLXDIR	:= lib/MLX42
# LIBMLX	:= $(MLXDIR)/build/libmlx42.a
# BLDMLX	:= $(MLXDIR)/build

# LFTDIR	:= lib/libft
# LIBFT	:= $(LFTDIR)/libft.a

# MTXDIR	:= lib/mtxlib
# LIBMTX	:= $(MTXDIR)/libmtx.a

# SUBMOD_SRC := $(GLFWDIR)/src $(MLXDIR)/src $(LFTDIR)/libft.h $(MTXDIR)/src
# INCL	:= -I include/ -I $(LFTDIR)  -I $(MLXDIR)/include -I $(GLFWDIR)/include -I $(MTXDIR)/includes

# BASE_LIBS := -ldl -pthread -lm
# PROJ_LIBS := $(LIBMTX) $(LIBMLX) $(LIBGLFW) $(LIBFT)
# LIBS	:= $(PROJ_LIBS) $(BASE_LIBS)

# UNAME_S := $(shell uname -s)
# ifeq ($(UNAME_S), Darwin)
#         LIBS += -framework OpenGL -framework Cocoa -framework IOKit
# endif

# NAME	:= cub3D

# all: $(NAME)

# $(SUBMOD_SRC):
# 	@echo "Submodule init"
# 	git submodule init
# 	git submodule update --remote --merge

# $(BLDGLFW):
# 	cmake -S $(GLFWDIR) -B $(BLDGLFW)

# $(LIBGLFW): $(BLDGLFW)
# 	@echo "make glfw"
# 	make -C $(BLDGLFW)

# $(BLDMLX): $(LIBGLFW)
# 	cmake -S $(MLXDIR) -B $(BLDMLX)

# $(LIBMLX): $(BLDMLX)
# 	make -C $(BLDMLX) -j4

# $(LIBFT):
# 	make -C $(LFTDIR)

# $(LIBMTX):
# 	make -C $(MTXDIR)

# %.o: %.c 
# 	$(CC) $(CFLAGS) $(INCL) -o $@ -c $<

# #$(NAME): git_submodule $(LIBMTX) $(LIBMLX) $(LIBFT) $(OBJS)
# $(NAME): $(SUBMOD_SRC) $(PROJ_LIBS) $(OBJS)
# 	@echo "Linking executable"
# 	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) $(INCL) -o $(NAME)

# clean:
# 	@rm -f $(OBJS)
# #	@rm -rf $(MLXDIR)/build
# #	@rm -rf $(GLFWDIR)/build

# fclean: clean
# 	@rm -f $(NAME)

# re: clean all

# .PHONY: all, clean, fclean, re, libmlx
