/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 15:34:05 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/11 20:47:00 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	remove_flow2(t_room *room1, t_room *room2)
{
	t_link	*tmp1;
	t_link	*tmp2;

	tmp1 = room1->links;
	while (tmp1->room != room2)
		tmp1 = tmp1->next;
	tmp1->flow = 0;
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = 0;
}

void	remove_paths_flow(t_path *path)
{
	t_path *tmp_path;
	t_link *tmp_link;

	tmp_path = path;
	while (tmp_path)
	{
		tmp_link = tmp_path->path;
		while (tmp_link)
		{
			if (tmp_link->next)
				remove_flow2(tmp_link->room, tmp_link->next->room);
			tmp_link->room->flow = 0;
			tmp_link = tmp_link->next;
		}
		tmp_path = tmp_path->next;
	}
}

int		get_flow_path3(t_farm **farm, t_link *queue, t_link *path, int par_num)
{
	t_room	*neighbor;
	t_link	*visited;
	t_link	*links;
	t_link	*cur;

	static int i = 0;
	static int j = 0;
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
			{
				enqueue(&queue, neighbor);
				neighbor->parent = cur->room;
				neighbor->parent_num = par_num;
			}
			if (neighbor == (*farm)->end && j++ > i)
			{
				return (handle_end_found(*farm, neighbor, path, par_num) +
					free_two_queues(queue, visited));
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	i++;
	j = 0;
	return (free_queue(visited));
}


int		get_flow_path(t_farm **farm, t_link *queue, t_link *path, int par_num)
{
	t_room	*neighbor;
	t_link	*visited;
	t_link	*links;
	t_link	*cur;

	static int i = 0;
	static int j = 0;
	init_queue(&queue, &visited, farm);
	while (queue)
	{
		cur = queue;
		while (cur->next)
			cur = cur->next;
		links = cur->room->links;
			if (j++ < i)
			{
				links = links->next;
			}
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited) || neighbor == (*farm)->end)
				update_queue(&queue, cur, links, par_num);
			if (neighbor == (*farm)->end)
			{
				return (handle_end_found(*farm, neighbor, path, par_num) +
					free_two_queues(queue, visited));
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	i++;
	j = 0;
	return (free_queue(visited));
}

void	get_flow_paths(t_farm **farm, int mode)
{
	static int		parent_num = 1;

	if (mode != 2)
		while (get_flow_path(farm, NULL, NULL, parent_num++))
			;
	else
		while (get_flow_path3(farm, NULL, NULL, parent_num++))
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
	path = cur_head;
	while (path->next)
		path = path->next;
	if (path->room != (*farm)->end)
		return (NULL);
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
			if (found_path)
			{
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
