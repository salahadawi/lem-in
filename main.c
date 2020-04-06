/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 22:54:44 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/06 22:44:50 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

char *g_flags = "\0";

void	enqueue(t_link **queue, t_room *new_room)
{
	t_link *new;
	
	new = new_link(new_room);
	if (!*queue)
		*queue = new;
	else
	{
		new->next = *queue;
		*queue = new;
	}
}

t_room	*dequeue(t_link **queue)
{
	t_link	*current;
	t_room	*tmp;

	current = *queue;
	if (!current->next)
	{
		tmp = current->room;
		(*queue) = NULL;
		return (tmp);
	}
	while (current->next->next)
		current = current->next;
	tmp = current->next->room;
	current->next = NULL;
	return (tmp);
	
}

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

void	find_weights(t_farm **farm)
{
	t_link *queue;
	t_room *neighbor;
	int		weight;
	t_link *visited;
	t_link *links;
	t_link *cur;
	
	queue = NULL;
	visited = NULL;
	enqueue(&visited, (*farm)->start);
	(*farm)->end->weight = 0;
	enqueue(&queue, (*farm)->end);
	weight = 1;
	while (queue)
	{
		cur = queue;
		while (cur->next)
			cur = cur->next;
		ft_printf("#Examining %s neighbors\n", cur->room->name); //
		links = cur->room->links;
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited))
			{
				enqueue(&queue, neighbor);
				ft_printf("#Added to qu eue: %s\n", neighbor->name); //
			}
			if (neighbor->weight > cur->room->weight + 1)
			{
				ft_printf("#Change %s weight to %d\n", neighbor->name, cur->room->weight+1); //
				neighbor->weight = cur->room->weight + 1;
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
		weight++;
	}
}

char	*get_flags(int argc, char **argv)
{
	char	*flags;
	int		i;
	
	flags = (char*)ft_memalloc(argc);
	i = 1;
	while (i < argc)
	{
		if (ft_strlen(argv[i]) != 2 || argv[i][0] != '-')
		{
			ft_fprintf(2, "Error: Invalid flag.\n");
			exit(0);
		}
		flags[i - 1] = argv[i][1];
		i++;
	}
	flags[i] = '\0';
	return (flags);
}

void	check_links_valid(t_farm **farm)
{
	if ((*farm)->start->weight == MAX_INT)
		handle_error("No path from start to end found.");
}

t_ant	*new_ant(t_farm *farm, int *ant_number)
{
	t_ant *ant;

	ant = (t_ant*)ft_memalloc(sizeof(t_ant));
	ant->room = farm->start;
	ant->number = (*ant_number)++;
	return (ant);
}

void	create_ants(t_farm *farm, t_ant **first)
{
	int		ant_number;
	int		ants_amount;
	t_ant	*ants;
	
	ant_number = 1;
	ants = new_ant(farm, &ant_number);
	(*first) = ants;
	ants_amount = farm->ants_amount;
	while (--ants_amount)
	{
		ants->next = new_ant(farm, &ant_number);
		ants = ants->next;
	}
	ants->next = NULL;
}

void	remove_ant(t_ant **first, t_ant **ants)
{
	t_ant *tmp;
	t_ant *first_tmp;

	tmp = (*ants)->next;
	if (*first == (*ants)->next)
	{
		free(*ants);
		*ants = tmp;
		return ;
	}
	first_tmp = (*first);
	while (first_tmp->next != *ants)
		first_tmp = first_tmp->next;
	first_tmp->next = tmp;
	free(*ants);
	*ants = tmp;
}

int		optimal_room1(t_link *links, t_ant **ant, t_farm *farm)
{
	t_room *optimal_room;

	optimal_room = (*ant)->room;
	while (links)
	{
		if (links->room->weight < optimal_room->weight && !links->room->occupied)
			optimal_room = links->room;	
		links = links->next;
	}
	if ((*ant)->room == optimal_room)
		return (0);
	(*ant)->room->occupied = 0;
	(*ant)->room = optimal_room;
	if ((*ant)->room != farm->end)
		(*ant)->room->occupied = 1;
	return (1);
}

int		optimal_room(t_link *links, t_ant **ant, t_farm *farm)
{
	t_room *optimal_room;
	t_link *tmp_link;

	tmp_link = links;
	optimal_room = (*ant)->room;
	while (links)
	{
		if (links->room->weight <= optimal_room->weight && !links->room->occupied)
			optimal_room = links->room;	
		links = links->next;
	}
	if ((*ant)->room == optimal_room)
	{
		while (tmp_link)
		{
			if (!tmp_link->room->occupied)
				if (tmp_link->room->weight <= optimal_room->weight + farm->ants_amount)
					optimal_room = tmp_link->room;	
			tmp_link = tmp_link->next;
		}
	}
	if ((*ant)->room == optimal_room)
		return (0);
	if ((*ant)->room != farm->start)
		(*ant)->room->occupied = 0;
	(*ant)->room = optimal_room;
	if ((*ant)->room != farm->end)
		(*ant)->room->occupied = 1;
	return (1);
}

void	move_ants(t_farm **farm)
{
	t_ant *ants;
	t_ant *first;
	
	create_ants(*farm, &first);
	while (first)
	{
		ants = first;
		while (ants)
		{
			if ((*farm)->alg(ants->room->links, &ants, *farm))
				ft_printf("L%d-%s ", ants->number, ants->room->name); //maybe somekind of sprintf implementation, currently might print extra spaces
			if (ants->room == (*farm)->end)
			{
				if (ants == first)
					first = ants->next;
				remove_ant(&first, &ants);
				(*farm)->ants_amount--;
			}
			else
				ants = ants->next;
		}
		ft_printf("\n");
	}
}

void	print_visualizer_info(t_farm *farm)
{
	ft_printf("#%d ", farm->ants_amount);
	ft_printf("%s %s\n", farm->start->name, farm->end->name);
}

void	make_links_oneway(t_farm **farm)
{
	t_link *queue;
	t_room *neighbor;
	int		weight;
	t_link *visited;
	t_link *links;
	t_link *cur;
	
	queue = NULL;
	visited = NULL;
	(*farm)->end->weight = 0;
	enqueue(&queue, (*farm)->end);
	weight = 1;
	while (queue)
	{
		cur = queue;
		while (cur->next)
			cur = cur->next;
		ft_printf("#Examining %s neighbors\n", cur->room->name); //
		links = cur->room->links;
		while (links)
		{
			neighbor = links->room;
			if (!room_in_links(neighbor, visited))
			{
				enqueue(&queue, neighbor);
				ft_printf("#Added to queue: %s\n", neighbor->name); //
			}
			if (neighbor->weight > cur->room->weight + 1)
			{
				ft_printf("#Change %s weight to %d\n", neighbor->name, cur->room->weight+1); //
				neighbor->weight = cur->room->weight + 1;
			}
			links = links->next;
		}
		enqueue(&visited, dequeue(&queue));
		weight++;
	}
}

int	main(int argc, char **argv)
{
	t_farm *farm;

	g_flags = get_flags(argc, argv);
	farm = save_input();
	find_weights(&farm); //calculate and add weights to each room
	check_links_valid(&farm);
	// not needed right now, maybe for drawing line weights
	//if (ft_strchr(g_flags, 'v'))
		//print_visualizer_info(farm);
	//print_farm(farm);
	//make_links_oneway(&farm);
	print_file(farm->file_start); //remember to print newline before ant movements
	farm->start->occupied = 1;
	move_ants(&farm); //
	free_farm(&farm);
	free(g_flags);
	return (0);
}