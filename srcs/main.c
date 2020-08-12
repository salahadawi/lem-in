/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 22:54:44 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 18:56:17 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

char *g_flags = "\0";

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
	if (!farm->fastest_paths || moves_made < farm->fastest_paths_moves)
	{
		if (farm->fastest_paths)
			free_paths(farm->fastest_paths);
		farm->fastest_paths = farm->paths;
		farm->fastest_paths_moves = moves_made;
	}
	else
		free_paths(farm->paths);
	if ((moves_made - moves_required) > 10)
		return (0);
	return (1);
}

void	optimize_paths(t_farm *farm)
{
	while (farm->paths)
	{
		remove_paths_flow(farm->paths);
		simulate_move_amount(farm);
		farm->paths = NULL;
		if ((double)(clock() - farm->timer)
			/ CLOCKS_PER_SEC > farm->seconds - 1.3)
			break ;
		save_paths(&farm, 1);
	}
	save_paths(&farm, 2);
	while (farm->paths)
	{
		remove_paths_flow(farm->paths);
		simulate_move_amount(farm);
		farm->paths = NULL;
		if ((double)(clock() - farm->timer)
			/ CLOCKS_PER_SEC > farm->seconds - 0.6)
			break ;
		save_paths(&farm, 2);
	}
	if (!farm->paths)
		farm->paths = farm->fastest_paths;
}

int		main(int argc, char **argv)
{
	t_farm *farm;

	farm = save_input(argc, argv);
	find_weights(&farm, 1, NULL);
	check_links_valid(&farm);
	print_file(farm->file_start);
	farm->start->occupied = 1;
	save_paths(&farm, 0);
	if (farm->seconds && farm->rooms_amount > 30)
		optimize_paths(farm);
	move_ants(&farm);
	print_info(farm);
	free_farm(&farm);
	free(g_flags);
	if (ft_strchr(g_flags, 'x'))
		system("leaks lem-in");
	return (0);
}
