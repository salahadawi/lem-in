/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mergesort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 16:08:51 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/03 16:09:51 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

t_path	*sorted_merge(t_path *first_half, t_path *second_half)
{
	t_path *sorted;

	if (!first_half)
		return (second_half);
	if (!second_half)
		return (first_half);
	if (first_half->size < second_half->size)
	{
		sorted = first_half;
		sorted->next = sorted_merge(first_half->next, second_half);
	}
	else
	{
		sorted = second_half;
		sorted->next = sorted_merge(first_half, second_half->next);
	}
	return (sorted);
}

void	split_list(t_path *head, t_path **first_half, t_path **second_half)
{
	t_path *fast;
	t_path *slow;

	slow = head;
	fast = head->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*first_half = head;
	*second_half = slow->next;
	slow->next = NULL;
}

void	mergesort_paths(t_path **paths)
{
	t_path	*head;
	t_path	*first_half;
	t_path	*second_half;

	head = *paths;
	if (!head || !head->next)
		return ;
	split_list(head, &first_half, &second_half);
	mergesort_paths(&first_half);
	mergesort_paths(&second_half);
	*paths = sorted_merge(first_half, second_half);
}
