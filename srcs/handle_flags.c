/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 18:54:55 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 18:55:17 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	handle_flag_error(char *flags)
{
	ft_fprintf(2, "Error: Invalid flag.\n");
	if (ft_strchr(flags, 'x'))
		system("leaks lem-in");
	exit(0);
}

char	*get_flags(t_farm *farm, int argc, char **argv)
{
	char	*flags;
	int		i;

	flags = (char*)ft_memalloc(argc);
	i = 1;
	while (i < argc)
	{
		if (ft_strlen(argv[i]) != 2 || argv[i][0] != '-')
			handle_flag_error(flags);
		flags[i - 1] = argv[i][1];
		i++;
		if (argv[i - 1][1] == 't' && i < argc)
		{
			if (!ft_isdigit(argv[i][0]))
				handle_flag_error(flags);
			farm->seconds = (double)ft_atoi(argv[i++]);
		}
	}
	flags[i] = '\0';
	return (flags);
}
