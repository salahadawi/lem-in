/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 23:08:25 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/25 23:08:44 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		find_first_room_by_names(t_room **room, char *name1, char *name2)
{
	while (!ft_strequ((*room)->name, name1) && !ft_strequ((*room)->name, name2))
	{
		*room = (*room)->next;
		if (!*room)
			handle_error("Room specified in links not found.");
	}
	if (ft_strequ((*room)->name, name1))
		return (1);
	else
		return (2);	
}

void	find_room_by_name(t_room **room, char *name)
{
	if (ft_strequ((*room)->name, name))
		handle_error("Links contain a room linking to itself.");
	while (!ft_strequ((*room)->name, name))
	{
		*room = (*room)->next;
		if (!*room)
			handle_error("Room specified in links not found.");
	}
}