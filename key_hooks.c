/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:04:15 by njaber            #+#    #+#             */
/*   Updated: 2018/04/11 15:28:34 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int		key_press_hook(int key_code, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	if (key_code == 53)
		exit(0);
	else if (key_code == 8)
		p->use_opencl = !p->use_opencl;
	else if (key_code == 12)
		p->aliasing = (p->aliasing + 1) % 3;
	else if (key_code == 3)
		p->fog = (int)p->fog % 4 + 1;
	else if (key_code == 41)
		p->use_motion_blur = !p->use_motion_blur;
	else if (key_code == 35)
	{
		p->is_perspective_active = !p->is_perspective_active;
		clSetKernelArg(p->draw_vbo->cores[0], 7, sizeof(int),
				(int[1]){p->is_perspective_active});
	}
	else if (key_code == 37)
		p->win->img.line_draw_mode = !p->win->img.line_draw_mode;
	else
		p->keys[key_code] = 1;
	return (0);
}

int		key_release_hook(int key_code, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	p->keys[key_code] = 0;
	return (0);
}
