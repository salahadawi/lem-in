/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 18:53:51 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/03 18:20:00 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_H
# define SDL_H
# define SCREEN_WIDTH 1600
# define SCREEN_HEIGHT 900
# define MAX_INT 2147483647
# define MIN_INT -2147483648
# define START 1
# define END 2

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include "../libft/includes/libft.h"

enum				e_sorting_mode
{
	SORT_ROOM_X,
	SORT_ROOM_Y
};

typedef	struct			s_file
{
	char				*line;
	int					executed;
	struct s_file		*next;
	struct s_file		*prev;
}						t_file;

typedef	struct			s_room
{
	char				*name;
	int					x;
	int					y;
	int					x_scaled;
	int					y_scaled;
	struct s_room		*next;
	struct s_link		*links;
}						t_room;

typedef	struct			s_link
{
	t_room				*room;
	struct s_link		*next;
}						t_link;

typedef	struct			s_texture
{
	SDL_Texture			*texture;
	int					width;
	int					height;
	struct s_texture	*next;
}						t_texture;

typedef struct			s_move
{
	int					x;
	int					y;
	struct s_move		*next;
}						t_move;

typedef	struct			s_ant
{
	int					number;
	t_room				*room;
	int					current_x;
	int					current_y;
	struct s_ant		*next;
	t_texture			*texture;
	t_move				*moves;
}						t_ant;

typedef	struct			s_lem_in
{
	int					x_min;
	int					x_max;
	int					y_min;
	int					y_max;
	t_file				*file;
	t_room				*first;
	t_room				*start;
	t_room				*end;
	int					ants_amount;
	t_ant				*ants;
}						t_lem_in;

typedef struct			s_mouse
{
	int					mouse1;
	int					mouse2;
	int					x;
	int					y;
}						t_mouse;

typedef struct			s_mods
{
	double				zoom;
	int					offset_x;
	int					offset_y;
	int					speed;
}						t_mods;

typedef struct			s_sdl
{
	SDL_Window			*window;
	SDL_Surface			*screen;
	SDL_Surface			*image;
	SDL_Event			e;
	int					img_flags;
	SDL_Renderer		*renderer;
	t_texture			*textures;
	t_mods				*mods;
	t_mouse				*mouse;
	TTF_Font			*font;
}						t_sdl;

#endif
