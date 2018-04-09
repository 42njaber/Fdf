/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 00:14:34 by njaber            #+#    #+#             */
/*   Updated: 2018/04/09 19:23:49 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

void	display_data_float(t_win *win, char *name, double data, int y)
{
	char	*tmp;

	mlx_string_put(win->mlx, win->win, 10, y, 0xFFFFFF, name);
	mlx_string_put(win->mlx, win->win, 100, y, 0xFFFFFF,
			(tmp = ft_itoa(rint(data))));
	free(tmp);
}

void	display_data_vec(t_win *win, char *name, t_vec2 data, int y)
{
	char	*tmp;

	mlx_string_put(win->mlx, win->win, 10, y, 0xFFFFFF, name);
	mlx_string_put(win->mlx, win->win, 100, y, 0xFFFFFF,
			(tmp = ft_itoa(rint(data.x))));
	free(tmp);
	mlx_string_put(win->mlx, win->win, 150, y, 0xFFFFFF, ",");
	mlx_string_put(win->mlx, win->win,
			170, y, 0xFFFFFF, (tmp = ft_itoa(rint(data.y))));
	free(tmp);
}

void	display_data_str(t_win *win, char *name, char *data, int y)
{
	mlx_string_put(win->mlx, win->win, 10, y, 0xFFFFFF, name);
	mlx_string_put(win->mlx, win->win, 100, y, 0xFFFFFF, data);
}

void	paint_window(t_win *win, t_kernel *opencl_kernel)
{
	uint64_t	time;

	time = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
	if (++win->frame % 60 == 0)
		win->fps = 300000000000 / (time - win->frames[0]);
	mlx_clear_window(win->mlx, win->win);
	if (opencl_kernel != NULL)
		clFinish(opencl_kernel->opencl->gpu_command_queue);
	mlx_put_image_to_window(win->mlx, win->win, win->img.link, 0, 0);
	if (opencl_kernel != NULL)
	{
		clSetKernelArg(opencl_kernel->cores[1], 1, sizeof(int),
				(int[1]){0x00000000});
		clSetKernelArg(opencl_kernel->cores[1], 0, sizeof(cl_mem),
				&opencl_kernel->memobjs[0]);
		clEnqueueNDRangeKernel(opencl_kernel->opencl-> gpu_command_queue,
				opencl_kernel->cores[1], 1, NULL, (size_t[1])
				{win->img.px_size / 8 * win->img.size.x * win->img.size.y / 4},
				(size_t[1]){128}, 0, NULL, NULL);
		clFinish(opencl_kernel->opencl->gpu_command_queue);
	}
	else
		clear_img(&win->img);
	display_data_float(win, "FPS", win->fps, 10);
	win->frames[win->frame % 30] = time;
}

int		init_new_win(t_win *win, t_ivec size, char *title)
{
	if ((win->mlx = mlx_init()) == NULL ||
		(win->win = mlx_new_window(win->mlx,
			size.x, size.y, title)) == NULL)
		return (0);
	init_new_image(win, &(win->img), size);
	win->size = size;
	win->frame = 0;
	win->fps = 0;
	ft_bzero(win->frames, 30 * sizeof(uint64_t));
	return (1);
}
