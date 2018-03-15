/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 19:55:44 by njaber            #+#    #+#             */
/*   Updated: 2018/03/15 05:17:28 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

double		tint(double d)
{
	return (rint(trunc(d)));
}

static void	draw_line_sample(t_img *img, unsigned int *colors,
		t_vec2 *v, int sample)
{
	t_vec2	unit;
	t_vec2	pixel;
	double	gradiant;

	pixel = (t_vec2){v[0].x, v[0].y};
	if (fabs(v[1].x - v[0].x) > fabs(v[1].y - v[0].y))
		unit = (t_vec2){copysign(1, v[1].x - v[0].x),
			copysign((v[1].y - v[0].y) / (v[1].x - v[0].x), 
				v[1].y - v[0].y)};
	else
		unit = (t_vec2){copysign((v[1].x - v[0].x) /
			(v[1].y - v[0].y), v[1].x - v[0].x),
			copysign(1, v[1].y - v[0].y)};
	while (fabs(pixel.x - v[0].x) < fabs(v[1].x - v[0].x) + 0.01
			&& fabs(pixel.y - v[0].y) < fabs(v[1].y - v[0].y) + 0.01)
	{
		gradiant = unit.x > 0.1 ? (pixel.x - v[0].x) / (v[1].x - v[0].x) :
			(pixel.y - v[0].y) / (v[1].y - v[0].y); 
		img_px(img, color_gradiant(colors, gradiant), 
				(t_ivec){tint(pixel.x), tint(pixel.y)}, sample);
		pixel = (t_vec2){pixel.x + unit.x, pixel.y + unit.y};
	}
}

void	draw_line(t_img *img, unsigned int *colors, t_vec2 *v)
{
	int		sample;
	t_vec2	verticies[2];

	if ((v[0].x < 0 && v[1].x < 0) 
			|| (v[0].x > img->size.x && v[1].x > img->size.x)
			|| (v[0].y < 0 && v[1].y < 0)
				|| (v[0].y > img->size.y && v[1].y > img->size.y))
		return ;
	else if (img->samples == 1)
		draw_line_sample(img, colors, v, 0);
	else
	{
		sample = 0;
		while (++sample <= img->samples)
		{
			verticies[0] = (t_vec2){v[0].x + sin(sample * M_PI / img->samples),
						v[0].y + cos(sample * M_PI / img->samples)};
			verticies[1] = (t_vec2){v[1].x + sin(sample * M_PI / img->samples),
						v[1].y + cos(sample * M_PI / img->samples)};
			draw_line_sample(img, colors, verticies, sample);
		}
		combine_samples(img/*, (t_vec2){ft_min(v[0].x, v[1].x) - 1, 
				ft_min(v[0].x, v[1].x) - 1}, (t_vec2){ft_max(v[0].x, v[1].x) + 1, 
				ft_max(v[0].x, v[1].x) + 1}*/);
	}
}
