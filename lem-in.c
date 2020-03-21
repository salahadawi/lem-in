/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:18:53 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/21 18:26:30 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

int		handle_error(int code)
{
	if (code == 1)
		ft_printf("Error\n");
	else if (code == 2)
		ft_printf("Error: Malloc failed\n");
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

char	*ft_strcatnl(char *s1, const char *s2)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (!s2)
		return (s1);
	while (s1[i])
		i++;
	while (s2[j])
	{
		s1[i + j] = s2[j];
		j++;
	}
	s1[i + j] = '\n';
	s1[i + j + 1] = '\0';
	return (s1);
}

char	*join_input(char *s1, char *s2)
{
	char *str;
	int i;
	int j;

	i = -1;
	j = -1;
	if (!(str = (char*)ft_memalloc(ft_strlen(s1) + ft_strlen(s2) + 2)))
		return (NULL);
	while (s1[++i])
		str[i] = s1[i];
	while (s2[++j])
		str[i + j] = s2[j];
	str[i + j] = '\n';
	str[i + j + 1] = '\0';
	free(s1);
	free(s2);
	return (str);
}

char	**save_input(void)
{
	char **file;
	char *input;
	char *line;

	input = ft_strnew(0);
	while (get_next_line(0, &line) > 0)
	{
		if (!(input = join_input(input, line)))
			handle_error(2);
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
	//if (!check_file(file))
		//return (handle_error(1));
	while (file[i])
	{
		ft_printf("%s\n", file[i++]);
	}
	return (0);
}

//Need to check if room exists
//Need to check if input contains empty lines and return error
