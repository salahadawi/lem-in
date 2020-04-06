/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_rooms.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/29 13:01:18 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/04 19:07:46 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

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


//can probably change return value to void?
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
