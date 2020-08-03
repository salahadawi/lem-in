/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 16:11:46 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/03 16:12:24 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int		adjust_flow(t_room *room1, t_room *room2)
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
	if (!tmp1->flow)
		tmp1->flow = -1;
	else if (tmp1->flow < 0)
		tmp1->flow--;
	else
		tmp1->flow = 0;
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = -(tmp1->flow);
	return (value);
}

int		handle_end_found(t_farm *farm, t_room *neighbor, t_link *path, int n)
{
	(void)n;
	adjust_flow(neighbor, neighbor->parent);
	while (neighbor->parent && !neighbor->parent->flow)
	{
		neighbor = neighbor->parent;
		path = path->next;
		if (neighbor->parent)
			adjust_flow(neighbor, neighbor->parent);
		if (neighbor != farm->start)
			neighbor->flow = 1;
	}
	return (1);
}

void	update_queue(t_link **queue, t_link *cur, t_link *links, int num)
{
	t_room *neighbor;

	neighbor = links->room;
	if (neighbor->parent_num < num && (!neighbor->flow
		|| links->flow == -1))
	{
		enqueue(queue, neighbor);
		neighbor->parent = cur->room;
		neighbor->parent_num = num;
	}
}

int		free_two_queues(t_link *queue1, t_link *queue2)
{
	free_queue(queue1);
	free_queue(queue2);
	return (1);
}

t_path	*create_path(t_link *path, int size)
{
	t_path *new_path;

	new_path = (t_path*)ft_memalloc(sizeof(t_path));
	new_path->path = path;
	new_path->size = size;
	new_path->id = path->next->room->name;
	return (new_path);
}
