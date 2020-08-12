/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 18:53:26 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 18:56:21 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	print_info(t_farm *farm)
{
	int	lines_required;

	if (ft_strchr(g_flags, 'd'))
	{
		lines_required = get_lines_required(farm);
		if (lines_required != -1)
			ft_printf("#Here is the number of lines required: %d\n",
			lines_required);
		ft_printf("#Amount of moves made: %d\n", farm->moves_done);
		if (lines_required != -1)
			ft_printf("#Difference: %+d\n", farm->moves_done - lines_required);
	}
	else if (ft_strchr(g_flags, 'v'))
		ft_printf("#Total moves: %d\n", farm->moves_done);
}

void	print_visualizer_info(t_farm *farm)
{
	ft_printf("#%d ", farm->ants_amount);
	ft_printf("%s %s\n", farm->start->name, farm->end->name);
}

int		get_lines_required(t_farm *farm)
{
	char *line;

	if (farm->file_start && farm->file_start->next)
		line = farm->file_start->next->line;
	else
		return (-1);
	if (!ft_strstr(line, "#Here is the number of lines required:"))
		return (-1);
	while (!ft_isdigit(*line) && *line)
		line++;
	return (ft_atoi(line));
}
