/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 18:30:49 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	save_line_file(t_farm **farm, char *line)
{
	t_file	**file;

	file = &(*farm)->file_end;
	if (*file)
	{
		(*file)->next = file_new(line);
		*file = (*file)->next;
	}
	else
	{
		*file = file_new(line);
		(*farm)->file_start = *file;
	}
}

t_farm	*save_input(int argc, char **argv)
{
	t_farm	*farm;
	char	*line;

	init_farm(&farm);
	g_flags = get_flags(farm, argc, argv);
	save_ants_amount(&farm);
	line = save_rooms(&farm);
	check_room_errors(&farm);
	save_links(&farm, line);
	return (farm);
}
