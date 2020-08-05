/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 15:34:05 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/05 13:08:51 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int		get_flow_path(t_farm **farm, t_link *queue, t_link *path, int par_num)
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
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited) || neighbor == (*farm)->end)
				update_queue(&queue, cur, links, par_num);
			if (neighbor == (*farm)->end)
				return (handle_end_found(*farm, neighbor, path, par_num) +
					free_two_queues(queue, visited));
				links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	return (free_queue(visited));
}

void	get_flow_paths(t_farm **farm)
{
	int		parent_num;

	parent_num = 1;
	while (get_flow_path(farm, NULL, NULL, parent_num++))
		;
}

t_path	*find_path(t_farm **farm, t_link *links)
{
	t_link	*path;
	t_link	*cur_head;
	int		size;

	size = 1;
	path = new_link((*farm)->start);
	(*farm)->paths_amount++;
	cur_head = path;
	while (links)
	{
		if (links->flow == 1)
		{
			path->next = new_link(links->room);
			path = path->next;
			size++;
			links = links->room->links;
		}
		else
			links = links->next;
	}
	return (create_path(cur_head, size));
}

void	get_paths(t_farm **farm)
{
	t_path	*found_path;
	t_path	*tmp;
	t_link	*links;

	tmp = NULL;
	links = (*farm)->start->links;
	while (links)
	{
		if (links->flow)
		{
			found_path = find_path(farm, links);
			if ((*farm)->paths)
			{
				tmp->next = found_path;
				tmp = tmp->next;
			}
			else
			{
				tmp = found_path;
				(*farm)->paths = tmp;
			}
		}
		links = links->next;
	}
}

void	save_paths(t_farm **farm)
{
	if ((*farm)->rooms_amount < 30)
		get_flow_paths2(farm);
	else
		get_flow_paths(farm);
	get_paths(farm);
	mergesort_paths(&(*farm)->paths);
}
