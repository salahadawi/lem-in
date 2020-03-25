/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 22:54:16 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/25 23:03:59 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem-in.h"

void	free_file(t_file **file)
{
	t_file *tmp;

	while (*file)
	{
		tmp = (*file)->next;
		free((*file)->line);
		free((*file));
		*file = tmp;
	}
}

void	free_farm(t_farm **farm)
{
	t_room *tmp_room;
	t_link *tmp_link;

	free_file(&(*farm)->file_start);
	while ((*farm)->first)
	{
		tmp_room = (*farm)->first->next;
		while ((*farm)->first->links)
		{
			tmp_link = (*farm)->first->links->next;
			free ((*farm)->first->links);
			(*farm)->first->links = tmp_link;
		}
		free((*farm)->first->name);
		free((*farm)->first);
		(*farm)->first = tmp_room;
	}
	free(*farm);
}

void	free_links(char *link1, char *link2)
{
	free(link1);
	free(link2);
}