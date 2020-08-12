/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 22:54:16 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 16:03:32 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

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

void	free_paths(t_path *paths)
{
	t_path *tmp_path;
	t_link *tmp_link;

	while (paths)
	{
		while (paths->path)
		{
			tmp_link = paths->path->next;
			free(paths->path);
			paths->path = tmp_link;
		}
		tmp_path = paths->next;
		free(paths);
		paths = tmp_path;
	}
}

void	free_farm(t_farm **farm)
{
	t_room *tmp_room;
	t_link *tmp_link;

	free_file(&(*farm)->file_start);
	free_paths((*farm)->paths);
	while ((*farm)->first)
	{
		tmp_room = (*farm)->first->next;
		while ((*farm)->first->links)
		{
			tmp_link = (*farm)->first->links->next;
			free((*farm)->first->links);
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

void	remove_ant(t_ant **first, t_ant **ants)
{
	t_ant *tmp;
	t_ant *first_tmp;

	tmp = (*ants)->next;
	if (*first == (*ants)->next)
	{
		free(*ants);
		*ants = tmp;
		return ;
	}
	first_tmp = (*first);
	while (first_tmp->next != *ants)
		first_tmp = first_tmp->next;
	first_tmp->next = tmp;
	free(*ants);
	*ants = tmp;
}
