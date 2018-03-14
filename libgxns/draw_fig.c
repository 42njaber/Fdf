/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/27 19:55:44 by njaber            #+#    #+#             */
/*   Updated: 2018/03/13 23:22:51 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

double	tint(double d)
{
	return (rint(trunc(d)));
}

static void	draw_line_sample(t_img *img, int color,
		t_vec2 *vrtxs, int sample)
{
	t_vec2	unit;
	t_vec2	pixel;

	pixel = (t_vec2){vrtxs[0].x, vrtxs[0].y};
	if (fabs(vrtxs[1].x - vrtxs[0].x) > fabs(vrtxs[1].y - vrtxs[0].y))
		unit = (t_vec2){copysign(1, vrtxs[1].x - vrtxs[0].x),
			copysign((vrtxs[1].y - vrtxs[0].y) / (vrtxs[1].x - vrtxs[0].x), 
				vrtxs[1].y - vrtxs[0].y)};
	else
		unit = (t_vec2){copysign((vrtxs[1].x - vrtxs[0].x) /
			(vrtxs[1].y - vrtxs[0].y), vrtxs[1].x - vrtxs[0].x),
			copysign(1, vrtxs[1].y - vrtxs[0].y)};
	while (fabs(pixel.x - vrtxs[0].x) < fabs(vrtxs[1].x - vrtxs[0].x) + 0.2
			&& fabs(pixel.y - vrtxs[0].y) < fabs(vrtxs[1].y - vrtxs[0].y) + 0.2)
	{
		img_px(img, color, (t_ivec){tint(pixel.x), tint(pixel.y)}, sample);
		if (tint(pixel.x + unit.x) != tint(pixel.x) 
			&& tint(pixel.y + unit.y) != tint(pixel.y))
		{
			if ((fmod(pixel.x, 1) - !signbit(unit.x)) * unit.x * -1 <
				(fmod(pixel.y, 1) - !signbit(unit.y)) * unit.y * -1)
				img_px(img, color, (t_ivec){tint(pixel.x + unit.x), 
						tint(pixel.y)}, sample);
			else
				img_px(img, color, (t_ivec){tint(pixel.x), 
						tint(pixel.y + unit.y)}, sample);
		}
		pixel = (t_vec2){pixel.x + unit.x, pixel.y + unit.y};
	}
}

void	draw_line(t_img *img, int color, t_vec2 *vrtxs)
{
	int		sample;
	t_vec2	verticies[2];

	if ((vrtxs[0].x < 0 || vrtxs[0].y < 0 || vrtxs[0].x > img->size.x
				|| vrtxs[0].y > img->size.y) && (vrtxs[1].x < 0 || vrtxs[1].y < 0 
					|| vrtxs[1].x > img->size.x || vrtxs[1].y > img->size.y))
		return ;
	if (img->samples == 1)
		draw_line_sample(img, color, vrtxs, 0);
	else
	{
		sample = 0;
		while (++sample <= img->samples)
		{
			verticies[0] = (t_vec2){vrtxs[0].x + sin(sample * M_PI / img->samples),
						vrtxs[0].y + cos(sample * M_PI / img->samples)};
			verticies[1] = (t_vec2){vrtxs[1].x + sin(sample * M_PI / img->samples),
						vrtxs[1].y + cos(sample * M_PI / img->samples)};
			draw_line_sample(img, color, verticies, sample);
		}
		combine_samples(img);
	}
}
