/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/25 21:26:44 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

void	check_ants_amount(char *line)
{
	int i;

	i = 0;
	if (!ft_atoi(line))
		handle_error("Ants amount is not a positive integer.");
	while (line[i])
		if (!ft_isdigit(line[i++]))
			handle_error("Ants amount is not a positive integer.");
	if (ft_atoilong(line) > 2147483647)
		handle_error("The amount of ants exceeds INT_MAX.");
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

void	save_line_file(t_farm **farm, char *line)
{
	t_file	**file;

	file = &(*farm)->file_end;
	if (*file)
	{
		(*file)->next = file_new(line);
		*file = (*file)->next;
	}
	else
	{
		*file = file_new(line);
		(*farm)->file_start = *file;
	}
}

void	save_ants_amount(t_farm **farm)
{
	char *line;

	if (get_next_line(0, &line) != 1)
		handle_error("The file is invalid.");
	while (check_line_comment(line))
	{
		save_line_file(farm, line);
		if (get_next_line(0, &line) != 1)
			handle_error("The file is invalid.");
	}
	check_ants_amount(line);
	(*farm)->ants_amount = ft_atoi(line);
	save_line_file(farm, line);
}

int		get_room_name(t_room **room, char *line, int *i)
{
	while (line[*i] != ' ' && line[*i])
		(*i)++;
	if (line[(*i)++] != ' ') // change to if (!line[i]) ??
		return (0);
	(*room)->name = ft_strsub(line, 0, *i - 1);
	return (1);
}

int		get_room_x(t_room **room, char *line, int *i)
{
	if (ft_atoilong(&line[*i]) > 2147483647)
	{
		free((*room)->name);
		return (0);
	}
	(*room)->x = ft_atoi(&line[*i]);
	if (line[*i] == '-')
		(*i)++;
	while (ft_isdigit(line[*i]))
		(*i)++;
	if (line[(*i)++] != ' ')
	{
		free((*room)->name);
		return (0);
	}
	return (1);
}

int		get_room_y(t_room **room, char *line, int *i)
{
	if (ft_atoilong(&line[*i]) > 2147483647)
	{
		free((*room)->name);
		return (0);
	}
	(*room)->y = ft_atoi(&line[*i]);
	if (line[*i] == '-')
		(*i)++;
	while (ft_isdigit(line[*i]))
		(*i)++;
	if (!line[*i])
		return (1);
	free((*room)->name);
	return (0);
}


int		get_line_room(t_room **room, char *line)
{
	int i;

	i = 0;
	*room = new_room();
	if (!(get_room_name(room, line, &i)))
		return (0);
	if (!(get_room_x(room, line, &i)))
		return (0);
	if (!(get_room_y(room, line, &i)))
		return (0);
	return (1);
}

int		save_line_room(t_farm **farm, t_room **room, char *line)
{
	if (*room)
	{
		if (!(get_line_room(&(*room)->next, line)))
		{
			free((*room)->next);
			(*room)->next = NULL;
			return (0);
		}
		*room = (*room)->next;
	}
	else
	{
		if (!(get_line_room(room, line)))
		{
			free(*room);
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
	save_line_file(farm, *line);
	get_next_line(0, line);
	if (*room)
	{
		if (!(get_line_room(&(*room)->next, *line)))
			handle_error("Command not followed by valid room.");
		*room = (*room)->next;
	}
	else
		if (!(get_line_room(room, *line)))
			handle_error("Command not followed by valid room.");
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
		save_line_file(farm, line);
	}
	handle_error("No links found in file.");
	return (NULL);
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
				handle_error("File contains duplicate rooms.");
			tmp = tmp->next;
		}
		room = room->next;
	}
}

void	check_mandatory_commands(t_farm **farm)
{
	if (!(*farm)->start)
		handle_error("Start room has not been specified.");
	if (!(*farm)->end)
		handle_error("End room has not been specified.");
}

void	check_room_errors(t_farm **farm)
{
	check_room_duplicates(farm);
	check_mandatory_commands(farm);
}

void	get_line_link(char *line, char **link1, char **link2)
{
	int i;

	i = 0;
	while (line[i] != '-' && line[i])
		i++;
	*link1 = ft_strsub(line, 0, i);
	if (line[i++] != '-')
		handle_error("File contains invalid link.");
	if (!line[i])
		handle_error("File contains invalid link.");
	*link2 = ft_strsub(line, i, ft_strlen(&line[i]));
}

int		find_first_room_by_names(t_room **room, char *name1, char *name2)
{
	while (!ft_strequ((*room)->name, name1) && !ft_strequ((*room)->name, name2))
	{
		*room = (*room)->next;
		if (!*room)
			handle_error("Room specified in links not found.");
	}
	if (ft_strequ((*room)->name, name1))
		return (1);
	else
		return (2);	
}

void	find_room_by_name(t_room **room, char *name)
{
	if (ft_strequ((*room)->name, name))
		handle_error("Links contain a room linking to itself.");
	while (!ft_strequ((*room)->name, name))
	{
		*room = (*room)->next;
		if (!*room)
			handle_error("Room specified in links not found.");
	}
}

void	link_rooms(t_room **room1, t_room **room2)
{
	t_link *link1;
	t_link *link2;

	link1 = (*room1)->links;
	link2 = (*room2)->links;
	if (!link1)
		link1 = new_link(*room2);
	else
	{
		while (link1->next)
			link1 = link1->next;
		link1->next = new_link(*room2);
	}
	if (!link2)
		link2 = new_link(*room1);
	else
	{
		while (link2->next) // function to cycle link until end
			link2 = link2->next;
		link2->next = new_link(*room1);
	}
	if (!(*room1)->links)
		(*room1)->links = link1;
	if (!(*room2)->links)
		(*room2)->links = link2;
}

void	free_links(char *link1, char *link2)
{
	free(link1);
	free(link2);
}

void	save_links_to_rooms(t_farm **farm, char *line)
{
	t_room	*room1;
	t_room	*room2;
	int		first_link;
	char	*name1;
	char	*name2;

	get_line_link(line, &name1, &name2);
	room1 = (*farm)->first;
	first_link = find_first_room_by_names(&room1, name1, name2);
	room2 = room1;
	if (first_link == 1)
		find_room_by_name(&room2, name2);
	else
		find_room_by_name(&room2, name1);
	link_rooms(&room1, &room2);
	free_links(name1, name2);
}

void	save_links(t_farm **farm, char *line)
{
	save_links_to_rooms(farm, line);
	save_line_file(farm, line);
	while (get_next_line(0, &line))
	{
		if (!check_line_comment(line))
			save_links_to_rooms(farm, line);
		save_line_file(farm, line);
	}
}

t_farm	*save_input(void)
{
	t_farm	*farm;
	char	*line;

	(void)line; //temp
	init_farm(&farm);
	save_ants_amount(&farm);
	line = save_rooms(&farm);
	check_room_errors(&farm);
	save_links(&farm, line);
	return (farm);
}

void	print_farm(t_farm *farm)
{
	t_room *room;
	t_link *link;

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
	room = farm->first;
	while (room)
	{
		link = room->links;
		while (link)
		{
			ft_printf("%s-%s\n", room->name, link->room->name);
			link = link->next;
		}
		room = room->next;
	}
}

void	print_file(t_file *file)
{
	while (file)
	{
		ft_printf("%s\n", file->line);
		file = file->next;
	}
}

void	free_file(t_file **file)
{
	t_file *tmp;

	while (*file)
	{
		tmp = (*file)->next;
		free((*file)->line);
		free((*file));
		*file = tmp;
	}
}

void	free_farm(t_farm **farm)
{
	t_room *tmp_room;
	t_link *tmp_link;

	free_file(&(*farm)->file_start);
	while ((*farm)->first)
	{
		tmp_room = (*farm)->first->next;
		while ((*farm)->first->links)
		{
			tmp_link = (*farm)->first->links->next;
			free ((*farm)->first->links);
			(*farm)->first->links = tmp_link;
		}
		free((*farm)->first->name);
		free((*farm)->first);
		(*farm)->first = tmp_room;
	}
	free(*farm);
}

int	main(void)
{
	t_farm *farm;

	farm = save_input();
	print_farm(farm);
	//print_file(farm->file_start);
	free_farm(&farm);
	return (0);
}

//Remember to free file after everything
//Maybe should learn more about optimal file reading and parsing?