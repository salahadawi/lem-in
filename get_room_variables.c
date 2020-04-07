/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_room_variables.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 23:16:06 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/07 19:09:33 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		get_line_room(t_room **room, char *line)
{
	int i;

	i = 0;
	*room = new_room();
	if (!(get_room_name(room, line, &i)))
		return (0);
	if (!(get_room_x(room, line, &i)))
		return (0);
	if (!(get_room_y(room, line, &i)))
		return (0);
	return (1);
}

int		get_room_name(t_room **room, char *line, int *i)
{
	while (line[*i] != ' ' && line[*i])
		(*i)++;
	if (line[(*i)++] != ' ')
		return (0);
	(*room)->name = ft_strsub(line, 0, *i - 1);
	return (1);
}

int		get_room_x(t_room **room, char *line, int *i)
{
	if (ft_atoilong(&line[*i]) > 2147483647)
	{
		free((*room)->name);
		return (0);
	}
	(*room)->x = ft_atoi(&line[*i]);
	if (line[*i] == '-')
		(*i)++;
	if (!ft_isdigit(line[*i]))
	{
		free((*room)->name);
		return (0);
	}
	while (ft_isdigit(line[*i]))
		(*i)++;
	if (line[(*i)++] != ' ')
	{
		free((*room)->name);
		return (0);
	}
	return (1);
}

int		get_room_y(t_room **room, char *line, int *i)
{
	if (ft_atoilong(&line[*i]) > 2147483647)
	{
		free((*room)->name);
		return (0);
	}
	(*room)->y = ft_atoi(&line[*i]);
	if (line[*i] == '-')
		(*i)++;
	if (!ft_isdigit(line[*i]))
	{
		free((*room)->name);
		return (0);
	}
	while (ft_isdigit(line[*i]))
		(*i)++;
	if (!line[*i])
		return (1);
	free((*room)->name);
	return (0);
}
