/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 21:00:27 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/29 16:51:39 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

void	print_farm(t_farm *farm)
{
	t_room *room;
	t_link *link;

	room = farm->first;
	ft_printf("%d\n", farm->ants_amount);
	while (room)
	{
		if (farm->start == room)
			ft_printf("#start\n");
		if (farm->end == room)
			ft_printf("#end\n");
		ft_printf("%s %d %d\n", room->name, room->x, room->y);
		ft_printf("Weight: %d\n", room->weight); //debug for room weight
		room = room->next;
	}
	room = farm->first;
	while (room)
	{
		link = room->links;
		while (link)
		{
			ft_printf("%s-%s\n", room->name, link->room->name);
			link = link->next;
		}
		room = room->next;
	}
}

void	print_file(t_file *file)
{
	while (file)
	{
		ft_printf("%s\n", file->line);
		file = file->next;
	}
}