/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 15:34:05 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/03 16:01:12 by sadawi           ###   ########.fr       */
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

t_path	*create_path(t_link *path, int size)
{
	t_path *new_path;

	new_path = (t_path*)ft_memalloc(sizeof(t_path));
	new_path->path = path;
	new_path->size = size;
	new_path->id = path->next->room->name;
	return (new_path);
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

t_path	*sorted_merge(t_path *first_half, t_path *second_half)
{
	t_path *sorted;

	if (!first_half)
		return (second_half);
	if (!second_half)
		return (first_half);
	if (first_half->size < second_half->size)
	{
		sorted = first_half;
		sorted->next = sorted_merge(first_half->next, second_half);
	}
	else
	{
		sorted = second_half;
		sorted->next = sorted_merge(first_half, second_half->next);
	}
	return (sorted);
}

void	split_list(t_path *head, t_path **first_half, t_path **second_half)
{
	t_path *fast;
	t_path *slow;

	slow = head;
	fast = head->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*first_half = head;
	*second_half = slow->next;
	slow->next = NULL;
}

void	mergesort_paths(t_path **paths)
{
	t_path	*head;
	t_path	*first_half;
	t_path	*second_half;

	head = *paths;
	if (!head || !head->next)
		return ;
	split_list(head, &first_half, &second_half);
	mergesort_paths(&first_half);
	mergesort_paths(&second_half);
	*paths = sorted_merge(first_half, second_half);
}

void	save_paths(t_farm **farm)
{
	get_flow_paths(farm);
	get_paths(farm);
	mergesort_paths(&(*farm)->paths);
}
