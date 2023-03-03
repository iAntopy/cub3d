# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 20:40:05 by iamongeo          #+#    #+#              #
#    Updated: 2023/03/03 06:54:05 by iamongeo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Ajouter tous les .c dans source ici ligne par ligne suivi d'un backslash
SRC_FLS	:=	main.c 			\
			error_handling.c 	\
			init_raycaster.c	\
			renderer.c		\
			player_controls.c

SRCS	:= $(addprefix src/, $(SRC_FLS))

OBJS	:= $(SRCS:.c=.o)

CFLAGS	:= -Wextra -Wall -Werror -ffast-math -O3

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
	@git submodule init

git_submodule: $(SUBMOD_SRC)
	@git submodule update

$(BLDGLFW):
	@cmake -S $(GLFWDIR) -B $(BLDGLFW)

$(LIBGLFW): $(BLDGLFW)
	make -C $(BLDGLFW)

$(BLDMLX): $(LIBGLFW)
	@cmake -S $(MLXDIR) -B $(BLDMLX)

$(LIBMLX): $(BLDMLX)
	make -C $(BLDMLX) -j4

$(LIBFT):
	make -C $(LFTDIR)

$(LIBMTX):
	make -C $(MTXDIR)

%.o: %.c 
	@$(CC) $(CFLAGS) $(INCL) -o $@ -c $<

#$(NAME): git_submodule $(LIBMTX) $(LIBMLX) $(LIBFT) $(OBJS)
$(NAME): git_submodule $(PROJ_LIBS) $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(INCL) -o $(NAME)

clean:
	@rm -f $(OBJS)
#	@rm -rf $(MLXDIR)/build
#	@rm -rf $(GLFWDIR)/build

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, libmlx
