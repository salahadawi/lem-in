/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/29 13:02:45 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

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

t_farm	*save_input(void)
{
	t_farm	*farm;
	char	*line;

	init_farm(&farm);
	save_ants_amount(&farm);
	line = save_rooms(&farm);
	check_room_errors(&farm);
	save_links(&farm, line);
	
	return (farm);
}

//Remember to free file after everything
//Maybe should learn more about optimal file reading and parsing?