/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/01/24 18:31:47 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		handle_error(int error_code)
{
	if (error_code)
		ft_printf("Error\n");
	return (1);
}

int		check_ants_amount(char *line)
{
	int i;

	i = 0;
	while (line[i])
		if (!ft_isdigit(line[i++]))
			return (0);
	if (ft_atoilong(line) > 2147483647)
		return (0);
	return (1);
}

int		check_line_comment(char *line)
{
	return (line[0] == '#');
}

int		check_line_command(char *line)
{
	if (!line[0])
		return (0);
	return (line[0] == '#' && line[1] == '#');
}

int		check_line_link(char *line)
{
	int i;

	i = 0;
	while (line[i] != '-' && line[i])
		i++;
	if (line[i++] != '-')
		return (0);
	if (!line[i])
		return (0);
	return (1);
}

int		check_line_room(char *line)
{
	int i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	if (line[i++] != ' ')
		return (0);
	if (ft_atoilong(&line[i]) > 2147483647)
		return (0);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (line[i++] != ' ')
		return (0);
	if (ft_atoilong(&line[i]) > 2147483647)
		return (0);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (!line[i])
		return (1);
	return (0);
}


int		check_file(char **file)
{
	int i;

	i = 1;
	if (!check_ants_amount(file[0]))
		return (0);
	while (file[i])
	{
		if (!check_line_room(file[i]) && !check_line_command(file[i])
			&& !check_line_comment(file[i]))
			break ;
		i++;
	}
	while (file[i])
	{
		if (!check_line_link(file[i]) && !check_line_comment(file[i]))
			break ;
		i++;
	}
	if (!file[i])
		return (1);
	return (0);
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

//Need to check if room exists
//Need to check if input contains empty lines and return error
