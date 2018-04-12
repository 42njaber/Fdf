/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgxns.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 23:56:43 by njaber            #+#    #+#             */
/*   Updated: 2018/04/12 14:27:40 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGXNS_H
# define LIBGXNS_H

# include <time.h>
# include <math.h>
# include <OpenCL/opencl.h>
# include "../minilibx_macos/mlx.h"
# include "../ft_printf/ft_printf.h"
# include "../libft/libft.h"
# include "gnl/get_next_line.h"

typedef struct	s_vec2 {
	double	x;
	double	y;
}				t_vec2;

typedef struct	s_vec3 {
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef float	t_mat4[16];

typedef struct	s_ivec {
	int		x;
	int		y;
}				t_ivec;

typedef struct	s_ocl {
	cl_context			gpu_context;
	size_t				gpu_nbr;
	cl_device_id		gpus[16];
	cl_command_queue	gpu_command_queue;
	size_t				gpu_wg_sz;
}				t_ocl;

typedef struct	s_kernel {
	cl_program		program;
	cl_mem			*memobjs;
	size_t			memobjs_nbr;
	cl_kernel		cores[8];
	t_ocl			*opencl;
}				t_kernel;

typedef struct	s_img {
	void			*link;
	unsigned char	*buf;
	t_ivec			size;
	int				px_size;
	int				line;
	int				endian;
	int				line_draw_mode;
	t_kernel		line_algo;
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

t_vec3			apply_mat_vec3(t_vec3 v, t_mat4 m);
void			print_mat(t_mat4 m);

void			identity(t_mat4 m);
void			translate(t_mat4 m, t_vec3 v);
void			scale(t_mat4 m, t_vec3 v);
void			rotate(t_mat4 m, t_vec3 v);
void			multiply(t_mat4 m1, t_mat4 m2);

int				init_new_win(t_win *win, t_ivec size, char *title);
void			paint_window(t_win *win, t_kernel *opencl_kernel,
												char use_motion_blur);

void			display_data_float(t_win *win, char *name, double data, int y);
void			display_data_vec2(t_win *win, char *name, t_vec2 data, int y);
void			display_data_vec3(t_win *win, char *name, t_vec3 data, int y);
void			display_data_str(t_win *win, char *name, char *data, int y);

t_ocl			*init_opencl(void);

void			init_new_image(t_win *win, t_img *img, t_ivec size);
void			img_px(t_img *img, unsigned int color, t_ivec pixel);
void			clear_img(t_img *img);

void			draw_line(t_img *img, unsigned int *color, t_vec2 *verticies);
unsigned int	color_gradiant(unsigned int *colors, double gradiant);

cl_program		create_program_from_file(cl_context context, char *file);

#endif
