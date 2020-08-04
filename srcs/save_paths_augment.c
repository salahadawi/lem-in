/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_paths_augment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 18:08:13 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/04 18:25:06 by sadawi           ###   ########.fr       */
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
	int weight = 1;
	while (neighbor->parent)
	{
		neighbor = neighbor->parent;
		if (neighbor->parent)
			weight += adjust_flow2(neighbor, neighbor->parent);
	}
	if (weight > links->weight)
		return (0);
	return (1);
}

t_link	*get_flow_path2(t_farm **farm, t_link *queue, t_link *path)
{
	t_link	*path_head;
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
		if (cur->room == (*farm)->end)
		{
			enqueue(&visited, dequeue(&queue));
			continue;
		}
		while (links)
		{
			neighbor = links->room;
			if (cur->room == (*farm)->start && links->flow)
			{
				links = links->next;
				continue;
			}
			if (!room_in_links(neighbor, visited) || neighbor == (*farm)->end)
			{
				enqueue(&queue, neighbor);
				if (!neighbor->parent || neighbor == (*farm)->end)
					neighbor->parent = cur->room;
			}
			if (neighbor == (*farm)->end)
			{
				if (links->flow)
				{
					if (!check_path(neighbor, links))
						break;
				}
				path = new_link(neighbor);
				path_head = path;
				adjust_flow(neighbor, neighbor->parent);
				int weight = 1;
				while (neighbor->parent)
				{
					neighbor = neighbor->parent;
					path->next = new_link(neighbor);
					path = path->next;
					if (neighbor->parent)
						weight += adjust_flow(neighbor, neighbor->parent);
				}
				adjust_weight((*farm)->end, (*farm)->end->parent, weight);
				reset_parents(farm, NULL);
				free_two_queues(visited, queue);
				return (path_head);
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	free_queue(visited);
	reset_parents(farm, NULL);
	return (NULL);
}

void	remove_flow(t_room *room1, t_room *room2)
{
	t_link	*tmp1;
	t_link	*tmp2;

	tmp1 = room1->links;
	while (tmp1->room != room2)
		tmp1 = tmp1->next;
	tmp1->flow++;
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = -(tmp1->flow);
}

void	remove_path_flow(t_link *path)
{
	while (path->next)
	{
		remove_flow(path->room, path->next->room);
		path = path->next;
	}
}

void	free_path(t_path *path)
{
	t_link *tmp_link;

	while (path->path)
	{
		tmp_link = path->path->next;
		free(path->path);
		path->path = tmp_link;
	}
	free(path);
}

void	free_link(t_link *link)
{
	t_link *tmp;

	while (link)
	{
		tmp = link->next;
		free(link);
		link = tmp;
	}
}

void	augment_path(t_farm **farm, t_link *new_path)
{
	t_path *path;
	t_path *tmp;
	t_path *prev;

	prev = NULL;
	path = (*farm)->paths;
	while (path)
	{
		if (ft_strequ(path->id, new_path->next->room->name))
		{
			remove_path_flow(path->path);
			tmp = path->next;
			free_link(new_path);
			if (prev)
				prev->next = tmp;
			else
				(*farm)->paths = tmp;
			return ;
		}
		prev = path;
		path = path->next;
	}
	free_link(new_path);
}

void	get_flow_paths2(t_farm **farm)
{
	t_link	*new_path;

	while ((new_path = get_flow_path2(farm, NULL, NULL)))
		augment_path(farm, new_path);
	free_paths(farm);
}

t_path	*get_paths2(t_farm **farm)
{
	t_path	*path_head;
	t_path	*tmp;
	t_link	*cur_head;
	t_link	*path;
	t_link	*links;
	t_link	*cur;
	int		size;

	path_head = NULL;
	tmp = NULL;
	links = (*farm)->start->links;
	while (links)
	{
		if (links->flow)
		{
			size = 1;
			path = new_link((*farm)->start);
			(*farm)->paths_amount++;
			cur_head = path;
			cur = links;
			while (cur)
			{
				if (cur->flow == 1)
				{
					path->next = new_link(cur->room);
					path = path->next;
					size++;
					cur = cur->room->links;
				}
				else
					cur = cur->next;
			}
			if (path_head)
			{
				tmp->next = create_path(cur_head, size);
				tmp = tmp->next;
			}
			else
			{
				tmp = create_path(cur_head, size);
				path_head = tmp;
			}
		}
		links = links->next;
	}
	return (path_head);
}
