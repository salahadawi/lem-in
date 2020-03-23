/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/22 21:30:05 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H
# define START 1
# define END 2

#include "libft.h"

typedef	struct		s_file
{
	char 			*line;
	struct s_file	*next;
}					t_file;

typedef	struct 		s_room
{
	char			*name;
	int				x;
	int				y;
	char			*next;
	struct s_link	*links;
}					t_room;

typedef	struct		s_link
{
	t_room			*linked_room;
	struct s_link	*next;
}					t_link;

typedef	struct		s_farm
{
	int				ants_amount;
	t_room			*start;
	t_room			*end;
	t_room			*first;
}					t_farm;

#endif
