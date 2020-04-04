
#include "sdl.h"

void	handle_error(char *message)
{
	ft_printf("Visualizer: %s\n", message);
	exit(0);
}

void	handle_error_sdl(char *message)
{
	ft_printf("Visualizer: %s SDL_ERROR %s\n", message, SDL_GetError());
	exit(0);
}

void init_texture(t_texture **texture)
{
	*texture = (t_texture*)ft_memalloc(sizeof(t_texture));
	(*texture)->texture = NULL;
	(*texture)->width = 0;
	(*texture)->height = 0;
}

void	init_mouse(t_mouse **mouse)
{
	if (!(*mouse = (t_mouse*)ft_memalloc(sizeof(t_mouse))))
		handle_error("Malloc failed");
	(*mouse)->mouse1 = 0;
	(*mouse)->mouse2 = 0;
	(*mouse)->x = 0;
	(*mouse)->y = 0;
}

void	init_mods(t_mods **mods)
{
	if (!(*mods = (t_mods*)ft_memalloc(sizeof(t_mods))))
		handle_error("Malloc failed");
	(*mods)->zoom = 0.8;
	(*mods)->offset_x = SCREEN_WIDTH / 2;
	(*mods)->offset_y = SCREEN_HEIGHT / 2;
}

void	init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		handle_error_sdl("SDL could not initialize!");
	sdl->window = SDL_CreateWindow("Lem-in", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error_sdl("Window could not be created!");
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		handle_error_sdl("Renderer could not be created!");
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error_sdl("SDL image could not initialize!");
	for (int i = 0; i < TEXTURES_AMOUNT; i++)
		init_texture(&sdl->textures[i]);
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	init_mods(&sdl->mods);
	init_mouse(&sdl->mouse);
}

void	free_texture(t_texture *texture)
{	
	if (texture->texture)
	{
		SDL_DestroyTexture(texture->texture);
		texture->texture = NULL;
		texture->width = 0;
		texture->height = 0;
	}
}

void	texture_load_from_file(t_sdl *sdl, t_texture *texture, char *path)
{
	SDL_Surface *loaded_surface;
	SDL_Texture	*new_texture;

	free_texture(texture);
	if (!(loaded_surface = IMG_Load(path)))
		handle_error_sdl("Unable to load image!");
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
	if (!(new_texture = SDL_CreateTextureFromSurface(sdl->renderer, loaded_surface)))
		handle_error_sdl("Unable to create texture from surface!");
	texture->texture = new_texture;
	texture->width = loaded_surface->w;
	texture->height = loaded_surface->h;
	SDL_FreeSurface(loaded_surface);
}

void	render_texture(t_sdl *sdl, t_texture *texture, int x, int y)
{
	SDL_Rect render_quad = {x, y, texture->width, texture->height};
	SDL_RenderCopy(sdl->renderer, texture->texture, NULL, &render_quad);
}

void	load_media(t_sdl *sdl)
{
	(void)sdl;
	//texture_load_from_file(sdl, sdl->textures[0], "arrow.png");
}

void	close_sdl(t_sdl *sdl)
{
	for (int i = 0; i < TEXTURES_AMOUNT; i++)
		free_texture(sdl->textures[i]);
	SDL_DestroyRenderer(sdl->renderer);
	sdl->renderer = NULL;
	SDL_FreeSurface(sdl->image);
	sdl->image = NULL;
	SDL_DestroyWindow(sdl->window);
	sdl->window = NULL;
	free(sdl);
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

t_file	*file_new(char *line)
{
	t_file *file;

	if (!(file = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Malloc failed.");
	file->line = line;
	file->next = NULL;
	return (file);
}

int		check_line_room(char *line)
{
	int i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	if (line[i++] != ' ')
		return (0);
	if (ft_atoilong(&line[i]) > 2147483647)
		return (0);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (line[i++] != ' ')
		return (0);
	if (ft_atoilong(&line[i]) > 2147483647)
		return (0);
	if (line[i] == '-')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	if (!line[i])
		return (1);
	return (0);
}

void	free_links(char *link1, char *link2)
{
	free(link1);
	free(link2);
}

t_link	*new_link(t_room *room)
{
	t_link *link;

	if (!(link = (t_link*)ft_memalloc(sizeof(t_link))))
		handle_error("Malloc failed.");
	link->room = room;
	link->next = NULL;
	return (link);
}

t_room	*new_room(void)
{
	t_room *room;

	room = (t_room*)ft_memalloc(sizeof(t_room));
	room->name = NULL;
	room->x = 0;
	room->y = 0;
	room->next = NULL;
	room->links = NULL;
	return (room);
}

void	update_min_max(t_lem_in *lem_in, t_room *room)
{
	if (room->x > lem_in->x_max)
		lem_in->x_max = room->x;
	if (room->x < lem_in->x_min)
		lem_in->x_min = room->x;
	if (room->y > lem_in->y_max)
		lem_in->y_max = room->y;
	if (room->y < lem_in->y_min)
		lem_in->y_min = room->y;
}

void	get_room_name(t_room **room, char *line, int *i)
{
	while (line[*i] != ' ' && line[*i])
		(*i)++;
	(*i)++;
	(*room)->name = ft_strsub(line, 0, *i - 1);
}

void	get_room_xy(t_room **room, char *line, int *i)
{
	(*room)->x = ft_atoi(&line[*i]);
	if (line[*i] == '-')
		(*i)++;
	while (ft_isdigit(line[*i]))
		(*i)++;
	(*i)++;
	(*room)->y = ft_atoi(&line[*i]);
}

void	get_line_room(t_lem_in *lem_in, t_room **room, char *line)
{
	int i;

	i = 0;
	*room = new_room();
	get_room_name(room, line, &i);
	get_room_xy(room, line, &i);
	if (!lem_in->first)
		lem_in->first = (*room);
}

int		check_line_link(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i++] == '-')
			return (1);
	}
	return (0);
}

int		find_first_room_by_names(t_room **room, char *name1, char *name2)
{
	while (!ft_strequ((*room)->name, name1) && !ft_strequ((*room)->name, name2))
	{
		*room = (*room)->next;
		if (!*room)
			handle_error("Room specified in links not found.");
	}
	if (ft_strequ((*room)->name, name1))
		return (1);
	else
		return (2);	
}

void	find_room_by_name(t_room **room, char *name)
{
	if (ft_strequ((*room)->name, name))
		handle_error("Links contain a room linking to itself.");
	while (!ft_strequ((*room)->name, name))
	{
		*room = (*room)->next;
		if (!*room)
			handle_error("Room specified in links not found.");
	}
}

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
		while (link2->next) // function to cycle link until end
			link2 = link2->next;
		link2->next = new_link(*room1);
	}
	if (!(*room1)->links)
		(*room1)->links = link1;
	if (!(*room2)->links)
		(*room2)->links = link2;
}

void	save_links_to_rooms(t_lem_in *lem_in, char *line)
{
	t_room	*room1;
	t_room	*room2;
	int		first_link;
	char	*name1;
	char	*name2;

	get_line_link(line, &name1, &name2);
	room1 = lem_in->first;
	first_link = find_first_room_by_names(&room1, name1, name2);
	room2 = room1;
	if (first_link == 1)
		find_room_by_name(&room2, name2);
	else
		find_room_by_name(&room2, name1);
	link_rooms(&room1, &room2);
	free_links(name1, name2);
}

t_file	*save_input(t_lem_in *lem_in)
{
	char *line;
	t_file *file;
	t_file *first;
	t_room	*room;


	if (get_next_line(0, &line) < 1)
		handle_error("Error in lem-in");
	file = file_new(line);
	first = file;
	room = NULL;
	while (get_next_line(0, &line) > 0)
	{
		if (line[0] == 'L')
			break;
		if (check_line_room(line))
		{
			if (!room)
				get_line_room(lem_in, &room, line);
			else
			{
				get_line_room(lem_in, &room->next, line);
				room = room->next;
			}
			update_min_max(lem_in, room);
		}
		else if (check_line_link(line))
		{
			save_links_to_rooms(lem_in, line);
		}
		file->next = file_new(line);
		file = file->next;
	}
	return (first);
}

void	print_file(t_file *file)
{
	while (file)
	{
		ft_printf("%s\n", file->line);
		file = file->next;
	}
}

void	print_lem_in(t_lem_in *lem_in)
{
	t_room *room;

	ft_printf("Min xy: (%d.%d)\nMax xy (%d.%d)\n", lem_in->x_min, lem_in->y_min,
	lem_in->x_max, lem_in->y_max);
	room = lem_in->first;
	while (room)
	{
		ft_printf("Room name: %s. x,y: (%d.%d) Scaled: (%d.%d)\n", room->name, room->x, room->y,
		room->x_scaled, room->y_scaled);
		room = room->next;
	}
}

t_lem_in	*init_lem_in(void)
{
	t_lem_in *lem_in;

	if (!(lem_in = (t_lem_in*)ft_memalloc(sizeof(t_lem_in))))
		handle_error("Malloc failed");
	lem_in->map = NULL;
	lem_in->file = NULL;
	lem_in->x_min = MAX_INT;
	lem_in->y_min = MAX_INT;
	lem_in->x_max = MIN_INT;
	lem_in->y_max = MIN_INT;
	lem_in->first = NULL;
	return (lem_in);
}

int		scale(int n, int old[2], int new[2])
{
	int result;

	result = (new[1] - new[0]) * (n - old[0]) / (old[1] - old[0]) + new[0];
	return (result);
}

void	scale_rooms(t_lem_in *lem_in)
{
	/*for (t_room *room = lem_in->first; room; room = room->next)
	{
		room->x_scaled = scale(room->x, (int[2]){lem_in->x_min, lem_in->x_max},
		(int[2]){0, SCREEN_WIDTH});
		room->y_scaled = scale(room->y, (int[2]){lem_in->y_min, lem_in->y_max},
		(int[2]){0, SCREEN_HEIGHT});
	}*/
	for (t_room *room = lem_in->first; room; room = room->next)
	{
		room->x_scaled = room->x - lem_in->x_max / 2;
		room->x_scaled = scale(room->x_scaled, (int[2]){lem_in->x_min, lem_in->x_max},
		(int[2]){0, SCREEN_WIDTH});
		room->y_scaled = room->y - lem_in->y_max / 2;
		room->y_scaled = scale(room->y_scaled, (int[2]){lem_in->y_min, lem_in->y_max},
		(int[2]){0, SCREEN_HEIGHT});
	}
}

void	split_list(t_room *head, t_room **first_half, t_room **second_half)
{
	t_room *fast;
	t_room *slow;

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

t_room	*sorted_merge_room_x(t_room *first_half, t_room *second_half)
{
	t_room *sorted;

	if (!first_half)
		return (second_half);
	if (!second_half)
		return (first_half);
	if (first_half->x <= second_half->x)
	{
		sorted = first_half;
		sorted->next = sorted_merge_room_x(first_half->next, second_half);
	}
	else
	{
		sorted = second_half;
		sorted->next = sorted_merge_room_x(first_half, second_half->next);
	}
	return (sorted);
}

t_room	*sorted_merge_room_y(t_room *first_half, t_room *second_half)
{
	t_room *sorted;

	if (!first_half)
		return (second_half);
	if (!second_half)
		return (first_half);
	if (first_half->y <= second_half->y)
	{
		sorted = first_half;
		sorted->next = sorted_merge_room_y(first_half->next, second_half);
	}
	else
	{
		sorted = second_half;
		sorted->next = sorted_merge_room_y(first_half, second_half->next);
	}
	return (sorted);
}

t_room	*sorted_merge(t_room *first_half, t_room *second_half, int sorting_mode)
{
	if (sorting_mode == SORT_ROOM_X)
		return (sorted_merge_room_x(first_half, second_half));
	if (sorting_mode == SORT_ROOM_Y)
		return (sorted_merge_room_y(first_half, second_half));
	else
		handle_error("Invalid sort mode");
	return NULL;
}

void	mergesort(t_room **room, int sorting_mode)
{
	t_room *head;
	t_room *first_half;
	t_room *second_half;

	head = (*room);
	if (!head || !head->next)
		return ;
	split_list(head, &first_half, &second_half);
	for (t_room *print = first_half; print; print = print->next)
		ft_printf("FIRST: %s\n", print->name);
	for (t_room *print = second_half; print; print = print->next)
		ft_printf("SECOND: %s\n", print->name);
	mergesort(&first_half, sorting_mode);
	mergesort(&second_half, sorting_mode);
	*room = sorted_merge(first_half, second_half, sorting_mode);
}

void	normalize_distances(t_lem_in *lem_in)
{
	int i;

	mergesort(&lem_in->first, SORT_ROOM_X);
	for (t_room *print = lem_in->first; print; print = print->next)
		ft_printf("SORTED_X: %s (%d.%d)\n", print->name, print->x, print->y);
	i = 0;
	for  (t_room *room = lem_in->first; room; room = room->next)
	{
		if (room->next && room->next->x != room->x)
		{
			room->x = i;
			i++;
		}
		else
			room->x = i;
	}
	for (t_room *print = lem_in->first; print; print = print->next)
		ft_printf("SORTED_X: %s (%d.%d)\n", print->name, print->x, print->y);
	i = 0;
	mergesort(&lem_in->first, SORT_ROOM_Y);
	for (t_room *print = lem_in->first; print; print = print->next)
		ft_printf("SORTED_Y: %s (%d.%d)\n", print->name, print->x, print->y);
	i = 0;
	for  (t_room *room = lem_in->first; room; room = room->next)
	{
		if (room->next && room->next->y != room->y)
		{
			room->y = i;
			i++;
		}
		else
			room->y = i;
	}
	//change room coordinates to be next to each other
}

int	main(int argc, char **argv)
{
	t_sdl	*sdl;
	t_lem_in	*lem_in;

	(void)argc;
	(void)argv;
	lem_in = init_lem_in();
	if (!(sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		handle_error("Malloc failed");
	init(sdl);
	load_media(sdl);
	lem_in->file = save_input(lem_in);
	normalize_distances(lem_in);
	scale_rooms(lem_in);
	//print_file(lem_in->file);
	print_lem_in(lem_in);
	while (1)
	{
		while (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
			else if (sdl->e.type == SDL_MOUSEWHEEL)
			{
				if (sdl->e.wheel.y > 0)
					sdl->mods->zoom += 0.05;
				else if (sdl->e.wheel.y < 0)
					sdl->mods->zoom -= 0.05;
			}
			if (sdl->e.type == SDL_MOUSEBUTTONDOWN)
					sdl->mouse->mouse1 = 1;
			if (sdl->e.type == SDL_MOUSEBUTTONUP)
					sdl->mouse->mouse1 = 0;
			if (sdl->e.type == SDL_MOUSEMOTION)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (sdl->mouse->mouse1)
				{
					if (x != sdl->mouse->x)
						sdl->mods->offset_x += x - sdl->mouse->x;
					if (y != sdl->mouse->y)
						sdl->mods->offset_y += y - sdl->mouse->y;
				}
				sdl->mouse->x = x;
				sdl->mouse->y = y;
			}
		}

		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdl->renderer);
		for (t_room *room = lem_in->first; room; room = room->next)
		{
			SDL_Rect fillRect = {room->x_scaled * sdl->mods->zoom + sdl->mods->offset_x, room->y_scaled * sdl->mods->zoom + sdl->mods->offset_y, 10, 10};
			SDL_SetRenderDrawColor(sdl->renderer, 0x99, 0x99, 0x99, 0xFF);        
			SDL_RenderFillRect(sdl->renderer, &fillRect);
			SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(sdl->renderer, 0xCC, 0xCC, 0xCC, 0x99);
			for (t_link *link = room->links; link; link = link->next)
			{
				ft_printf("link: %s-%s", room->name, link->room->name);
				ft_printf("link: (%d.%d) (%d.%d))\n", room->x_scaled, room->y_scaled, link->room->x_scaled, link->room->y_scaled);
				SDL_RenderDrawLine(sdl->renderer, room->x_scaled * sdl->mods->zoom + sdl->mods->offset_x, room->y_scaled * sdl->mods->zoom + sdl->mods->offset_y, link->room->x_scaled * sdl->mods->zoom + sdl->mods->offset_x, link->room->y_scaled * sdl->mods->zoom + sdl->mods->offset_y);
			}
			ft_printf("done\n");
		}
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
