/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/19 08:04:25 by njaber            #+#    #+#             */
/*   Updated: 2018/04/08 22:20:55 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libgxns/libgxns.h"

typedef struct	s_map {
	size_t	x;
	size_t	y;
	int		**height;
}				t_map;

typedef struct	s_ptr {
	t_map				*map;
	t_win				*win;
	t_vec2				origin_rot;
	t_vec2				rot;
	t_vec2				dest_rot;
	t_vec2				pos;
	t_vec2				dest_pos;
	t_ivec				mouse_origin;
	char				keys[512];
	int					button;
	double				z_size;
	double				zoom;
	double				dest_zoom;
	t_mat4				transform;
	t_mat4				perspective;
	t_mat4				align;
	unsigned int		colors[5];
	char				is_perspective_active;
	char				use_opencl;
	char				aliasing;
	double				far;
	double				near;
	double				fov;
	t_ocl				*opencl;
	t_kernel			*draw_vbo;
	float				*vbo;
	unsigned int		*vbo_idx;
}				t_ptr;

void			init_struct(int fd, t_ptr *p);

void			set_hooks(t_ptr *p);
int				mouse_hook(int button, int x, int y, void *parm);
int				motion_hook(int x, int y, void *parm);
int				loop_hook(void *parm);
int				key_press_hook(int key_code, void *parm);
int				key_release_hook(int key_code, void *parm);
int				button_release_hook(int button, int x, int y, void *parms);
int				button_press_hook(int button, int x, int y, void *parms);

void			key_handler(t_ptr *p);

void			draw_map(t_ptr *p);

void			create_kernel(t_ptr *p);

#endif
