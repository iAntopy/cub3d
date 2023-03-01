# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 20:40:05 by iamongeo          #+#    #+#              #
#    Updated: 2023/03/01 05:48:48 by iamongeo         ###   ########.fr        #
                                                                              #
# **************************************************************************** #

# Ajouter tous les .c dans source ici ligne par ligne suivi d'un backslash
SRC_FLS	:=	main.c 			\
		error_handling.c 	


SRCS	:= $(addprefix src/, $(SRC_FLS))

OBJS	:= $(SRCS:.c=.o)

CFLAGS	:= -Wextra -Wall -Werror

GLFWDIR	:= lib/glfw
LIBGLFW	:= $(GLFWDIR)/build/src/libglfw3.a
BLDGLFW	:= $(GLFWDIR)/build

MLXDIR	:= lib/MLX42
LIBMLX	:= $(MLXDIR)/build/libmlx42.a
BLDMLX	:= $(MLXDIR)/build

SUBMOD_SRC := $(GLFWDIR)/src $(MLXDIR)/src 

LFTDIR	:= lib/libft
LIBFT	:= $(LFTDIR)/libft.a

MTXDIR	:= lib/mtxlib
LIBMTX	:= $(MTXDIR)/libmtx.a

INCL	:= -I include/ -I $(LFTDIR)  -I $(MLXDIR)/include -I $(GLFWDIR)/include

LIBS	:=  $(LIBMLX) $(LIBGLFW) $(LIBFT) -ldl -pthread -lm

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

$(LIBMLX):
	make -C $(MLXDIR)

%.o: %.c 
	@$(CC) $(CFLAGS) $(INCL) -o $@ -c $<

$(NAME): git_submodule $(LIBMLX) $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(INCL) -o $(NAME)

clean:
	@rm -f $(OBJS)
	@rm -rf $(MLXDIR)/build
	@rm -rf $(GLFWDIR)/build

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, libmlx
