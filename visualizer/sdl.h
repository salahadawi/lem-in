/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 18:53:51 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/02 19:56:04 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_H
# define SDL_H
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define TEXTURES_AMOUNT 1

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include "../libft/includes/libft.h"

typedef	struct		s_file
{
	char 			*line;
	struct s_file	*next;
}					t_file;

typedef	struct		s_texture
{
	SDL_Texture		*texture;
	int				width;
	int				height;
}					t_texture;

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Surface		*image;
	SDL_Event		e;
	int				img_flags;
	SDL_Renderer	*renderer;
	t_texture		*textures[TEXTURES_AMOUNT];
}					t_sdl;

#endif
