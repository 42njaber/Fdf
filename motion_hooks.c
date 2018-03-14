/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motion_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 20:18:52 by njaber            #+#    #+#             */
/*   Updated: 2018/03/14 00:36:11 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int		motion_hook(int x, int y, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	(void)(y + x);
	if (p->button == 1)
	{
		p->dest_rot = (t_vec2){p->origin_rot.x - (y - p->mouse_origin.y),
			p->origin_rot.y - (x - p->mouse_origin.x)};
		if (p->dest_rot.x > 80)
			p->dest_rot.x = 80;
		if (p->dest_rot.x < 0)
			p->dest_rot.x = 0;
	}
	return (0);
}
