/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 23:03:04 by njaber            #+#    #+#             */
/*   Updated: 2018/04/12 14:30:30 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static unsigned int		get_color_at(t_ptr *p, double z)
{
	unsigned int		ret;

	if (z < -20)
		ret = (p->colors[0]);
	else if (z < -10)
		ret = (color_gradiant((unsigned int[2]){p->colors[0],
					p->colors[1]}, (z + 20) / 10));
	else if (z < 0)
		ret = (color_gradiant((unsigned int[2]){p->colors[1],
					p->colors[2]}, (z + 10) / 10));
	else if (z < 10)
		ret = (color_gradiant((unsigned int[2]){p->colors[2],
					p->colors[3]}, z / 10));
	else if (z < 20)
		ret = (color_gradiant((unsigned int[2]){p->colors[3],
					p->colors[4]}, (z - 10) / 10));
	else
		ret = (p->colors[4]);
	return (ret);
}

static void				map_segment(t_ptr *p, t_ivec v1, t_ivec v2)
{
	t_vec3				v[2];
	t_vec2				points[2];
	unsigned int		colors[2];

	v[0] = (t_vec3){v1.x, v1.y, p->map->height[v1.y][v1.x]};
	v[1] = (t_vec3){v2.x, v2.y, p->map->height[v2.y][v2.x]};
	colors[0] = get_color_at(p, v[0].z);
	colors[1] = get_color_at(p, v[1].z);
	v[0] = apply_mat_vec3(v[0], p->transform);
	v[1] = apply_mat_vec3(v[1], p->transform);
	if (!p->is_perspective_active || (v[0].z > p->near && v[1].z > p->near))
	{
		v[0] = apply_mat_vec3(v[0], p->perspective);
		v[1] = apply_mat_vec3(v[1], p->perspective);
		v[0] = apply_mat_vec3(v[0], p->align);
		v[1] = apply_mat_vec3(v[1], p->align);
		points[0] = (t_vec2){v[0].x, v[0].y};
		points[1] = (t_vec2){v[1].x, v[1].y};
		draw_line(&p->win->img, colors, points);
	}
}

void					set_args(t_ptr *p)
{
	clSetKernelArg(p->draw_vbo->cores[0], 7,
			sizeof(int), (int[1]){p->aliasing});
	clSetKernelArg(p->draw_vbo->cores[0], 9,
			sizeof(int), (float[1]){p->use_motion_blur});
	clSetKernelArg(p->draw_vbo->cores[0], 10,
			sizeof(float[16]), p->transform);
	clSetKernelArg(p->draw_vbo->cores[0], 11,
			sizeof(float[16]), p->perspective);
	clSetKernelArg(p->draw_vbo->cores[0], 12,
			sizeof(float[16]), p->align);
	clSetKernelArg(p->draw_vbo->cores[0], 13,
			sizeof(float), (float[1]){p->zoom});
	clSetKernelArg(p->draw_vbo->cores[0], 14,
			sizeof(float), (float[1]){p->fog});
}

void					draw_vbo_opencl(t_ptr *p)
{
	cl_event	event;
	cl_int		err;

	if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
			p->draw_vbo->cores[0], 1, NULL, (size_t[1]){((p->vbo_size /
				p->opencl->gpu_wg_sz) + 1) * p->opencl->gpu_wg_sz},
			(size_t[1]){p->opencl->gpu_wg_sz}, 0, NULL, &event)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du kernel"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	if ((err = clEnqueueReadBuffer(p->opencl->gpu_command_queue,
			p->draw_vbo->memobjs[0], CL_TRUE, 0,
			p->win->img.line * p->win->img.size.y,
			p->win->img.buf, 1, &event, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec durant la lecture du buffer"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
}

void					draw_map(t_ptr *p)
{
	size_t		x;
	size_t		y;

	if (p->draw_vbo != NULL && p->use_opencl)
	{
		set_args(p);
		draw_vbo_opencl(p);
	}
	else
	{
		y = 0;
		while (y < p->map->y && (x = 0) == 0)
		{
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
}
