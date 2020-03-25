/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_room_errors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 23:09:11 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/25 23:10:49 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

void	check_room_errors(t_farm **farm)
{
	check_room_duplicates(farm);
	check_mandatory_commands(farm);
}

// currently checks 4000 rooms in around 0,09 seconds, merge sort and check for
// duplicates might be significantly faster
void	check_room_duplicates(t_farm **farm)
{
	t_room *room;
	t_room *tmp;

	room = ((*farm)->first);
	while (room)
	{
		tmp = room->next;
		while (tmp)
		{
			if (ft_strequ(room->name, tmp->name))
				handle_error("File contains duplicate rooms.");
			tmp = tmp->next;
		}
		room = room->next;
	}
}

void	check_mandatory_commands(t_farm **farm)
{
	if (!(*farm)->start)
		handle_error("Start room has not been specified.");
	if (!(*farm)->end)
		handle_error("End room has not been specified.");
}