/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/19 10:10:08 by njaber            #+#    #+#             */
/*   Updated: 2018/03/15 05:16:04 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

void	clear_img(t_img *img)
{
	int		i;

	i = 0;
	while (i < img->line * img->size.y)
		img->buf[i++] = 0;
}

void	combine_samples(t_img *img)
{
	int		i;
	int		len;

	i = 0;
	len = img->line * img->size.y;
	while (i < len)
	{
		img->buf[i] = img->sample_buf[i];
		img->sample_buf[i] = 0;
		i++;
	}
}

void	img_px(t_img *img, unsigned int color, t_ivec pixel, int sample)
{
	int		px_bytes;

	px_bytes = (img->px_size + 7) / 8;
	if (pixel.x < 0 || pixel . y < 0 ||
			pixel.x >= img->size.x || pixel.y >= img->size.y)
		return ;
	if (sample == 0) 
	{
		img->buf[pixel.x * px_bytes + 0 + pixel.y * img->line] = color % 256;
		img->buf[pixel.x * px_bytes + 1 + pixel.y * img->line] = (color / 256) % 256;
		img->buf[pixel.x * px_bytes + 2 + pixel.y * img->line] = (color / 256 / 256) % 256;
		img->buf[pixel.x * px_bytes + 3 + pixel.y * img->line] = (color / 256 / 256 / 256);
	}
	else 
	{
		img->sample_buf[(pixel.x * px_bytes + 0 
			+ pixel.y * img->line)] += (color % 256) / img->samples;
		img->sample_buf[(pixel.x * px_bytes + 1 
			+ pixel.y * img->line)] += ((color / 256) % 256) / img->samples;
		img->sample_buf[(pixel.x * px_bytes + 2 
			+ pixel.y * img->line)] += ((color / 256 / 256) % 256) / img->samples;
		img->sample_buf[(pixel.x * px_bytes + 3 
			+ pixel.y * img->line)] += (color / 256 / 256 / 256) / img->samples;
	}
}

void	init_new_image(void *mlx, t_img *img, t_ivec size, int samples)
{
	img->link = mlx_new_image(mlx, size.x, size.y);
	img->size = size;
	img->buf = mlx_get_data_addr(img->link, &(img->px_size),
			&(img->line), &(img->endian));
	if (samples != 1)
		if ((img->sample_buf = (char*)ft_memalloc(size.y * img->line)) == NULL)
			ft_error("[Erreur] Echec d'allocation mémoire.");
	img->samples = samples;
}
