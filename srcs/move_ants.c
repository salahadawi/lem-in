/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:56:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/07/21 14:32:28 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	move_ants(t_farm **farm)
{
	t_ant	*ants;
	t_ant	*first;
	int		moves;

	moves = 0;
	create_ants(*farm, &first);
	while (first)
	{
		ants = first;
		while (ants)
		{
			if ((*farm)->alg(ants->room->links, &ants, *farm))
				ft_printf("L%d-%s ", ants->number, ants->room->name);
			if (ants->room == (*farm)->end)
			{
				if (ants == first)
					first = ants->next;
				remove_ant(&first, &ants);
			}
			else
				ants = ants->next;
		}
		ft_printf("\n", moves++);
	}
	ft_printf((ft_strchr(g_flags, 'v') ? "#Total moves: %d" : ""), moves);
}

int		optimal_room(t_link *links, t_ant **ant, t_farm *farm)
{
	t_room	*optimal_room;
	t_link	*tmp_link;

	tmp_link = links;
	optimal_room = (*ant)->room;
	while (links)
	{
		if (links->room->weight <= optimal_room->weight
		&& !links->room->occupied)
			optimal_room = links->room;
		links = links->next;
	}
	if ((*ant)->room == optimal_room)
		optimal_room = find_next_optimal_room(tmp_link, *ant, farm);
	if ((*ant)->room == optimal_room)
		return (0);
	if ((*ant)->room != farm->start)
		(*ant)->room->occupied = 0;
	if ((*ant)->room == farm->start)
		farm->start_ants_amount--;
	(*ant)->room = optimal_room;
	if ((*ant)->room != farm->end)
		(*ant)->room->occupied = 1;
	return (1);
}

t_room	*find_next_optimal_room(t_link *links, t_ant *ant, t_farm *farm)
{
	t_room *optimal_room;

	optimal_room = ant->room;
	while (links)
	{
		if (!links->room->occupied)
			if (links->room->weight <= optimal_room->weight
			+ farm->start_ants_amount - ant->room->weight)
				optimal_room = links->room;
		links = links->next;
	}
	return (optimal_room);
}

int		optimal_room1(t_link *links, t_ant **ant, t_farm *farm)
{
	t_room *optimal_room;

	optimal_room = (*ant)->room;
	while (links)
	{
		if (links->room->weight < optimal_room->weight
		&& !links->room->occupied)
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

int		count_ants_in_line(t_ant *ant, t_farm *farm)
{
	int		ants_in_line;
	t_room	*current_room;
	t_room	*next_room;

	current_room = ant->room;
	ants_in_line = 0;
	while (current_room != farm->start)
	{
		next_room = current_room->links->room;
		current_room->links = current_room->links->next;
		while (current_room->links)
		{
			if (current_room->links->room->weight > next_room->weight)
				next_room = current_room->links->room;
		}
		if (next_room->occupied && next_room != farm->start)
			ants_in_line++;
		current_room = next_room;
		ft_printf("%d ", ants_in_line);
		ft_printf("\n");
	}
	return (ants_in_line);
}
