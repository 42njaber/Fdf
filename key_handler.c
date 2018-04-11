/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 22:42:46 by njaber            #+#    #+#             */
/*   Updated: 2018/04/11 15:58:40 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		move2(t_ptr *p)
{
	if (p->keys[49])
		p->dest_pos.z += 1;
	if (p->keys[257])
		p->dest_pos.z += -1;
	if (p->dest_pos.z < -20)
		p->dest_pos.z = -20;
	if (p->dest_pos.z > 500)
		p->dest_pos.z = 500;
}

static void		move(t_ptr *p)
{
	if (p->keys[123])
	{
		p->dest_pos.x += 0.01 * cos(p->rot.y / 180 * M_PI) * p->zoom;
		p->dest_pos.y += 0.01 * -sin(p->rot.y / 180 * M_PI) * p->zoom;
	}
	if (p->keys[124])
	{
		p->dest_pos.x += -0.01 * cos(p->rot.y / 180 * M_PI) * p->zoom;
		p->dest_pos.y += -0.01 * -sin(p->rot.y / 180 * M_PI) * p->zoom;
	}
	if (p->keys[125])
	{
		p->dest_pos.x += -0.01 * sin(p->rot.y / 180 * M_PI) * p->zoom;
		p->dest_pos.y += -0.01 * cos(p->rot.y / 180 * M_PI) * p->zoom;
	}
	if (p->keys[126])
	{
		p->dest_pos.x += 0.01 * sin(p->rot.y / 180 * M_PI) * p->zoom;
		p->dest_pos.y += 0.01 * cos(p->rot.y / 180 * M_PI) * p->zoom;
	}
	if (fabs(p->dest_pos.x) > p->map->x / 2)
		p->dest_pos.x = copysign(p->map->x / 2, p->dest_pos.x);
	if (fabs(p->dest_pos.y) > p->map->y / 2)
		p->dest_pos.y = copysign(p->map->y / 2, p->dest_pos.y);
	move2(p);
}

static void		adjust(t_ptr *p)
{
	if (p->keys[125])
		p->z_size /= 1.05;
	if (p->keys[126])
		p->z_size *= 1.05;
	if (p->z_size < 0.01)
		p->z_size = 0.01;
	if (p->z_size > 2)
		p->z_size = 2;
}

void			key_handler(t_ptr *p)
{
	if (p->keys[261])
		adjust(p);
	else
		move(p);
}
