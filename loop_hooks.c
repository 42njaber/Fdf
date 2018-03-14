/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 17:04:31 by njaber            #+#    #+#             */
/*   Updated: 2018/03/14 01:44:58 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		map_segment(t_ptr *p, t_ivec v1, t_ivec v2)
{
	t_vec4	v[2];
	t_vec2	points[2];

	v[0] = (t_vec4){v1.x, v1.y, p->map->height[v1.y][v1.x], 1.0};
	v[1] = (t_vec4){v2.x, v2.y, p->map->height[v2.y][v2.x], 1.0};
	v[0] = apply_mat_vec3(v[0], p->transform);
	v[1] = apply_mat_vec3(v[1], p->transform);
	//v[0] = apply_mat_vec3(v[0], p->perspective);
	//v[1] = apply_mat_vec3(v[1], p->perspective);
	//v[0] = (t_vec4){v[0].x / v[0].z, v[0].y / v[0].z, v[0].z / v[0].z, 1};
	//v[1] = (t_vec4){v[1].x / v[1].z, v[1].y / v[1].z, v[1].z / v[1].z, 1};
	points[0] = (t_vec2){v[0].x, v[0].y};
	points[1] = (t_vec2){v[1].x, v[1].y};
	draw_line(&p->win->img, 0xFF00FF, points);
}


static void		draw_map(t_ptr *p)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < p->map->y)
	{
		x = 0;
		while (x < p->map->x)
		{
			if (x < p->map->x - 1)
				map_segment(p, (t_ivec){x, y}, (t_ivec){x + 1, y});
			if (y < p->map->y - 1)
				map_segment(p, (t_ivec){x, y}, (t_ivec){x, y + 1});
			x++;
		}
		y++;
	}
}

static void		print_data(t_ptr *p)
{
	display_data_vec(p->win, "Rotation", (t_vec2){p->rot.x, 
			fmod(fmod(p->rot.y, 360) + 360, 360)}, 30);
	display_data_float(p->win, "Zoom", p->zoom, 50);
	display_data_vec(p->win, "Positon", p->pos, 70);
	display_data_float(p->win, "Z Size", p->z_size * 10, 90);
}

void			transform_handler(t_ptr *p)
{
	double	s;

	p->rot = (t_vec2){(p->rot.x + (p->dest_rot.x * 0.05)) / 1.05,
		(p->rot.y + (p->dest_rot.y * 0.05)) / 1.05};
	p->pos = (t_vec2){(p->pos.x + (p->dest_pos.x * 0.05)) / 1.05,
		(p->pos.y + (p->dest_pos.y * 0.05)) / 1.05};
	p->zoom = (p->zoom + (p->dest_zoom * 0.08)) / 1.08;
	identity(p->transform);
	scale(p->transform, (t_vec4){1, 1, p->z_size, 1});
	translate(p->transform, (t_vec4){(p->map->x - 1) / -2.0, (p->map->y - 1) / -2.0, 0.0, 0.0});
	translate(p->transform, (t_vec4){p->pos.x, p->pos.y, 0.0, 0.0});
	rotate(p->transform, (t_vec4){0, 0, p->rot.y, 0});
	rotate(p->transform, (t_vec4){p->rot.x, 0, 0, 0});
	scale(p->transform, (t_vec4){p->zoom, p->zoom, p->zoom, 1.0});
	translate(p->transform, (t_vec4){p->win->size.x / 2, p->win->size.y / 2, 0.0, 0.0});
	s = 1 / tan(p->fov * 0.5 * M_PI / 180);
	ft_memcpy(p->perspective, (t_mat4){
			s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, -p->far / (p->far - p->near), -1,
			0, 0, -(p->far * p->near) / (p->far - p->near), 0
		}, 
		sizeof(t_mat4));
}

int				loop_hook(void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	key_handler(p);
	transform_handler(p);
	clear_img(&p->win->img);
	draw_map(p);
	paint_window(p->win);
	print_data(p);
	return (0);
}
