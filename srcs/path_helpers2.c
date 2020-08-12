/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helpers2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 18:57:26 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 18:57:53 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	get_flow_paths(t_farm **farm, int mode)
{
	static int		parent_num = 1;

	if (mode != 2)
		while (get_flow_path(farm, NULL, NULL, parent_num++))
			;
	else
		while (get_flow_path3(farm, NULL, NULL, parent_num++))
			;
}

void	remove_flow2(t_room *room1, t_room *room2)
{
	t_link	*tmp1;
	t_link	*tmp2;

	tmp1 = room1->links;
	while (tmp1->room != room2)
		tmp1 = tmp1->next;
	tmp1->flow = 0;
	tmp2 = room2->links;
	while (tmp2->room != room1)
		tmp2 = tmp2->next;
	tmp2->flow = 0;
}

void	remove_paths_flow(t_path *path)
{
	t_path *tmp_path;
	t_link *tmp_link;

	tmp_path = path;
	while (tmp_path)
	{
		tmp_link = tmp_path->path;
		while (tmp_link)
		{
			if (tmp_link->next)
				remove_flow2(tmp_link->room, tmp_link->next->room);
			tmp_link->room->flow = 0;
			tmp_link = tmp_link->next;
		}
		tmp_path = tmp_path->next;
	}
}

int		free_queue_and_increment(t_link *queue, int *i, int *j)
{
	(*i)++;
	(*j) = 0;
	free_queue(queue);
	return (0);
}

t_link	*get_cur_and_links(t_link *queue, t_link **links)
{
	t_link *cur;

	cur = queue;
	while (cur->next)
		cur = cur->next;
	*links = cur->room->links;
	return (cur);
}
