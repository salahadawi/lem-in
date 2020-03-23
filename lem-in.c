/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/23 12:53:15 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		handle_error(char *message)
{
	ft_printf("%s\n", message);
	exit(0);
}

void	check_ants_amount(char *line)
{
	int i;

	i = 0;
	while (line[i])
		if (!ft_isdigit(line[i++]))
			handle_error("Error: Ants amount is not a positive integer.");
	if (ft_atoilong(line) > 2147483647)
		handle_error("Error: The amount of ants exceeds INT_MAX.");
}

int		check_line_comment(char *line)
{
	if (line[0] == '#')
	{
		if (!ft_strequ(line, "##start") && !ft_strequ(line, "##end"))
			return (1);
	}
		return (line[1] != '#' && line[1]);
}

int		check_line_command(char *line)
{
	if (line[0] == '#')
		return (line[1] == '#');
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

void	check_file(char **file)
{
	int i;

	i = 1;
	check_ants_amount(file[0]);
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
	if (file[i])
		handle_error("The file is invalid.");
}

t_file	*file_new(char *line)
{
	t_file *file;

	if (!(file = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Error: Malloc failed.");
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
		handle_error("Error: Malloc failed.");
	i = 0;
	while (file)
	{
		new_file[i++] = file->line;
		free_and_go_to_next(&file);
	}
	new_file[i] = 0;
	return (new_file);
}

void	save_ants_amount(t_farm **farm)
{
	char **line;

	if (get_next_line(0, &line) != 1)
		handle_error("The file is invalid.");
	check_ants_amount(line);
	(*farm)->ants_amount = ft_atoi(line);
	free(line);
}



int		get_line_room(t_farm **farm, t_room **room, char *line)
{
	int i;

	i = 0;
	if (check_line_comment(line))
		return (1);
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

void	save_rooms(t_farm **farm)
{
	char	*line;
	t_room	*room;
	int		command;

	room = NULL;
	while (get_next_line(0, &line) > 0)
	{
		if (!check_line_comment(line))
		{
			command = check_line_command;
		}
		if (room)
		{
			if (!(get_line_room(&farm, &room, &line)))
				break ;
			room = room->next;
		}
		else
			if (!(room = get_line_room(&farm, &room, &line)))
				break ;
		if ((*farm)->first)
			(*farm)->first = room;
	}
}

void	init_farm(t_farm **farm)
{
	(*farm) = (t_farm*)malloc(sizeof(t_farm));
	(*farm)->start = NULL;
	(*farm)->end = NULL;
	(*farm)->first = NULL;
}

char	**save_input(void)
{
	char	*line;
	t_file *first;
	t_file *tmp;
	int		size;
	t_farm	*farm;
	
	first = NULL;
	tmp = NULL;
	size = 0;
	init_farm(&farm);
	save_ants_amount(&farm); // add functions here
	save_rooms(&farm);
	save_links(&farm);
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
	check_file(file);
	print_file(file);
	free_file(file);
	return (0);
}

//Remember to free file after everything
//Maybe should learn more about optimal file reading and parsing?