/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 15:34:05 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 18:57:49 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int		get_flow_path3(t_farm **farm, t_link *queue, t_link *path, int par_num)
{
	t_room		*neighbor;
	t_link		*visited;
	t_link		*links;
	t_link		*cur;
	static int	i[2];

	init_queue(&queue, &visited, farm);
	while (queue)
	{
		cur = get_cur_and_links(queue, &links);
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited) || neighbor == (*farm)->end)
				update_queue(&queue, cur, links, par_num);
			if (neighbor == (*farm)->end && i[1]++ > i[0])
				return (handle_end_found(*farm, neighbor, path, par_num)
					+ free_two_queues(queue, visited));
				links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	return (free_queue_and_increment(visited, &(i[0]), &i[1]));
}

int		get_flow_path(t_farm **farm, t_link *queue, t_link *path, int par_num)
{
	t_room		*neighbor;
	t_link		*visited;
	t_link		*links;
	t_link		*cur;
	static int	i[2];

	init_queue(&queue, &visited, farm);
	while (queue)
	{
		cur = get_cur_and_links(queue, &links);
		if (i[1]++ < i[0])
			links = links->next;
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
	return (free_queue_and_increment(visited, &(i[0]), &i[1]));
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

void	save_paths(t_farm **farm, int mode)
{
	if ((*farm)->rooms_amount < 30)
		get_flow_paths2(farm);
	else
		get_flow_paths(farm, mode);
	get_paths(farm);
	if (!(*farm)->paths)
		return ;
	mergesort_paths(&(*farm)->paths);
}
