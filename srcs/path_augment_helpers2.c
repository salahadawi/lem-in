/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_augment_helpers2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/05 15:09:44 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/05 15:10:10 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int		adjust_flow2(t_room *room1, t_room *room2)
{
	t_link	*tmp1;
	t_link	*tmp2;
	int		value;

	tmp1 = room1->links;
	while (tmp1->room != room2)
		tmp1 = tmp1->next;
	if (tmp1->flow < 0)
		value = 1;
	else if (tmp1->flow > 0)
		value = -1;
	else
		value = 1;
	return (value);
	tmp1->flow = -(!tmp1->flow);
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = -(tmp1->flow);
	return (value);
}

void	adjust_weight(t_room *room1, t_room *room2, int weight)
{
	t_link	*tmp1;
	t_link	*tmp2;

	tmp1 = room1->links;
	while (tmp1->room != room2)
		tmp1 = tmp1->next;
	tmp1->weight = weight;
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->weight = weight;
}

void	reset_parents(t_farm **farm, t_link *queue)
{
	t_room	*neighbor;
	t_link	*visited;
	t_link	*links;
	t_link	*cur;

	init_queue(&queue, &visited, farm);
	while (queue)
	{
		cur = queue;
		while (cur->next)
			cur = cur->next;
		links = cur->room->links;
		cur->room->parent = NULL;
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited))
				enqueue(&queue, neighbor);
			neighbor->parent = NULL;
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	free_queue(visited);
}

int		check_path(t_room *neighbor, t_link *links)
{
	int weight;

	weight = 1;
	while (neighbor->parent)
	{
		neighbor = neighbor->parent;
		if (neighbor->parent)
			weight += adjust_flow2(neighbor, neighbor->parent);
	}
	if (weight >= links->weight)
		return (0);
	return (1);
}

t_link	*handle_end_found2(t_farm **farm, t_room *neighbor, t_link *q1,
t_link *q2)
{
	t_link	*path_head;
	t_link	*tmp;
	int		weight;

	weight = 1;
	tmp = new_link(neighbor);
	path_head = tmp;
	adjust_flow(neighbor, neighbor->parent);
	while (neighbor->parent)
	{
		neighbor = neighbor->parent;
		tmp->next = new_link(neighbor);
		tmp = tmp->next;
		if (neighbor->parent)
			weight += adjust_flow(neighbor, neighbor->parent);
	}
	adjust_weight((*farm)->end, (*farm)->end->parent, weight);
	reset_parents(farm, NULL);
	free_two_queues(q1, q2);
	return (path_head);
}
