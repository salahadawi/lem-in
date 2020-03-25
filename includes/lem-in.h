/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/25 23:23:44 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H
# define START 1
# define END 2

#include "libft.h"

typedef	struct		s_file
{
	char 			*line;
	struct s_file	*next;
}					t_file;

typedef	struct 		s_room
{
	char			*name;
	int				x;
	int				y;
	//int			occupied; ??
	struct s_room	*next;
	struct s_link	*links;
}					t_room;

typedef	struct		s_link
{
	t_room			*room;
	struct s_link	*next;
}					t_link;

typedef	struct		s_farm
{
	t_file			*file_start;
	t_file			*file_end;
	int				ants_amount;
	t_room			*start;
	t_room			*end;
	t_room			*first;
}					t_farm;

// maybe somekind of ant struct?
typedef	struct 		s_ant
{
	t_room			*room;
	//int			moved ?? if moved this turn
}					t_ant;

int		handle_error(char *message);

t_file	*file_new(char *line);

t_room	*new_room(void);

t_link	*new_link(t_room *room);

void	init_farm(t_farm **farm);

void	check_ants_amount(char *line);

int		check_line_comment(char *line);

int		check_line_command(char *line);

void	save_line_file(t_farm **farm, char *line);

void	save_ants_amount(t_farm **farm);

int		get_room_name(t_room **room, char *line, int *i);

int		get_room_x(t_room **room, char *line, int *i);

int		get_room_y(t_room **room, char *line, int *i);

int		get_line_room(t_room **room, char *line);

int		save_line_room(t_farm **farm, t_room **room, char *line);

int		save_command_room(t_farm **farm, t_room **room, char **line, int cmd);

char	*save_rooms(t_farm **farm);

void	check_mandatory_commands(t_farm **farm);

void	check_room_duplicates(t_farm **farm);

void	check_room_errors(t_farm **farm);

void	get_line_link(char *line, char **link1, char **link2);

int		find_first_room_by_names(t_room **room, char *name1, char *name2);

void	find_room_by_name(t_room **room, char *name);

void	link_rooms(t_room **room1, t_room **room2);

void	free_links(char *link1, char *link2);

void	save_links_to_rooms(t_farm **farm, char *line);

void	save_links(t_farm **farm, char *line);

t_farm	*save_input(void);

void	print_farm(t_farm *farm);

void	print_file(t_file *file);

void	free_file(t_file **file);

void	free_farm(t_farm **farm);

#endif