/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 18:53:51 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/03 22:27:20 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_H
# define SDL_H
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define TEXTURES_AMOUNT 0
# define MAX_INT 2147483647
# define MIN_INT - 2147483648

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include "../libft/includes/libft.h"

typedef	struct		s_file
{
	char 			*line;
	struct s_file	*next;
}					t_file;

typedef	struct		s_room
{
	char			*name;
	int				x;
	int				y;
	int				x_scaled;
	int				y_scaled;
	struct s_room	*next;
	struct s_link	*links;
}					t_room;

typedef	struct		s_link
{
	t_room			*room;
	struct s_link	*next;
}					t_link;

typedef	struct		s_lem_in
{
	int				x_min;
	int				x_max;
	int				y_min;
	int				y_max;
	t_room			**map;
	t_file			*file;
	t_room			*first;
}					t_lem_in;


typedef	struct		s_texture
{
	SDL_Texture		*texture;
	int				width;
	int				height;
}					t_texture;

typedef struct		s_mouse
{
	int				mouse1;
	int				mouse2;
	int				x;
	int				y;
}					t_mouse;

typedef struct		s_mods
{
	double			zoom;
	int				offset_x;
	int				offset_y;
}					t_mods;

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Surface		*image;
	SDL_Event		e;
	int				img_flags;
	SDL_Renderer	*renderer;
	t_texture		*textures[TEXTURES_AMOUNT];
	t_mods			*mods;
	t_mouse			*mouse;
}					t_sdl;

#endif
