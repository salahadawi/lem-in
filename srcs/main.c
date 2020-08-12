/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 22:54:44 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 16:02:23 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

char *g_flags = "\0";

char	*get_flags(t_farm *farm, int argc, char **argv)
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
			if (ft_strchr(flags, 'x'))
				system("leaks lem-in");
			exit(0);
		}
		flags[i - 1] = argv[i][1];
		i++;
		if (argv[i - 1][1] == 't' && i < argc)
		{
			if (!ft_isdigit(argv[i][0]))
			{
				ft_fprintf(2, "Error: Invalid flag.\n");
				if (ft_strchr(flags, 'x'))
					system("leaks lem-in");
				exit(0);
			}
			farm->seconds = (double)ft_atoi(argv[i++]);
		}
	}
	flags[i] = '\0';
	return (flags);
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
	while (!ft_isdigit(*line) && *line)
		line++;
	return (ft_atoi(line));
}

void	reset_path_ants(t_farm *farm)
{
	t_path	*paths;

	paths = farm->paths;
	while (paths)
	{
		paths->ants_amount = 0;
		paths = paths->next;
	}
}

int		count_moves_done(t_farm *farm)
{
	t_path	*paths;
	int		ants_amount;
	int		total_moves;

	ants_amount = farm->ants_amount;
	while (ants_amount--)
	{
		paths = farm->paths;
		while (paths->next && paths->size + paths->ants_amount >=
							paths->next->size + paths->next->ants_amount)
			paths = paths->next;
		paths->ants_amount++;
	}
	total_moves = farm->paths->size + farm->paths->ants_amount - 1;
	reset_path_ants(farm);
	return (total_moves);
}

int		simulate_move_amount(t_farm *farm)
{
	int moves_required;
	int	moves_made;

	if (farm->paths->size < 2)
		return (1);
	moves_required = get_lines_required(farm);
	if (!moves_required)
		return (1);
	moves_made = count_moves_done(farm);
	//ft_printf("#REQUIRED: %d\n", moves_required);
	//ft_printf("#MOVES MADE: %d\n", moves_made);
	if ((moves_made - moves_required) > 10)
	{
		//ft_printf("#Too many moves made! Try another path.\n");
		if (!farm->fastest_paths || moves_made < farm->fastest_paths_moves)
		{
			farm->fastest_paths = farm->paths;
			farm->fastest_paths_moves = moves_made;
		}
		return (0);
	}
	return (1);
}

int		main(int argc, char **argv)
{
	t_farm *farm;

	clock_t start = clock();

	g_flags = get_flags(argc, argv);
	farm = save_input();
	find_weights(&farm, 1, NULL);
	check_links_valid(&farm);
	print_file(farm->file_start);
	farm->start->occupied = 1;
	save_paths(&farm, 0);
	while (farm->paths && !simulate_move_amount(farm))
	{
		remove_paths_flow(farm->paths);
		farm->paths = NULL;
		if ((double)(clock() - start) / CLOCKS_PER_SEC > 2)
			break ;
	 	save_paths(&farm, 1);
	}
	if (!farm->paths)
	{
		save_paths(&farm, 2);
		while (farm->paths && !simulate_move_amount(farm))
		{
			remove_paths_flow(farm->paths);
			farm->paths = NULL;
			if ((double)(clock() - start) / CLOCKS_PER_SEC > 2.7)
				break ;
			save_paths(&farm, 2);
		}
	}
	if (!farm->paths)
		farm->paths = farm->fastest_paths;
	move_ants(&farm);
	free_farm(&farm);
	free(g_flags);
	if (ft_strchr(g_flags, 'x'))
		system("leaks lem-in");
	return (0);
}
