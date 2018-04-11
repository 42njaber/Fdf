/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 17:04:31 by njaber            #+#    #+#             */
/*   Updated: 2018/04/11 14:15:12 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		print_data(t_ptr *p)
{
	display_data_float(p->win, "FPS", p->win->fps, 10);
	display_data_vec2(p->win, "Rotation:", (t_vec2){p->rot.x,
			fmod(fmod(p->rot.y, 360) + 360, 360)}, 30);
	display_data_float(p->win, "Zoom:", p->zoom * 10, 50);
	display_data_vec3(p->win, "Positon:", p->pos, 70);
	display_data_float(p->win, "Z Size:", p->z_size * 10, 90);
	display_data_float(p->win, "Fog:", p->fog * 10, 110);
	display_data_str(p->win, "Mode:", p->is_perspective_active ?
			"Perspective" : "Orthogonal", 130);
	if (p->draw_vbo == NULL)
		display_data_str(p->win, "OpenCL:", "Inactive", 150);
	else
		display_data_str(p->win, "OpenCL:", p->use_opencl ? "ON" : "OFF", 150);
	display_data_float(p->win, "AA:", (float)p->aliasing, 170);
	display_data_str(p->win, "M-Blur:", p->use_motion_blur ? "ON" : "OFF", 190);
}

static void		transform_handler(t_ptr *p)
{
	double	s;

	identity(p->transform);
	translate(p->transform,
			(t_vec3){(p->map->x - 1) / -2.0, (p->map->y - 1) / -2.0, 0.0});
	translate(p->transform, (t_vec3){p->pos.x, p->pos.y, -p->pos.z});
	scale(p->transform, (t_vec3){1, 1, p->z_size});
	rotate(p->transform, (t_vec3){0, 0, p->rot.y});
	rotate(p->transform, (t_vec3){p->rot.x, 0, 0});
	scale(p->transform, (t_vec3){1 * 1000, 1 * 1000, -1});
	translate(p->transform, (t_vec3){0.0, 0.0, p->zoom});
	identity(p->perspective);
	s = 1 / tan(p->fov * 0.5 * M_PI / 180);
	if (p->is_perspective_active)
	{
		ft_memcpy(p->perspective, (t_mat4){s, 0, 0, 0, 0, s, 0, 0, 0, 0,
				-(p->far) / (p->far - p->near), -(p->far * p->near)
				/ (p->far - p->near), 0, 0, 1, 0}, sizeof(t_mat4));
	}
	else
		scale(p->transform, (t_vec3){s / p->zoom, s / p->zoom, 1});
	identity(p->align);
	translate(p->align, (t_vec3){p->win->size.x / 2, p->win->size.y / 2, 0.0});
}

int				loop_hook(void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	key_handler(p);
	p->rot = (t_vec2){(p->rot.x + (p->dest_rot.x * 0.05)) / 1.05,
		(p->rot.y + (p->dest_rot.y * 0.05)) / 1.05};
	p->pos = (t_vec3){(p->pos.x + (p->dest_pos.x * 0.05)) / 1.05,
		(p->pos.y + (p->dest_pos.y * 0.05)) / 1.05,
		(p->pos.z + (p->dest_pos.z * 0.05)) / 1.05};
	p->zoom = (p->zoom + (p->dest_zoom * 0.08)) / 1.08;
	transform_handler(p);
	draw_map(p);
	if (p->use_opencl && p->draw_vbo != NULL)
		clFinish(p->opencl->gpu_command_queue);
	paint_window(p->win,
			p->use_opencl ? p->draw_vbo : NULL, p->use_motion_blur);
	print_data(p);
	return (0);
}
