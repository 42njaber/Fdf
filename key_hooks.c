/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:04:15 by njaber            #+#    #+#             */
/*   Updated: 2018/03/14 03:40:09 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int		key_press_hook(int keyCode, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	if (keyCode == 53)
		exit(0);
	else if (keyCode == 35)
		p->is_perspective_active = !p->is_perspective_active;
	else
	{
		//printf("Key pressed : %03d\n", keyCode);
		p->keys[keyCode] = 1;
	}
	return (0);
}

int		key_release_hook(int keyCode, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	//printf("Key released : %03d\n", keyCode);
	p->keys[keyCode] = 0;
	return (0);
}
