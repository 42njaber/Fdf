/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 22:42:46 by njaber            #+#    #+#             */
/*   Updated: 2018/03/13 23:51:45 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		move(t_ptr *p)
{
	if (p->keys[123])
	{
		p->dest_pos.x += 20 * cos(p->rot.y / 180 * M_PI) / p->zoom;
		p->dest_pos.y += 20 * -sin(p->rot.y / 180 * M_PI) / p->zoom;
	}
	if (p->keys[124])
	{
		p->dest_pos.x += -20 * cos(p->rot.y / 180 * M_PI) / p->zoom;
		p->dest_pos.y += -20 * -sin(p->rot.y / 180 * M_PI) / p->zoom;
	}
	if (p->keys[125])
	{
		p->dest_pos.x += -20 * sin(p->rot.y / 180 * M_PI) / p->zoom;
		p->dest_pos.y += -20 * cos(p->rot.y / 180 * M_PI) / p->zoom;
	}
	if (p->keys[126])
	{
		p->dest_pos.x += 20 * sin(p->rot.y / 180 * M_PI) / p->zoom;
		p->dest_pos.y += 20 * cos(p->rot.y / 180 * M_PI) / p->zoom;
	}
	if (fabs(p->dest_pos.x) > p->map->x / 2)
		p->dest_pos.x = copysign(p->map->x / 2, p->dest_pos.x);
	if (fabs(p->dest_pos.y) > p->map->y / 2)
		p->dest_pos.y = copysign(p->map->y / 2, p->dest_pos.y);
}

static void		adjust_height(t_ptr *p)
{
	if (p->keys[125])
		p->z_size -= 0.02;
	if (p->keys[126])
		p->z_size += 0.02;
	if (p->z_size < 0.1)
		p->z_size = 0.1;
	if (p->z_size > 2)
		p->z_size = 2;
}

void			key_handler(t_ptr *p)
{
	if (p->keys[257])
		adjust_height(p);
	else
		move(p);
}
