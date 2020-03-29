/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 22:54:44 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/29 18:09:13 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

void	enqueue(t_link **queue, t_room *new_room)
{
	t_link *new;
	
	new = new_link(new_room);
	if (!*queue)
		*queue = new;
	else
	{
		new->next = *queue;
		*queue = new;
	}
}

t_room	*dequeue(t_link **queue)
{
	t_link	*current;
	t_room	*tmp;

	current = *queue;
	if (!current->next)
	{
		tmp = current->room;
		(*queue) = NULL;
		return (tmp);
	}
	while (current->next->next)
		current = current->next;
	tmp = current->next->room;
	current->next = NULL;
	return (tmp);
	
}

int		room_in_links(t_room *room, t_link *link)
{
	while (link)
	{
		if (link->room == room)
			return (1);
		link = link->next;
	}
	return (0);
}

void	find_weights(t_farm **farm)
{
	t_link *queue;
	t_room *neighbor;
	int		weight;
	t_link *visited;
	t_link *links;
	t_link *cur;
	
	queue = NULL;
	visited = NULL;
	(*farm)->end->weight = 0;
	enqueue(&queue, (*farm)->end);
	weight = 1;
	while (queue)
	{
		cur = queue;
		while (cur->next)
			cur = cur->next;
		ft_printf("Examining %s neighbors\n", cur->room->name);
		links = cur->room->links;
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited))
			{
				enqueue(&queue, neighbor);
				ft_printf("Added to queue: %s\n", neighbor->name); //
			}
			if (neighbor->weight > cur->room->weight + 1)
			{
				ft_printf("Change %s weight to %d\n", neighbor->name, cur->room->weight+1);
				neighbor->weight = cur->room->weight + 1;
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
		weight++;
	}
}

int	main(void)
{
	t_farm *farm;

	farm = save_input();
	find_weights(&farm); //calculate and add weights to each room
	print_farm(farm);
	//print_file(farm->file_start);
	free_farm(&farm);
	return (0);
}