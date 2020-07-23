/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 15:34:05 by sadawi            #+#    #+#             */
/*   Updated: 2020/07/23 20:25:41 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/* void	reset_weights(t_farm **farm, int weight, t_link *queue)
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
			if (!room_in_links(neighbor, visited))
				enqueue(&queue, neighbor);
			neighbor->weight = MAX_INT;
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
		weight++;
	}
	free_queue(visited);
}
 */
/* void	print_weights(t_farm **farm, t_link *queue)
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
			if (!room_in_links(neighbor, visited))
				enqueue(&queue, neighbor);
			ft_printf("#Room %s weight: %d\n", neighbor->name, neighbor->weight);
			ft_printf("#Link %s-%s flow: %d\n", cur->room->name, neighbor->name, links->flow);
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	free_queue(visited);
} */

/* void	find_weights2(t_farm **farm, int weight, t_link *queue)
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
			if (!room_in_links(neighbor, visited))
				enqueue(&queue, neighbor);
			if (neighbor->weight > cur->room->weight + 1)
			{
				if (cur->room == (*farm)->end && links->flow)
					neighbor->weight = MAX_INT - 50;
				else
					neighbor->weight = cur->room->weight + 1;
			}
			else
				neighbor->weight = MAX_INT - 50;
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
		weight++;
	}
	free_queue(visited);
} */

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
	//tmp1->flow = -(tmp1->flow == -1 || tmp1->flow == 0);
	ft_printf(" Flow: %d,", tmp1->flow);
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = -(tmp1->flow);
	ft_printf("%d\n", tmp2->flow);
	return (value);
}

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
	ft_printf(" Flow: %d,", tmp1->flow);
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = -(tmp1->flow);
	ft_printf("%d\n", tmp2->flow);
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
	ft_printf(" Weight: %d,", tmp1->weight);
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->weight = weight;
	ft_printf("%d\n", tmp2->weight);
}

/* void	save_paths2(t_farm **farm)
{
	t_room *tmp;
	t_link *link;
	t_link *best;
	int i = 0;

	tmp = (*farm)->start;
	ft_printf("#%s->", tmp->name);
	while (tmp != (*farm)->end && i < 100)
	{
		link = tmp->links;
		best = NULL;
		while (link)
		{
			if (!(tmp == (*farm)->start && link->flow) && (!best || link->room->weight < best->room->weight))
				best = link;
			link = link->next;
		}
		if (!best)
			return ;
		if (best->room != (*farm)->end)
			ft_printf("%s->", best->room->name);
		else
			ft_printf("%s\n", best->room->name);
		//adjust_flow(best, tmp);
		tmp = best->room;
		i++;
	}
	//print_weights(farm, NULL);
	reset_weights(farm, 1, NULL);
	//print_weights(farm, NULL);
	find_weights2(farm, 1, NULL);
	print_weights(farm, NULL);
} */

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

t_link	*get_flow_path(t_farm **farm, t_link *queue, t_link *path)
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
				{
					neighbor->parent = cur->room;
					ft_printf("#%s, neighbor of: %s\n", neighbor->name, cur->room->name);
				}
			}
			if (neighbor == (*farm)->end)
			{
				ft_printf("#FOUND LINK %s-%s WITH FLOW: %d\n", cur->room->name, neighbor->name, links->flow);
				if (links->flow)
				{
					if (!check_path(neighbor, links))
					{
						ft_printf("#Bad path, discarding...\n");
						break;
					}
					else
						ft_printf("#Augmenting path found!\n");
				}
				ft_printf("#%s\n#|", neighbor->name);
				path = new_link(neighbor);
				path_head = path;
				adjust_flow(neighbor, neighbor->parent);
				int weight = 1;
				while (neighbor->parent)
				{
					neighbor = neighbor->parent;
					path->next = new_link(neighbor);
					path = path->next;
					ft_printf(neighbor->parent ? "#%s\n#|" : "#%s", neighbor->name);
					if (neighbor->parent)
						weight += adjust_flow(neighbor, neighbor->parent);
				}
				ft_printf("\n#Path total weight: %d", weight);
				adjust_weight((*farm)->end, (*farm)->end->parent, weight);
				reset_parents(farm, NULL);
				return (path_head);
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	free_queue(visited);
	ft_printf("#No path found!\n");
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
	//tmp1->flow = -(tmp1->flow == -1 || tmp1->flow == 0);
	ft_printf(" Flow: %d,", tmp1->flow);
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = -(tmp1->flow);
	ft_printf("%d\n", tmp2->flow);
}

void	remove_path_flow(t_link *path)
{
	while (path->next)
	{
		remove_flow(path->room, path->next->room);
		path = path->next;
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
			free(path);
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

t_path	*create_path(t_link *path)
{
	t_path *new_path;

	new_path = (t_path*)ft_memalloc(sizeof(t_path));
	new_path->path = path;
	new_path->id = path->next->room->name;
	return (new_path);
}

void	get_flow_paths(t_farm **farm)
{
	t_path	*tmp;
	t_link	*new_path;

	while ((new_path = get_flow_path(farm, NULL, NULL)))
	{
		augment_path(farm, new_path);
		if (!(*farm)->paths)
			(*farm)->paths = create_path(new_path);
		else
		{
			tmp = (*farm)->paths;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = create_path(new_path);
			tmp = tmp->next;
		}
	}
}

void	print_paths(t_path *paths)
{
	t_path *path;
	t_link *rooms;

	path = paths;
	while (path)
	{
		rooms = path->path;
		while (rooms)
		{
			ft_printf(rooms->next ? "%s->" : "%s\n", rooms->room->name);
			rooms = rooms->next;
		}
		path = path->next;
	}
}

int		find_flow(t_room *room1, t_room *room2)
{
	t_link	*tmp1;
	t_link	*tmp2;
	int		value;
	
	value = 0;
	tmp1 = room1->links;
	while (tmp1->room != room2)
		tmp1 = tmp1->next;
	ft_printf(" Flow: %d,", tmp1->flow);
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	ft_printf("%d\n", tmp2->flow);
	return (value);
}

void	print_flow(t_farm **farm, t_link *queue)
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
			if (!room_in_links(neighbor, visited))
				enqueue(&queue, neighbor);
			if (links->flow)
			{
				ft_printf(" Flow %s-%s: ", links->room->name, cur->room->name);
				find_flow(neighbor, cur->room);
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
	}
	free_queue(visited);
}

t_path	*get_paths(t_farm **farm)
{
	t_path	*path_head;
	t_path	*tmp;
	t_link	*cur_head;
	t_link	*path;
	t_link	*links;
	t_link	*cur;

	path_head = NULL;
	tmp = NULL;
	links = (*farm)->start->links;
	while (links)
	{
		if (links->flow)
		{
			path = new_link((*farm)->start);
			cur_head = path;
			ft_printf("%s",(*farm)->start->name);
			cur = links;
			while (cur)
			{
				//ft_printf("checking %s...", cur->room->name);
				if (cur->flow == 1)
				{
					path->next = new_link(cur->room);
					path = path->next;
					ft_printf("->%s", cur->room->name);
					cur = cur->room->links;
				}
				else
					cur = cur->next;
			}
			ft_printf("\n");
			if (path_head)
			{
				tmp->next = create_path(cur_head);
				tmp = tmp->next;
			}
			else
			{
				tmp = create_path(cur_head);
				path_head = tmp;
			}
		}
		links = links->next;
	}
	return (path_head);
}

void	save_paths(t_farm **farm)
{
	t_path	*final_paths;


	get_flow_paths(farm);
	print_paths((*farm)->paths);
	print_flow(farm, NULL);
	final_paths = get_paths(farm);
	print_paths(final_paths);
}