/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_paths_augment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 18:08:13 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 15:53:45 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	queue_room(t_farm **farm, t_room *neighbor, t_link **queues[2],
t_link *cur)
{
	if (!room_in_links(neighbor, *queues[0]) || neighbor == (*farm)->end)
	{
		enqueue(queues[1], neighbor);
		if (!neighbor->parent || neighbor == (*farm)->end)
			neighbor->parent = cur->room;
	}
}

t_link	*loop_links(t_farm **farm, t_link *links, t_link *cur,
t_link **queues[2])
{
	t_room *neighbor;

	while (links)
	{
		neighbor = links->room;
		if (cur->room == (*farm)->start && links->flow)
		{
			links = links->next;
			continue;
		}
		queue_room(farm, neighbor, queues, cur);
		if (neighbor == (*farm)->end)
		{
			if (links->flow && !check_path(neighbor, links))
				break ;
			return (handle_end_found2(farm, neighbor, *queues[0], *queues[1]));
		}
		links = links->next;
	}
	return (NULL);
}

t_link	*get_flow_path2(t_farm **farm, t_link *queue, t_link *path)
{
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
		if ((path = loop_links(farm, links, cur,
		((t_link**[2]){&visited, &queue}))))
			return (path);
		enqueue(&visited, dequeue(&queue));
	}
	free_queue(visited);
	reset_parents(farm, NULL);
	return (NULL);
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
			free_path(path);
			if (prev)
				prev->next = tmp;
			else
				(*farm)->paths = tmp;
			return ;
		}
		prev = path;
		path = path->next;
	}
}

void	get_flow_paths2(t_farm **farm)
{
	int		i;
	t_path	*tmp;
	t_link	*new_path;

	i = 0;
	while (i++ < 10000 && (new_path = get_flow_path2(farm, NULL, NULL)))
	{
		augment_path(farm, new_path);
		if (!(*farm)->paths)
			(*farm)->paths = create_path(new_path, 0);
		else
		{
			tmp = (*farm)->paths;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = create_path(new_path, 0);
			tmp = tmp->next;
		}
	}
	free_paths((*farm)->paths);
}
