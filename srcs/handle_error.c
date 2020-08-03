/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 20:56:16 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/03 15:34:03 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int		handle_error(char *message)
{
	if (ft_strchr(g_flags, 'e'))
		ft_fprintf(2, "Error: %s\n", message);
	else
		ft_fprintf(2, "Error\n");
	if (ft_strchr(g_flags, 'x'))
		system("leaks lem-in");
	exit(0);
}
