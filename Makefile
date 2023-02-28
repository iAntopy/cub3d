# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 20:40:05 by iamongeo          #+#    #+#              #
#    Updated: 2023/02/28 01:20:51 by iamongeo         ###   ########.fr        #
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

MLXDIR	:= lib/MLX42
LIBMLX	:= $(MLXDIR)/build/libmlx42.a

LFTDIR	:= lib/libft
LIBFT	:= $(LFTDIR)/libft.a


INCL	:= -I include/ -I $(LFTDIR)  -I $(MLXDIR)/include -I $(GLFWDIR)/include

LIBS	:=  $(LIBMLX) $(LIBGLFW) $(LIBFT) -ldl -pthread -lm

NAME	:= cub3D

all: $(NAME)

$(LIBGLFW):
	@cmake $(GLFWDIR) -B $(GLFWDIR)/build
	make -C $(MLXDIR)/build

$(LIBMLX): $(LIBGLFW)
	@cmake $(MLXDIR) -B $(MLXDIR)/build
	make -C $(MLXDIR)/build -j4

$(LIBFT):
	make -C $(LFTDIR)

%.o: %.c 
	@$(CC) $(CFLAGS) $(INCL) -o $@ -c $<

$(NAME): $(LIBGLFW) $(LIBMLX) $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(INCL) -o $(NAME)

clean:
	@rm -f $(OBJS)
#	@rm -f $(LIBMLX)/build

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, libmlx
