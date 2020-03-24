/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/24 13:33:00 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		handle_error(char *message)
{
	ft_fprintf(2, "%s\n", message);
	exit(0);
}

void	check_ants_amount(char *line)
{
	int i;

	i = 0;
	while (line[i])
		if (!ft_isdigit(line[i++]))
			handle_error("Error: Ants amount is not a positive integer.");
	if (ft_atoilong(line) > 2147483647)
		handle_error("Error: The amount of ants exceeds INT_MAX.");
}

int		check_line_comment(char *line)
{
	if (line[0] == '#')
		if (!ft_strequ(line, "##start") && !ft_strequ(line, "##end"))
			return (1);
	return (0);
}

int		check_line_command(char *line)
{
	if (ft_strequ(line, "##start"))
			return (START);
	else if (ft_strequ(line, "##end"))
			return (END);
	return (0);
}

int		check_line_link(char *line)
{
	int i;

	i = 0;
	while (line[i] != '-' && line[i])
		i++;
	if (line[i++] != '-')
		return (0);
	if (!line[i])
		return (0);
	return (1);
}

int		check_line_room(char *line)
{
	int i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	if (line[i++] != ' ')
		return (0);
	if (ft_atoilong(&line[i]) > 2147483647)
		return (0);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (line[i++] != ' ')
		return (0);
	if (ft_atoilong(&line[i]) > 2147483647)
		return (0);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (!line[i])
		return (1);
	return (0);
}

void	check_file(char **file)
{
	int i;

	i = 1;
	check_ants_amount(file[0]);
	while (file[i])
	{
		if (!check_line_room(file[i]) && !check_line_command(file[i])
			&& !check_line_comment(file[i]))
			break ;
		i++;
	}
	while (file[i])
	{
		if (!check_line_link(file[i]) && !check_line_comment(file[i]))
			break ;
		i++;
	}
	if (file[i])
		handle_error("The file is invalid.");
}

t_file	*file_new(char *line)
{
	t_file *file;

	if (!(file = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Error: Malloc failed.");
	file->next = NULL;
	file->line = line;
	return (file);
}

void	free_and_go_to_next(t_file **file)
{
	t_file *prev;
	
	prev = *file;
	*file = (*file)->next;
	free(prev);
}

char	**list_to_arr(t_file *file, int size)
{
	char	**new_file;
	int		i;
	
	if (!(new_file = (char**)ft_memalloc(sizeof(char**) * (size + 1))))
		handle_error("Error: Malloc failed.");
	i = 0;
	while (file)
	{
		new_file[i++] = file->line;
		free_and_go_to_next(&file);
	}
	new_file[i] = 0;
	return (new_file);
}

void	save_ants_amount(t_farm **farm)
{
	char *line;

	if (get_next_line(0, &line) != 1)
		handle_error("The file is invalid.");
	while (check_line_comment(line))
	{
		free(line);
		if (get_next_line(0, &line) != 1)
			handle_error("The file is invalid.");
	}
	check_ants_amount(line);
	(*farm)->ants_amount = ft_atoi(line);
	free(line);
}

t_room	*new_room(void)
{
	t_room *room;

	room = (t_room*)ft_memalloc(sizeof(t_room));
	room->next = NULL;
	room->links = NULL;
	return (room);
}

int		get_line_room(t_room **room, char *line)
{
	int i;

	i = 0;
	*room = new_room();
	while (line[i] != ' ' && line[i])
		i++;
	if (line[i++] != ' ') // change to if (!line[i]) ??
		return (0);
	(*room)->name = ft_strsub(line, 0, i - 1);
	if (ft_atoilong(&line[i]) > 2147483647)
	{
		free((*room)->name);
		return (0);
	}
	(*room)->x = ft_atoi(&line[i]);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (line[i++] != ' ')
	{
		free((*room)->name);
		return (0);
	}
	if (ft_atoilong(&line[i]) > 2147483647)
	{
		free((*room)->name);
		return (0);
	}
	(*room)->y = ft_atoi(&line[i]);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (!line[i])
		return (1);
	free((*room)->name);
	return (0);
}

int		save_line_room(t_farm **farm, t_room **room, char *line)
{
	if (*room)
	{
		if (!(get_line_room(&(*room)->next, line)))
		{
			free((*room)->next); //this can be shortened to a new function
			(*room)->next = NULL; //also probably need to free the name string? can be done in get_line_room actually
			return (0);
		}
		*room = (*room)->next;
	}
	else
	{
		if (!(get_line_room(room, line)))
		{
			free(*room); //this can be shortened to a new function
			*room = NULL;
			return (0);
		}
	}
	if (!(*farm)->first)
		(*farm)->first = *room;
	return (1);
}

int		save_command_room(t_farm **farm, t_room **room, char **line, int cmd)
{
	free(*line);
	get_next_line(0, line);
	if (*room)
	{
		if (!(get_line_room(&(*room)->next, *line)))
			handle_error("Error: Command not followed by valid room.");
		*room = (*room)->next;
	}
	else
		if (!(get_line_room(room, *line)))
			handle_error("Error: Command not followed by valid room.");
	if (!(*farm)->first)
		(*farm)->first = *room;
	if (cmd == START)
		(*farm)->start = *room;
	else if (cmd == END)
		(*farm)->end = *room;
	return (1);
}

char	*save_rooms(t_farm **farm)
{
	char	*line;
	t_room	*room;
	int		command;

	room = NULL;
	while (get_next_line(0, &line) > 0)
	{
		if (!check_line_comment(line))
		{
			
			if ((command = check_line_command(line)) > 0)
				save_command_room(farm, &room, &line, command);
			else
				if (!save_line_room(farm, &room, line))
					return (line);
		}
		free(line);
	}
	handle_error("Error: No links found in file.");
	return (NULL);
}

void	init_farm(t_farm **farm)
{
	(*farm) = (t_farm*)ft_memalloc(sizeof(t_farm));
	(*farm)->start = NULL;
	(*farm)->end = NULL;
	(*farm)->first = NULL;
}

// currently checks 4000 rooms in around 0,09 seconds, merge sort and check for
// duplicates might be significantly faster
void	check_room_duplicates(t_farm **farm)
{
	t_room *room;
	t_room *tmp;

	room = ((*farm)->first);
	while (room)
	{
		tmp = room->next;
		while (tmp)
		{
			if (ft_strequ(room->name, tmp->name))
				handle_error("Error: File contains duplicate rooms.");
			tmp = tmp->next;
		}
		room = room->next;
	}
}

t_farm	*save_input(void)
{
	t_farm	*farm;
	char	*line;
	
	(void)line; //temp
	init_farm(&farm);
	save_ants_amount(&farm); // add functions here
	line = save_rooms(&farm);
	check_room_duplicates(&farm);
	//free(line); // tmp
	//save_links(&farm, line); // save links should continue with same input line that failed in save rooms
	return (farm);
}

void	print_farm(t_farm *farm)
{
	t_room *room;

	room = farm->first;
	ft_printf("%d\n", farm->ants_amount);
	while (room)
	{
		if (farm->start == room)
			ft_printf("#start\n");
		if (farm->end == room)
			ft_printf("#end\n");
		ft_printf("%s %d %d\n", room->name, room->x, room->y);
		room = room->next;
	}
}

void	free_farm(t_farm **farm)
{
	t_room *tmp;
	
	while ((*farm)->first)
	{
		tmp = (*farm)->first->next;
		free((*farm)->first->name);
		free((*farm)->first);
		(*farm)->first = tmp;
	}
	free(*farm);
}

int	main(void)
{
	t_farm *farm;
	
	farm = save_input();
	//check_file(file);
	print_farm(farm);
	free_farm(&farm);
	return (0);
}

//Remember to free file after everything
//Maybe should learn more about optimal file reading and parsing?