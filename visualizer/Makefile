# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/02 18:52:09 by sadawi            #+#    #+#              #
#    Updated: 2020/08/03 17:08:34 by sadawi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf ../libft/libft.a \
-L ~/.brew/Cellar/sdl2/2.0.12_1/lib \
-L ~/.brew/Cellar/sdl2_image/2.0.5/lib \
-L ~/.brew/Cellar/sdl2_ttf/2.0.15/lib \
-I ~/.brew/Cellar/sdl2/2.0.12_1/include \
-I ~/.brew/Cellar/sdl2_image/2.0.5/include \
-I ~/.brew/Cellar/sdl2_ttf/2.0.15/include
NAME = visualizer
FLAGS = -Wall -Werror -Wextra -O3

all:
	gcc $(SRCS) $(FLAGS) $(LIBS) -o $(NAME)

noflags:
	gcc $(SRCS) $(LIBS) -o $(NAME)

fclean:
	@/bin/rm -f $(NAME)
	@echo Clean successful!
