/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_room_errors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 23:09:11 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/07 20:46:25 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	check_room_errors(t_farm **farm)
{
	check_room_duplicates(farm);
	check_mandatory_commands(farm);
}

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
