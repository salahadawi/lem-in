/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 20:58:02 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/07 19:10:21 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

t_file	*file_new(char *line)
{
	t_file *file;

	if (!(file = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Malloc failed.");
	file->line = line;
	file->next = NULL;
	return (file);
}

t_room	*new_room(void)
{
	t_room *room;

	room = (t_room*)ft_memalloc(sizeof(t_room));
	room->next = NULL;
	room->links = NULL;
	room->weight = MAX_INT;
	room->occupied = 0;
	return (room);
}

t_link	*new_link(t_room *room)
{
	t_link *link;

	if (!(link = (t_link*)ft_memalloc(sizeof(t_link))))
		handle_error("Malloc failed.");
	link->room = room;
	link->next = NULL;
	return (link);
}

void	init_farm(t_farm **farm)
{
	(*farm) = (t_farm*)ft_memalloc(sizeof(t_farm));
	(*farm)->file_start = NULL;
	(*farm)->file_end = NULL;
	(*farm)->start = NULL;
	(*farm)->end = NULL;
	(*farm)->first = NULL;
	if (ft_strchr(g_flags, '1'))
		(*farm)->alg = optimal_room1;
	else
		(*farm)->alg = optimal_room;
}

t_ant	*new_ant(t_farm *farm, int *ant_number)
{
	t_ant *ant;

	ant = (t_ant*)ft_memalloc(sizeof(t_ant));
	ant->room = farm->start;
	ant->number = (*ant_number)++;
	return (ant);
}
