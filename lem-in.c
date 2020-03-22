/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/22 17:20:53 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		handle_error(int code)
{
	if (code == 1)
		ft_printf("Error: File is invalid.\n");
	else if (code == 2)
		ft_printf("Error: Malloc failed.\n");
	exit(0);
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

t_file	*file_new(char *line)
{
	t_file *file;

	if (!(file = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error(2);
	file->next = NULL;
	file->line = line;
	return (file);
}

void	free_and_go_to_next(t_file **file)
{
	t_file *prev;
	
	prev = *file;
	*file = (*file)->next;
	free(prev);
}

char	**list_to_arr(t_file *file, int size)
{
	char	**new_file;
	int		i;
	
	if (!(new_file = (char**)ft_memalloc(sizeof(char**) * (size + 1))))
		handle_error(2);
	i = 0;
	while (file)
	{
		new_file[i++] = file->line;
		free_and_go_to_next(&file);
	}
	new_file[i] = 0;
	return (new_file);
}

char	**save_input(void)
{
	char *line;
	t_file *first;
	t_file *tmp;
	int		size;
	
	first = NULL;
	tmp = NULL;
	size = 0;
	while (get_next_line(0, &line) > 0)
	{
		if (tmp)
		{
			tmp->next = file_new(line);
			tmp = tmp->next;
		}
		else
			tmp = file_new(line);
		if (!first)
			first = tmp;
		size++;
	}
	tmp->next = NULL;
	return (list_to_arr(first, size));
}

void	print_file(char **file)
{
	int i;

	i = 0;
	while (file[i])
		ft_printf("%s\n", file[i++]);
}

void	free_file(char **file)
{
	int i;

	i = 0;
	while (file[i])
		free(file[i++]);
	free(file[i]);
	free(file);
}

int	main(void)
{
	char	**file;
	
	file = save_input();
	if (!check_file(file))
		return (handle_error(1));
	print_file(file);
	free_file(file);
	return (0);
}

//Need to check if room exists
//Need to check if input contains empty lines and return error
//Remember to free file after everything
//Maybe should learn more about optimal file reading and parsing?