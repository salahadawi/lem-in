/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 20:56:16 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/07 20:20:23 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

int		handle_error(char *message)
{
	if (ft_strchr(g_flags, 'e'))
		ft_fprintf(2, "Error: %s\n", message);
	else
		ft_fprintf(2, "Error\n");
	exit(0);
}
