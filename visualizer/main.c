
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
	(*mods)->zoom = 1;
	(*mods)->offset_x = SCREEN_WIDTH / 2;
	(*mods)->offset_y = SCREEN_HEIGHT / 2;
	(*mods)->speed = 8;
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
	if (TTF_Init() == -1)
		handle_error("SDL_ttf could not initialize!");
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

void	load_from_rendered_text(t_sdl *sdl, t_texture *texture, char *text, SDL_Color text_color)
{
	SDL_Surface *text_surface;
	
	free_texture(texture);
	if (!(text_surface = TTF_RenderText_Solid(sdl->font, text, text_color)))
		handle_error("Unable to render text surface!");
	if (!(texture->texture = SDL_CreateTextureFromSurface(sdl->renderer, text_surface)))
		handle_error("Unable to create texture from rendered text!");
	texture->width = text_surface->w;
	texture->height = text_surface->h;
	SDL_FreeSurface(text_surface);
	free(text);
}

void	texture_load_from_file(t_sdl *sdl, t_texture *texture, char *path)
{
	SDL_Surface *loaded_surface;
	SDL_Texture	*new_texture;

	free_texture(texture);
	if (!(loaded_surface = IMG_Load(path)))
		handle_error_sdl("Unable to load image!");
	if (!(new_texture = SDL_CreateTextureFromSurface(sdl->renderer, loaded_surface)))
		handle_error_sdl("Unable to create texture from surface!");
	texture->texture = new_texture;
	texture->width = 50;
	texture->height = 50;
	SDL_FreeSurface(loaded_surface);
}

void	render_texture(t_sdl *sdl, t_texture *texture, int x, int y)
{
	SDL_Rect render_quad;
	//check if texture is ant, not text
	if (texture->width == 50)
		render_quad = (SDL_Rect){x, y, texture->width * sdl->mods->zoom, texture->height * sdl->mods->zoom};
	else
		render_quad = (SDL_Rect){x, y, texture->width, texture->height};
	SDL_RenderCopy(sdl->renderer, texture->texture, NULL, &render_quad);
}

t_texture	*new_texture(void)
{
	t_texture *texture;

	if (!(texture = (t_texture*)ft_memalloc(sizeof(t_texture))))
		handle_error("Malloc failed.");
	texture->texture = NULL;
	texture->width = 0;
	texture->height = 0;
	texture->next = NULL;
	return (texture);
}

void	load_media(t_sdl *sdl, t_lem_in *lem_in)
{
	t_texture *texture;

	if (!(sdl->font = TTF_OpenFont("fonts/Action_Man.ttf", 20)))
		handle_error("Failed to load font!");
	SDL_Color text_color = {0, 0, 0, 0};
	texture = NULL;
	for (t_room *room = lem_in->first; room; room = room->next)
	{
		if (!texture)
		{
			texture = new_texture();
			sdl->textures = texture;
		}
		else
			texture->next = new_texture();	
		if (texture->next)
			texture = texture->next;
		if (room == lem_in->start)
			load_from_rendered_text(sdl, texture, ft_strjoin(room->name, " ##start"), text_color);
		else if (room == lem_in->end)
			load_from_rendered_text(sdl, texture, ft_strjoin(room->name, " ##end"), text_color);
		else
			load_from_rendered_text(sdl, texture, ft_strdup(room->name), text_color);
	}
	for (t_ant *ant = lem_in->ants; ant; ant = ant->next)
	{
		ant->texture = new_texture();
		texture_load_from_file(sdl, ant->texture, "ant.png");
	}
}

// Function to free the texture linked list
/*void	free_texture_list(t_texture)
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
}*/

void	close_sdl(t_sdl *sdl)
{
	//free_texture_list
	TTF_CloseFont(sdl->font);
	TTF_Quit();
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

t_file	*file_new(char *line, t_file *prev)
{
	t_file *file;

	if (!(file = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Malloc failed.");
	file->line = line;
	file->executed = 0;
	file->next = NULL;
	file->prev = prev;
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

int		check_line_comment(char *line)
{
	if (line[0] == '#')
		if (!ft_strequ(line, "##start") && !ft_strequ(line, "##end"))
			return (1);
	return (0);
}

int		check_line_command(char *line)
{
	if (ft_strequ(line, "##start"))
		return (START);
	else if (ft_strequ(line, "##end"))
		return (END);
	return (0);
}

void	save_ants_amount(t_lem_in *lem_in)
{
	char *line;

	if (get_next_line(0, &line) != 1)
		handle_error("Error in lem-in");
	while (check_line_comment(line))
	{
		if (get_next_line(0, &line) != 1)
			handle_error("Error in lem-in");
	}
	lem_in->ants_amount = ft_atoi(line);
}

int		save_command_room(t_lem_in *lem_in, t_room **room, char **line, int cmd)
{
	get_next_line(0, line);
	if (*room)
	{
		get_line_room(lem_in, &(*room)->next, *line);
		*room = (*room)->next;
	}
	else
		get_line_room(lem_in, room, *line);
	if (cmd == START)
		lem_in->start = *room;
	else if (cmd == END)
		lem_in->end = *room;
	return (1);
}

char	*save_rooms(t_lem_in *lem_in)
{
	t_room	*room;
	char	*line;
	int		command;

	room = NULL;
	while (get_next_line(0, &line) > 0)
	{
		if (!check_line_comment(line))
		{
			if ((command = check_line_command(line)) > 0)
				save_command_room(lem_in, &room, &line, command);
			else if (check_line_room(line))
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
			else
				break;
		}
		//file->next = file_new(line);
		//file = file->next;
	}
	return (line);
}

char	*save_links(t_lem_in *lem_in, char *line)
{

	if (check_line_link(line))
		save_links_to_rooms(lem_in, line);
	while (get_next_line(0, &line) > 0)
	{
		if (line[0] == 'L')
			return line;
		if (!check_line_comment(line))
		{
			if (check_line_link(line))
				save_links_to_rooms(lem_in, line);
		}
		//file->next = file_new(line);
		//file = file->next;
	}
	return (line);
}

void	save_movements(t_lem_in *lem_in, char *line)
{
	t_file *file;

// adds an extra empty file node to beginning to store first ant backwards move, seems to work pretty well?
	file = file_new(ft_strdup(" "), NULL);
	lem_in->file = file;
	file->next = file_new(line, file);
	file = file->next;
	while (get_next_line(0, &line) > 0)
	{
		file->next = file_new(line, file);
		file = file->next;
	}
}

void	save_input(t_lem_in *lem_in)
{
	char *line;

	save_ants_amount(lem_in);
	line = save_rooms(lem_in);
	line = save_links(lem_in, line);
	save_movements(lem_in, line);
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
	lem_in->start = NULL;
	lem_in->end = NULL;
	lem_in->ants_amount = 0;
	return (lem_in);
}

int		scale(int n, int old[2], int new[2])
{
	int result;
	if (!(old[1] - old[0]))
		old[1]++;
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
		(int[2]){0, SCREEN_WIDTH / 3});
		room->y_scaled = room->y - lem_in->y_max / 2;
		room->y_scaled = scale(room->y_scaled, (int[2]){lem_in->y_min, lem_in->y_max},
		(int[2]){0, SCREEN_HEIGHT / 3});
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
	mergesort(&first_half, sorting_mode);
	mergesort(&second_half, sorting_mode);
	*room = sorted_merge(first_half, second_half, sorting_mode);
}

void	normalize_distances(t_lem_in *lem_in)
{
	int i;

	mergesort(&lem_in->first, SORT_ROOM_X);
	//for (t_room *print = lem_in->first; print; print = print->next)
		//ft_printf("SORTED_X: %s (%d.%d)\n", print->name, print->x, print->y);
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
	lem_in->x_min = 0;
	lem_in->x_max = i;
	i = 0;
	mergesort(&lem_in->first, SORT_ROOM_Y);
	//for (t_room *print = lem_in->first; print; print = print->next)
		//ft_printf("SORTED_Y: %s (%d.%d)\n", print->name, print->x, print->y);
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
	lem_in->y_min = 0;
	lem_in->y_max = i;
	//change room coordinates to be next to each other
}

t_ant	*new_ant(t_lem_in *lem_in, int i)
{
	t_ant	*ant;

	if (!(ant = (t_ant*)ft_memalloc(sizeof(t_ant))))
		handle_error("Malloc failed.");
	ant->number = i;
	ant->room = lem_in->start;
	ant->next = NULL;
	ant->texture = NULL;
	ant->current_x = ant->room->x_scaled;
	ant->current_y = ant->room->y_scaled;
	ant->moves = NULL;
	return (ant);
}

void	create_ants(t_lem_in *lem_in)
{
	t_ant *ant;

	ant = NULL;
	for (int i = 1; i <= lem_in->ants_amount; i++)
	{
		if (!ant)
		{
			ant = new_ant(lem_in, i);
			lem_in->ants = ant;
		}
		else
		{
			ant->next = new_ant(lem_in, i);
			ant = ant->next;
		}
	}
}

t_move	*new_move(void)
{
	t_move *move;

	move = (t_move*)ft_memalloc(sizeof(t_move));
	move->x = 0;
	move->y = 0;
	move->next = NULL;
	return (move);
}

t_move	*save_move(int x, int y)
{
	t_move *move;

	move = new_move();
	move->x = x;
	move->y = y;
	return (move);
}

void	plot_line_low(t_ant *ant, int x1, int y1, int x2, int y2)
{
	int *dxy;
	int *xy;
	int yi;
	int d;
	t_move *move;

	move = NULL;
	dxy = (int[2]){x2 - x1, y2 - y1};
	yi = 1;
	if (dxy[1] < 0)
	{
		yi = -1;
		dxy[1] *= -1;
	}
	d = 2 * dxy[1] - dxy[0];
	xy = (int[2]){x1, y1};
	while (xy[0] <= x2)
	{
		if (!move)
		{
			move = save_move(xy[0], xy[1]);
			ant->moves = move;
		}
		else
		{
			move->next = save_move(xy[0], xy[1]);
			move = move->next;
		}
		if (d > 0)
		{
			xy[1] += yi;
			d -= 2 * dxy[0];
		}
		xy[0]++;
		d += 2 * dxy[1];
	}
}

void	plot_line_high(t_ant *ant, int x1, int y1, int x2, int y2)
{
	int *dxy;
	int *xy;
	int xi;
	int d;
	t_move *move;

	move = NULL;

	dxy = (int[2]){x2 - x1, y2 - y1};
	xi = 1;
	if (dxy[0] < 0)
	{
		xi = -1;
		dxy[0] *= -1;
	}
	d = 2 * dxy[0] - dxy[1];
	xy = (int[2]){x1, y1};
	while (xy[1] <= y2)
	{
		if (!move)
		{
			move = save_move(xy[0], xy[1]);
			ant->moves = move;
		}
		else
		{
			move->next = save_move(xy[0], xy[1]);
			move = move->next;
		}
		if (d > 0)
		{
			xy[0] += xi;
			d -= 2 * dxy[1];
		}
		xy[1]++;
		d += 2 * dxy[0];
	}
}

void	reverse_moves(t_move **moves)
{
	t_move *prev;
	t_move *current;
	t_move *next;

	prev = NULL;
	current = (*moves);
	next = NULL;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*moves = prev;
}

void	plot_line(t_ant *ant, int x1, int y1, int x2, int y2)
{
	if (abs(y2 - y1) < abs(x2 - x1))
	{
		if (x1 > x2)
		{
			plot_line_low(ant, x2, y2, x1, y1);
			reverse_moves(&ant->moves);
		}
		plot_line_low(ant, x1, y1, x2, y2);
	}
	else
	{
		if (y1 > y2)
		{
			plot_line_high(ant, x2, y2, x1, y1);
			reverse_moves(&ant->moves);
		}
		plot_line_high(ant, x1, y1, x2, y2);
	}
}

void	animate_ant_movement(t_lem_in *lem_in, t_file *file)
{
	char **move_arr;
	t_ant *ant;

	if (file->executed)
		return ;
	move_arr = ft_strsplit(file->line, ' ');
	file->executed = 1;
	ant =  NULL;
	//for (int i = 0; move_arr[i]; i++)
	//{
		//ft_printf("%s\n", move_arr[i]);
	//}
	for (int i = 0; move_arr[i]; i++)
	{
		for (t_ant *tmp_ant = lem_in->ants; tmp_ant; tmp_ant = tmp_ant->next)
		{
			//ft_printf("ANT NUMBER %d ANT_NUM %d\n", tmp_ant->number, ft_atoi(&move_arr[i][1]));
			if (tmp_ant->number == ft_atoi(&move_arr[i][1]))
			{
				ant = tmp_ant;
				//ft_printf("CCCC\n");
				break;
			}
		}
		//ft_printf("TESTTEST\n");
		if (!ant)
		{
			ft_printf("TESTAAAA\n");
			break;
		}
		//ft_printf("%sTESTBBBB\n", ant->room->links->next->room->name);
		for (t_link *room_link = ant->room->links; room_link; room_link = room_link->next)
		{
			ft_printf("Room: %s, link: %s\n", room_link->room->name, ft_strchr(move_arr[i], '-') + 1);
			if (ft_strequ(room_link->room->name, ft_strchr(move_arr[i], '-') + 1))
			{
				if (ant->room == lem_in->start)
				{
					if (file->prev)
						file->prev->line = ft_strjoinfree(file->prev->line, ft_sprintf(" L%d-%s", ant->number, lem_in->start->name));
				}
				ant->current_x = ant->room->x_scaled;
				ant->current_y = ant->room->y_scaled;
				ant->room = room_link->room;
				plot_line(ant, ant->current_x, ant->current_y, ant->room->x_scaled, ant->room->y_scaled);
				break;
			}
		}
	}
}

void	move_ant(t_sdl *sdl, t_ant *ant)
{
	if (!ant->moves)
		return ;
	ant->current_x = ant->moves->x;
	ant->current_y = ant->moves->y;
	ant->moves = ant->moves->next;
	for (int i = 0; i < sdl->mods->speed; i++)
	{
		if (!ant->moves)
			break;
		if (ant->moves->next)
			ant->moves = ant->moves->next;
	}
	//need algorith for line movement
	/*if (ant->current_x > ant->room->x_scaled)
		ant->current_x -= 1;
	else if (ant->current_x < ant->room->x_scaled)
		ant->current_x += 1;
	if (ant->current_y > ant->room->y_scaled)
		ant->current_y -= 1;
	else if (ant->current_y < ant->room->y_scaled)
		ant->current_y += 1;*/
}

int	main(int argc, char **argv)
{
	t_sdl	*sdl;
	t_lem_in	*lem_in;

	(void)argc;
	(void)argv;
	chdir("visualizer");
	lem_in = init_lem_in();
	save_input(lem_in);
	normalize_distances(lem_in);
	scale_rooms(lem_in);
	create_ants(lem_in);
	if (!(sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		handle_error("Malloc failed");
	init(sdl);
	load_media(sdl, lem_in);
	print_file(lem_in->file);
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
					if (sdl->mods->zoom > 0.05)
						sdl->mods->zoom -= 0.05;
			}
			else if (sdl->e.type == SDL_KEYDOWN)
			{
				switch(sdl->e.key.keysym.sym)
                {
                	case SDLK_RIGHT:
					if (lem_in->file->next)
					{
						lem_in->file->executed = 0;
						lem_in->file = lem_in->file->next;
					}
					break;
					case SDLK_LEFT:
					if (lem_in->file->prev)
					{
						lem_in->file->executed = 0;
						lem_in->file = lem_in->file->prev;
					}
					break;
					case SDLK_UP:
					if (sdl->mods->speed < 20)
						sdl->mods->speed++;
					break;
					case SDLK_DOWN:
					if (sdl->mods->speed > 0)
						sdl->mods->speed--;
					break;
				}
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
		animate_ant_movement(lem_in, lem_in->file);
		SDL_SetRenderDrawColor(sdl->renderer, 0x77, 0x77, 0x77, 0xFF);
		SDL_RenderClear(sdl->renderer);
		t_texture *texture = sdl->textures;
		for (t_room *room = lem_in->first; room; room = room->next)
		{
			SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(sdl->renderer, 0xCC, 0xCC, 0xCC, 0x99);
			for (t_link *link = room->links; link; link = link->next)
			{
				//ft_printf("link: %s-%s", room->name, link->room->name);
				//ft_printf("link: (%d.%d) (%d.%d))\n", room->x_scaled, room->y_scaled, link->room->x_scaled, link->room->y_scaled);
				SDL_RenderDrawLine(sdl->renderer, room->x_scaled * sdl->mods->zoom + sdl->mods->offset_x + 25 * sdl->mods->zoom, room->y_scaled * sdl->mods->zoom + sdl->mods->offset_y + 25 * sdl->mods->zoom, link->room->x_scaled * sdl->mods->zoom + sdl->mods->offset_x + 25 * sdl->mods->zoom, link->room->y_scaled * sdl->mods->zoom + sdl->mods->offset_y + 25 * sdl->mods->zoom);
			}
		}
		for (t_room *room = lem_in->first; room; room = room->next)
		{
			SDL_Rect fillRect = {room->x_scaled * sdl->mods->zoom + sdl->mods->offset_x, room->y_scaled * sdl->mods->zoom + sdl->mods->offset_y, 50 * sdl->mods->zoom, 50 * sdl->mods->zoom};
			SDL_SetRenderDrawColor(sdl->renderer, 0x99, 0x99, 0x99, 0xFF);
			SDL_RenderFillRect(sdl->renderer, &fillRect);
			render_texture(sdl, texture, room->x_scaled * sdl->mods->zoom + sdl->mods->offset_x, room->y_scaled * sdl->mods->zoom + sdl->mods->offset_y - 20);
			texture = texture->next;
		}
		for (t_ant *ant = lem_in->ants; ant; ant = ant->next)
		{
			move_ant(sdl, ant);
			if (!(ant->current_x == lem_in->end->x_scaled && ant->current_y == lem_in->end->y_scaled))
				render_texture(sdl, ant->texture, ant->current_x * sdl->mods->zoom + sdl->mods->offset_x, ant->current_y * sdl->mods->zoom + sdl->mods->offset_y);
		}
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
