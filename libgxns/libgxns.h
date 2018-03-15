/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgxns.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 23:56:43 by njaber            #+#    #+#             */
/*   Updated: 2018/03/15 05:16:53 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGXNS_H
# define LIBGXNS_H

#include <math.h>
#include <OpenCL/opencl.h>
#include "X.h"
#include "minilibx_macos/mlx.h"
#include "ft_printf/ft_printf.h"
#include "libft/libft.h"
#include "gnl/get_next_line.h"

typedef struct	s_vec2 {
	double	x;
	double	y;
}				t_vec2;

typedef struct	s_vec3 {
	double	x;
	double	y;
	double	z;
}				t_vec3;

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

typedef struct	s_kernel {
	cl_program		program;
	cl_mem			*memobjs;
	size_t			memobjs_nbr;
}				t_kernel;

typedef struct	s_ocl {
	cl_context			gpu_context;
	size_t				gpu_nbr;
	cl_device_id		gpus[16];
	cl_command_queue	gpu_command_queue;
	t_kernel			img_cpy;
}				t_ocl;

typedef struct	s_win {
	void		*mlx;
	void		*win;
	t_img		img;
	t_ivec		size;
	int			frame;
	double		fps;
	uint64_t	frames[30];
	t_ocl		*ocl;
}				t_win;

t_vec3			apply_mat_vec3(t_vec3 v, t_mat4 m);
void			print_mat(t_mat4 m);

void			identity(t_mat4 m);
void			translate(t_mat4 m, t_vec3 v);
void			scale(t_mat4 m, t_vec3 v);
void			rotate(t_mat4 m, t_vec3 v);
void			multiply(t_mat4 m1, t_mat4 m2);

int				init_new_win(t_win *win, t_ivec size, char *title);
void			paint_window(t_win *win);

void			display_data_float(t_win *win, char *name, double data, int y);
void			display_data_vec(t_win *win, char *name, t_vec2 data, int y);
void			display_data_str(t_win *win, char *name, char *data, int y);

t_ocl			*init_opencl(void);

void			init_new_image(void *mlx, t_img *img, t_ivec size, int samples);
void			img_px(t_img *img, unsigned int color, t_ivec pixel, int sample);
void			combine_samples(t_img *img);
void			clear_img(t_img *img);

void			draw_line(t_img *img, unsigned int *color, t_vec2 *verticies);
unsigned int	color_gradiant(unsigned int *colors, double gradiant);

#endif
