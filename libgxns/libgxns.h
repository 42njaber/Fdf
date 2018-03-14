/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgxns.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 23:56:43 by njaber            #+#    #+#             */
/*   Updated: 2018/03/14 00:32:48 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGXNS_H
# define LIBGXNS_H

#include <math.h>
#include "/opt/X11/include/X11/X.h"
#include "minilibx_macos/mlx.h"
#include "ft_printf/ft_printf.h"
#include "libft/libft.h"
#include "gnl/get_next_line.h"

typedef struct	s_vec2 {
	double	x;
	double	y;
}				t_vec2;

typedef struct	s_vec4 {
	double	x;
	double	y;
	double	z;
	double	w;
}				t_vec4;

typedef double t_mat4[16];

typedef struct	s_ivec {
	int		x;
	int		y;
}				t_ivec;

typedef struct	s_img {
	void	*link;
	char	*buf;
	char	*sample_buf;
	t_ivec	size;
	int		samples;
	int		px_size;
	int		line;
	int		endian;
}				t_img;

typedef struct	s_win {
	void		*mlx;
	void		*win;
	t_img		img;
	t_ivec		size;
	int			frame;
	double		fps;
	uint64_t	frames[30];
}				t_win;

t_vec4	apply_mat_vec3(t_vec4 v, t_mat4 m);
void	print_mat(t_mat4 m);

void	identity(t_mat4 m);
void	translate(t_mat4 m, t_vec4 v);
void	scale(t_mat4 m, t_vec4 v);
void	rotate(t_mat4 m, t_vec4 v);
void	multiply(t_mat4 m1, t_mat4 m2);

int		init_new_win(t_win *win, t_ivec size, char *title);
void	paint_window(t_win *win);

void	display_data_float(t_win *win, char *name, double data, int y);
void	display_data_vec(t_win *win, char *name, t_vec2 data, int y);

void	init_new_image(void *mlx, t_img *img, t_ivec size, int samples);
void	img_px(t_img *img, int color, t_ivec pixel, int sample);
void	combine_samples(t_img *img);
void	clear_img(t_img *img);

void	draw_line(t_img *img, int color, t_vec2 *verticies);

#endif
