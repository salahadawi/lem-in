/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/04 18:35:34 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "../libft/includes/libft.h"

# define START		1
# define END		2
# define MAX_INT	2147483647

extern char			*g_flags;

typedef	struct		s_file
{
	char			*line;
	struct s_file	*next;
}					t_file;

typedef	struct		s_room
{
	char			*name;
	int				x;
	int				y;
	int				occupied;
	int				weight;
	struct s_room	*next;
	struct s_link	*links;
	struct s_room	*parent;
	int				parent_num;
	int				flow;
}					t_room;

typedef	struct		s_link
{
	t_room			*room;
	int				flow;
	int				weight;
	struct s_link	*next;
}					t_link;

typedef	struct		s_ant
{
	int				number;
	t_room			*room;
	t_link			*path;
	struct s_ant	*next;
}					t_ant;

typedef struct		s_path
{
	char			*id;
	int				size;
	int				ants_amount;
	t_link			*path;
	struct s_path	*next;
}					t_path;

typedef	struct		s_farm
{
	t_file			*file_start;
	t_file			*file_end;
	int				ants_amount;
	int				start_ants_amount;
	t_room			*start;
	t_room			*end;
	t_room			*first;
	t_path			*paths;
	int				paths_amount;
	int				(*alg)(t_link*, t_ant**, struct s_farm*);
	int				rooms_amount;
}					t_farm;

int					handle_error(char *message);

t_file				*file_new(char *line);

t_room				*new_room(void);

t_link				*new_link(t_room *room);

void				init_farm(t_farm **farm);

void				check_ants_amount(char *line);

int					check_line_comment(char *line);

int					check_line_command(char *line);

void				save_line_file(t_farm **farm, char *line);

void				save_ants_amount(t_farm **farm);

int					get_room_name(t_room **room, char *line, int *i);

int					get_room_x(t_room **room, char *line, int *i);

int					get_room_y(t_room **room, char *line, int *i);

int					get_line_room(t_room **room, char *line);

int					save_line_room(t_farm **farm, t_room **room, char *line);

void				save_command_room(t_farm **farm, t_room **room, char **line,
int cmd);

char				*save_rooms(t_farm **farm);

void				check_mandatory_commands(t_farm **farm);

void				check_room_duplicates(t_farm **farm);

void				check_room_errors(t_farm **farm);

void				get_line_link(char *line, char **link1, char **link2);

int					find_first_room_by_names(t_room **room, char *name1,
char *name2);

void				find_room_by_name(t_room **room, char *name);

void				link_rooms(t_room **room1, t_room **room2);

void				free_links(char *link1, char *link2);

void				save_links_to_rooms(t_farm **farm, char *line);

void				save_links(t_farm **farm, char *line);

t_farm				*save_input(void);

void				print_farm(t_farm *farm);

void				print_file(t_file *file);

void				free_file(t_file **file);

void				free_farm(t_farm **farm);

void				enqueue(t_link **queue, t_room *new_room);

t_room				*dequeue(t_link **queue);

void				init_queue(t_link **queue, t_link **visited, t_farm **farm);

void				create_ants(t_farm *farm, t_ant **first);

void				check_links_valid(t_farm **farm);

void				move_ants(t_farm **farm);

int					count_ants_in_line(t_ant *ant, t_farm *farm);

t_ant				*new_ant(t_farm *farm, int *ant_number);

void				remove_ant(t_ant **first, t_ant **ants);

int					room_in_links(t_room *room, t_link *link);

void				find_weights(t_farm **farm, int weight, t_link *queue);

int					free_queue(t_link *queue);

void				save_paths(t_farm **farm);

t_link				*assign_ant_path(t_farm *farm);

void				mergesort_paths(t_path **paths);

void				update_queue(t_link **queue, t_link *cur, t_link *links,
						int num);

int					free_two_queues(t_link *queue1, t_link *queue2);

int					handle_end_found(t_farm *farm, t_room *neighbor,
						t_link *path, int n);

t_path				*create_path(t_link *path, int size);

int					adjust_flow(t_room *room1, t_room *room2);

void				get_flow_paths2(t_farm **farm);

void				free_path(t_path *path);

void				free_paths(t_farm **farm);

#endif
