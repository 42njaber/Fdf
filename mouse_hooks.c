/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:32:56 by njaber            #+#    #+#             */
/*   Updated: 2018/04/12 14:32:48 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int		button_press_hook(int button, int x, int y, void *parms)
{
	t_ptr	*p;

	p = (t_ptr*)parms;
	if (button == 4 || button == 5)
	{
		p->dest_zoom *= (button == 5) ? 0.96 : 1.04;
		if (p->dest_zoom > p->max_zoom)
			p->dest_zoom = p->max_zoom;
		if (p->dest_zoom < 2)
			p->dest_zoom = 2;
	}
	else
	{
		p->button = button;
		p->origin_rot = (t_vec2){p->dest_rot.x, p->dest_rot.y};
		p->mouse_origin = (t_ivec){x, y};
	}
	return (0);
}

int		button_release_hook(int button, int x, int y, void *parms)
{
	t_ptr	*p;

	p = (t_ptr*)parms;
	(void)(button + x + y);
	p->button = -1;
	return (0);
}
