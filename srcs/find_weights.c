/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_weights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:58:52 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/05 13:30:24 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

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

void	init_queue2(t_link **queue, t_link **visited, t_farm **farm)
{
	*queue = NULL;
	*visited = NULL;
	(*farm)->end->weight = 0;
	enqueue(queue, (*farm)->end);
}

void	find_weights(t_farm **farm, int weight, t_link *queue)
{
	t_room	*neighbor;
	t_link	*visited;
	t_link	*links;
	t_link	*cur;

	init_queue2(&queue, &visited, farm);
	while (queue)
	{
		cur = queue;
		while (cur->next)
			cur = cur->next;
		links = cur->room->links;
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited))
				enqueue(&queue, neighbor);
			if (neighbor->weight > cur->room->weight + 1)
				neighbor->weight = cur->room->weight + 1;
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
		weight++;
	}
	free_queue(visited);
}
