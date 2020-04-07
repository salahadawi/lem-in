/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_links.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/29 13:00:17 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/07 20:45:49 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	get_line_link(char *line, char **link1, char **link2)
{
	int i;

	i = 0;
	while (line[i] != '-' && line[i])
		i++;
	*link1 = ft_strsub(line, 0, i);
	if (line[i++] != '-')
		handle_error("File contains invalid link.");
	if (!line[i])
		handle_error("File contains invalid link.");
	*link2 = ft_strsub(line, i, ft_strlen(&line[i]));
}

void	link_rooms(t_room **room1, t_room **room2)
{
	t_link *link1;
	t_link *link2;

	link1 = (*room1)->links;
	link2 = (*room2)->links;
	if (!link1)
		link1 = new_link(*room2);
	else
	{
		while (link1->next)
			link1 = link1->next;
		link1->next = new_link(*room2);
	}
	if (!link2)
		link2 = new_link(*room1);
	else
	{
		while (link2->next)
			link2 = link2->next;
		link2->next = new_link(*room1);
	}
	if (!(*room1)->links)
		(*room1)->links = link1;
	if (!(*room2)->links)
		(*room2)->links = link2;
}

void	save_links_to_rooms(t_farm **farm, char *line)
{
	t_room	*room1;
	t_room	*room2;
	int		first_link;
	char	*name1;
	char	*name2;

	get_line_link(line, &name1, &name2);
	room1 = (*farm)->first;
	first_link = find_first_room_by_names(&room1, name1, name2);
	room2 = room1;
	if (first_link == 1)
		find_room_by_name(&room2, name2);
	else
		find_room_by_name(&room2, name1);
	link_rooms(&room1, &room2);
	free_links(name1, name2);
}

void	save_links(t_farm **farm, char *line)
{
	save_links_to_rooms(farm, line);
	save_line_file(farm, line);
	while (get_next_line(0, &line))
	{
		if (!check_line_comment(line))
			save_links_to_rooms(farm, line);
		save_line_file(farm, line);
	}
}

void	check_links_valid(t_farm **farm)
{
	if ((*farm)->start->weight == MAX_INT)
		handle_error("No path from start to end found.");
}
