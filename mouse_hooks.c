/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:32:56 by njaber            #+#    #+#             */
/*   Updated: 2018/03/13 23:04:06 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int		button_press_hook(int button, int x, int y, void *parms)
{
	t_ptr	*p;

	p = (t_ptr*)parms;
	if (button == 4 || button == 5)
	{
		p->dest_zoom *= (button == 4) ? 0.96 : 1.04;
		if (p->dest_zoom > 200)
			p->dest_zoom = 200;
		if (p->dest_zoom < 4)
			p->dest_zoom = 4;
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
