/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:56:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/04 18:37:06 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int		ant_move_through_path(t_ant **ant, t_farm *farm)
{
	if (!(*ant)->path->next->room->occupied)
	{
		(*ant)->path->room->occupied = 0;
		(*ant)->path = (*ant)->path->next;
		if ((*ant)->path->room != farm->end)
			(*ant)->path->room->occupied = 1;
		return (1);
	}
	else
		return (0);
}

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
			if (ant_move_through_path(&ants, *farm))
				ft_printf("L%d-%s ", ants->number, ants->path->room->name);
			if (ants->path->room == (*farm)->end)
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
	ft_printf((ft_strchr(g_flags, 'v') ? "#Total moves: %d\n" : ""), moves);
}

t_link	*assign_ant_path(t_farm *farm)
{
	t_path	*paths;

	paths = farm->paths;
	while (paths->next && paths->size + paths->ants_amount >=
							paths->next->size + paths->next->ants_amount)
		paths = paths->next;
	paths->ants_amount++;
	return (paths->path);
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
