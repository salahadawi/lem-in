/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 23:07:44 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/07 19:10:34 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		check_line_comment(char *line)
{
	if (line[0] == '#')
		if (!ft_strequ(line, "##start") && !ft_strequ(line, "##end"))
			return (1);
	return (0);
}

int		check_line_command(char *line)
{
	if (ft_strequ(line, "##start"))
		return (START);
	else if (ft_strequ(line, "##end"))
		return (END);
	return (0);
}
