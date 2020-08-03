# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/02 18:52:09 by sadawi            #+#    #+#              #
#    Updated: 2020/08/03 13:56:53 by sadawi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf ../libft/libft.a -L lib
NAME = visualizer
FLAGS = -Wall -Werror -Wextra

all:
	gcc $(SRCS) $(FLAGS) $(LIBS) -o $(NAME)

noflags:
	gcc $(SRCS) $(LIBS) -o $(NAME)

fclean:
	@/bin/rm -f $(NAME)
	@echo Clean successful!
