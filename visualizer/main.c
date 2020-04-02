
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

void	init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		handle_error_sdl("SDL could not initialize!");
	sdl->window = SDL_CreateWindow("Lem-in", 700, 200, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error_sdl("Window could not be created!");
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		handle_error_sdl("Renderer could not be created!");
	SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error_sdl("SDL image could not initialize!");
	for (int i = 0; i < TEXTURES_AMOUNT; i++)
		init_texture(&sdl->textures[i]);
	sdl->screen = SDL_GetWindowSurface(sdl->window);
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
	texture_load_from_file(sdl, sdl->textures[0], "arrow.png");
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

t_file	*save_input(void)
{
	char *line;
	t_file *file;
	t_file *first;

	if (get_next_line(0, &line) < 1)
		handle_error("Error in lem-in");
	file = file_new(line);
	first = file;
	while (get_next_line(0, &line) > 0)
	{
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

int	main(int argc, char **argv)
{
	t_sdl *sdl;
	t_file *file;

	(void)argc;
	(void)argv;
	sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl));
	init(sdl);
	load_media(sdl);
	file = save_input();
	print_file(file);
	while (1)
	{
		while (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
		}
		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdl->renderer);
		render_texture(sdl, sdl->textures[0], 0, 0);
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
