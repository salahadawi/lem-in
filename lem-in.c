/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/01/24 17:22:06 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		handle_error(int error_code)
{
	if (error_code)
		ft_printf("Error\n");
	return (1);
}

int		check_ants_amount(char *str)
{
	int i;

	i = 0;
	while (str[i])
		if (!ft_isdigit(str[i++]))
			return (0);
	if (ft_atoilong(str) > 2147483647)
		return (0);
	return (1);
}

int		check_file(char **file)
{
	if (!check_ants_amount(file[0]))
		return (0);
	return (1);
}

char	**save_input(void)
{
	char **file;
	char *input;
	char *line;

	input = ft_strnew(0);
	while (get_next_line(0, &line))
	{
		line = ft_strjoinfree(line, ft_strdup("\n"));
		input = ft_strjoinfree(input, line);
	}
	file = ft_strsplit(input, '\n');
	free(input);
	return (file);
}

int	main(void)
{
	char **file;
	int i;

	i = 0;
	file = save_input();
	if (!check_file(file))
		return (handle_error(1));
	while (file[i])
		ft_printf("%s\n", file[i++]);
	return (0);
}
