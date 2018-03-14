/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 17:34:43 by njaber            #+#    #+#             */
/*   Updated: 2018/03/12 20:03:16 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

t_vec4		apply_mat_vec3(t_vec4 v, t_mat4 m)
{
	t_vec4	ret;

	ret.x = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w;
	ret.y = m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w;
	ret.z = m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w;
	ret.w = m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w;
	return (ret);
}

void		print_mat(t_mat4 m)
{
	int		i;

	i = -1;
	while (++i < 16)
	{
		ft_printf("%5g", m[i]);
		if (i % 4 == 3)
			ft_printf("\n");
		else
			ft_printf(" | ");
	}
}
