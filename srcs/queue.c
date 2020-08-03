/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:54:30 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/03 15:56:45 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	enqueue(t_link **queue, t_room *new_room)
{
	t_link *new;

	new = new_link(new_room);
	if (!*queue)
		*queue = new;
	else
	{
		new->next = *queue;
		*queue = new;
	}
}

t_room	*dequeue(t_link **queue)
{
	t_link	*current;
	t_room	*tmp;

	current = *queue;
	if (!current->next)
	{
		tmp = current->room;
		free(*queue);
		(*queue) = NULL;
		return (tmp);
	}
	while (current->next->next)
		current = current->next;
	tmp = current->next->room;
	free(current->next);
	current->next = NULL;
	return (tmp);
}

void	init_queue(t_link **queue, t_link **visited, t_farm **farm)
{
	*queue = NULL;
	*visited = NULL;
	(*farm)->start->weight = 0;
	enqueue(queue, (*farm)->start);
}

int		free_queue(t_link *queue)
{
	t_link	*next;

	while (queue)
	{
		next = queue->next;
		free(queue);
		queue = next;
	}
	return (0);
}
