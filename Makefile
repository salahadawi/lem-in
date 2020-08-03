# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/10 19:26:38 by sadawi            #+#    #+#              #
#    Updated: 2020/08/03 16:12:30 by sadawi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in
CFILES = check_line.c find_room.c get_room_variables.c move_ants.c save_ants.c \
save_rooms.c check_room_errors.c find_weights.c  handle_error.c print.c \
save_input.c create_struct.c free_memory.c main.c queue.c save_links.c \
save_paths.c mergesort.c path_helpers.c
SRCS = $(addprefix srcs/, $(CFILES))
OBJS = $(addprefix objs/, $(notdir $(SRCS:.c=.o)))
INCLUDES = -I includes -I libft/includes
FLAGS = -Wall -Wextra -Werror -O3
RUN_LIB = make -C libft/ fclean && make -C libft/

all: $(NAME)

$(NAME):
	@$(RUN_LIB)
	@rm -rf objs
	@echo Compiling $(NAME)...
	@gcc $(FLAGS) $(INCLUDES) -c $(SRCS)
	@mkdir objs
	@mv $(notdir $(SRCS:.c=.o)) objs
	@gcc $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJS) libft/libft.a
	@echo $(NAME) compiled succesfully!

clean:
	@/bin/rm -f $(OBJS)
	@rm -rf objs
	@make -C libft/ clean
	@echo Clean successful!

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C libft/ fclean
	@echo Clean successful!

re: fclean all
