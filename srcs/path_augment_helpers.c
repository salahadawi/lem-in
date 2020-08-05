/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_augment_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/05 15:07:56 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/05 15:08:51 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	remove_flow(t_room *room1, t_room *room2)
{
	t_link	*tmp1;
	t_link	*tmp2;

	tmp1 = room1->links;
	while (tmp1->room != room2)
		tmp1 = tmp1->next;
	tmp1->flow++;
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = -(tmp1->flow);
}

void	remove_path_flow(t_link *path)
{
	while (path->next)
	{
		remove_flow(path->room, path->next->room);
		path = path->next;
	}
}

void	free_path(t_path *path)
{
	t_link *tmp_link;

	while (path->path)
	{
		tmp_link = path->path->next;
		free(path->path);
		path->path = tmp_link;
	}
	free(path);
}

void	free_link(t_link *link)
{
	t_link *tmp;

	while (link)
	{
		tmp = link->next;
		free(link);
		link = tmp;
	}
}
