/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 18:53:51 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/04 14:01:21 by sadawi           ###   ########.fr       */
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
# define BUFF_SIZE 128

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <string.h>
# include <unistd.h>

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

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

typedef struct	s_data
{
	int			amount;
	char		*output;

}				t_data;

typedef struct		s_fd
{
	char			*str;
	int				fd;
	struct s_fd		*next;
}					t_fd;

extern "C" {
	size_t ft_strlen(const char *s);

	int ft_isalpha(int c);

	int ft_isdigit(int c);

	int ft_strncmp(const char *s1, const char *s2, size_t n);

	int ft_atoi(char const *str);

	char *ft_strcpy(char *dst, const char *src);

	char *ft_strncpy(char *dst, const char *src, size_t len);

	char *ft_strcat(char *s1, const char *s2);

	char *ft_strncat(char *s1, const char *s2, size_t n);

	size_t ft_strlcat(char *s1, const char *s2, size_t n);

	char *ft_strchr(const char *s, int c);

	char *ft_strrchr(const char *s, int c);

	char *ft_strstr(char const *haystack, char const *needle);

	char *ft_strnstr(const char *haystack, const char *needle,
					 size_t len);

	int ft_isalpha(int c);

	int ft_isascii(int c);

	int ft_isdigit(int c);

	int ft_isalnum(int c);

	int ft_isprint(int c);

	int ft_toupper(int c);

	int ft_tolower(int c);

	void *ft_memset(void *b, int c, size_t len);

	void ft_bzero(void *s, size_t n);

	void *ft_memcpy(void *dst, const void *src, size_t n);

	void *ft_memccpy(void *dst, const void *src, int c, size_t n);

	void *ft_memmove(void *dst, const void *src, size_t len);

	void *ft_memchr(const void *s, int c, size_t n);

	int ft_memcmp(const void *s1, const void *s2, size_t n);

	char *ft_strdup(const char *s1);

	int ft_strcmp(const char *s1, const char *s2);

	void *ft_memalloc(size_t size);

	void ft_memdel(void **ap);

	char *ft_strnew(size_t size);

	void ft_strdel(char **as);

	void ft_strclr(char *s);

	void ft_striter(char *s, void (*f)(char *));

	void ft_striteri(char *s, void (*f)(unsigned int, char *));

	char *ft_strmap(char const *s, char (*f)(char));

	char *ft_strmapi(char const *s, char (*f)(unsigned int, char));

	int ft_strequ(char const *s1, char const *s2);

	int ft_strnequ(char const *s1, char const *s2, size_t n);

	char *ft_strsub(char const *s, unsigned int start, size_t len);

	char *ft_strjoin(char const *s1, char const *s2);

	char *ft_strtrim(char const *s);

	char **ft_strsplit(char const *s, char c);

	char *ft_itoa(int n);

	void ft_putchar(char c);

	void ft_putstr(const char *s);

	void ft_putendl(const char *s);

	void ft_putnbr(int n);

	void ft_putchar_fd(char c, int fd);

	void ft_putstr_fd(const char *s, int fd);

	void ft_putendl_fd(const char *s, int fd);

	void ft_putnbr_fd(int n, int fd);

	void ft_lstdelone(t_list **alst, void (*del)(void *, size_t));

	void ft_lstdel(t_list **alst, void (*del)(void *, size_t));

	void ft_lstiter(t_list *lst, void (*f)(t_list *elem));

	t_list *ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

	t_list *ft_lstnew(void const *content, size_t content_size);

	void ft_strfill(char *str, char c);

	int ft_lstevery(t_list *lst, int (*f)(t_list *elem));

	void ft_putlst(t_list *lst);

	t_list *ft_strsplitlst(const char *str, char c);

	void ft_swap(int *a, int *b);

	void ft_foreach(int *tab, int length, void (*f)(int));

	int ft_longlen(long int nbr);

	void ft_putlong(long int n);

	void ft_putulong(unsigned long int n);

	char *ft_itoa_base(long n, int base);

	char *ft_itoa_base_low(long n, int base);

	char *ft_itoa_base_ul(unsigned long n, int base);

	char *ft_itoa_base_ul_low(unsigned long n, int base);

	void ft_putdouble(long double d, int precision);

	char *ft_itoa_double(long double d, int precision);

	char *ft_chartostr(char c);

	long ft_atoilong(const char *str);

	char *ft_strjoinfree(char *s1, char *s2);

	int ft_isdigit_neg(char *str);

	char *ft_strcatfree(char *s1, const char *s2);

	int				ft_printf(const char *format, ...);

	int ft_fprintf(int fd, const char *format, ...);

	char *ft_sprintf(const char *format, ...);

	int handle_flags(const char **format, va_list *args, t_data *data);

	int check_flag(char *flag);

	int print_flag(char *flag, va_list *args, t_data *data);

	int handle_signed(char *flag, va_list *args, t_data *data);

	int handle_unsigned(char *flag, va_list *args, t_data *data);

	int handle_pointer(char *flag, va_list *args, t_data *data);

	int handle_base(char *flag, va_list *args, t_data *data);

	int handle_float(char *flag, va_list *args, t_data *data);

	int handle_percent(char *flag, t_data *data);

	char *toaddress(unsigned long n);

	int handle_output(char **output, char *flag, t_data *data);

	int handle_char_output(char **output, char *flag, t_data *data);

	void handle_precision(char **output, char *flag);

	void handle_hashtag(char **output, char *flag);

	void handle_plus(char **output, char *flag);

	void handle_space(char **output, char *flag);

	void handle_flag(char **output, char *flag);

	void handle_width(char **output, char *flag);

	int handle_char_width(char **output, char *flag, int width);

	void handle_zero(char **output, char *flag);

	void add_width(char **output, char *flag, char *padding);

	int flag_integer(char *flag);

	int check_flag_thrice(char *flag, char *s1, char *s2, char *s3);

	void handle_char_minus(char **output, char *padding);

	void free_two(char **s1, char **s2);

	void handle_hex_precision(char **output, char *flag);

	t_data *join_data(t_data *data1, t_data *data2);

	void init_data(t_data **data);

	char *join_char_to_str(char *str, char c);

	int		get_next_line(const int fd, char **line);
}

#endif
