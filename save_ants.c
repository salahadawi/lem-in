/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 23:06:18 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/07 12:53:37 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

void	save_ants_amount(t_farm **farm)
{
	char *line;

	if (get_next_line(0, &line) != 1)
		handle_error("The file is invalid.");
	while (check_line_comment(line))
	{
		save_line_file(farm, line);
		if (get_next_line(0, &line) != 1)
			handle_error("The file is invalid.");
	}
	check_ants_amount(line);
	(*farm)->ants_amount = ft_atoi(line);
	(*farm)->start_ants_amount = (*farm)->ants_amount;
	save_line_file(farm, line);
}

void	check_ants_amount(char *line)
{
	int i;

	i = 0;
	if (!ft_atoi(line))
		handle_error("Ants amount is not a positive integer.");
	while (line[i])
		if (!ft_isdigit(line[i++]))
			handle_error("Ants amount is not a positive integer.");
	if (ft_atoilong(line) > 2147483647)
		handle_error("The amount of ants exceeds INT_MAX.");
}