/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 22:54:44 by sadawi            #+#    #+#             */
/*   Updated: 2020/07/24 14:40:28 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

char *g_flags = "\0";

char	*get_flags(int argc, char **argv)
{
	char	*flags;
	int		i;

	flags = (char*)ft_memalloc(argc);
	i = 1;
	while (i < argc)
	{
		if (ft_strlen(argv[i]) != 2 || argv[i][0] != '-')
		{
			ft_fprintf(2, "Error: Invalid flag.\n");
			exit(0);
		}
		flags[i - 1] = argv[i][1];
		i++;
	}
	flags[i] = '\0';
	return (flags);
}

void	print_visualizer_info(t_farm *farm)
{
	ft_printf("#%d ", farm->ants_amount);
	ft_printf("%s %s\n", farm->start->name, farm->end->name);
}

int		main(int argc, char **argv)
{
	t_farm *farm;

	g_flags = get_flags(argc, argv);
	farm = save_input();
	find_weights(&farm, 1, NULL);
	check_links_valid(&farm);
	print_file(farm->file_start);
	farm->start->occupied = 1;
	save_paths(&farm);
	move_ants(&farm);
	free_farm(&farm);
	free(g_flags);
	return (0);
}
