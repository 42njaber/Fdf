/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 20:23:49 by njaber            #+#    #+#             */
/*   Updated: 2018/04/09 20:04:43 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		generate_vbo(t_ptr *p)
{
	size_t		i;

	if ((p->vbo = (float*)ft_memalloc(p->map->x * p->map->y *
			sizeof(float[4]))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	i = 0;
	while (i < p->map->x * p->map->y)
	{
		p->vbo[i * 4] = i % p->map->x;
		p->vbo[i * 4 + 1] = i / p->map->x;
		p->vbo[i * 4 + 2] = p->map->height[i / p->map->x][i % p->map->x];
		p->vbo[i * 4 + 3] = 1.0;
		i++;
	}
}

static void		generate_idx(t_ptr *p)
{
	size_t		i;
	size_t		tmp;

	if ((p->vbo_idx = (unsigned int*)ft_memalloc(((p->map->x - 1) * p->map->y +
			p->map->x * (p->map->y - 1)) * sizeof(unsigned int[2]))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	i = 0;
	while (i < (p->map->x - 1) * p->map->y + p->map->x * (p->map->y - 1))
	{
		tmp = i / (p->map->x * 2 - 1) * p->map->x;
		p->vbo_idx[i * 2] = tmp + (i + p->map->x) % (p->map->x * 2 - 1)
			% p->map->x;
		p->vbo_idx[i * 2 + 1] = tmp + ((i + p->map->x) % (p->map->x * 2 - 1)
				% p->map->x) + (i % (p->map->x * 2 - 1) < p->map->x - 1 ?
					1 : p->map->x);
		i++;
	}
}

static void		create_memobjs(t_ptr *p, t_ocl *opencl,
		t_kernel *kernel, t_img *img)
{
	generate_vbo(p);
	generate_idx(p);
	if ((kernel->memobjs = (cl_mem*)ft_memalloc(sizeof(cl_mem) * 8)) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	kernel->memobjs[0] = clCreateBuffer(opencl->gpu_context, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR, img->line * img->size.y, img->buf, NULL);
	kernel->memobjs[1] = clCreateBuffer(opencl->gpu_context,
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			p->map->x * p->map->y * sizeof(float[4]), p->vbo, NULL);
	kernel->memobjs[2] = clCreateBuffer(opencl->gpu_context,
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			((p->map->x - 1) * p->map->y + p->map->x * (p->map->y - 1))
			* sizeof(unsigned int[2]), p->vbo_idx, NULL);
	clSetKernelArg(kernel->cores[0], 0,
			sizeof(cl_mem), (void*)&kernel->memobjs[0]);
	clSetKernelArg(kernel->cores[0], 1,
			sizeof(cl_mem), (void*)&kernel->memobjs[1]);
	clSetKernelArg(kernel->cores[0], 2,
			sizeof(cl_mem), (void*)&kernel->memobjs[2]);
}

static int		build_program(t_ocl *opencl, t_kernel *kernel)
{
	char		*tmp;
	size_t		tmp2;
	int			err;

	kernel->program = create_program_from_file(opencl->gpu_context,
			"libgxns/kernel.cl");
	err = clBuildProgram(kernel->program, 0, NULL,
					"-cl-unsafe-math-optimizations", NULL, NULL);
	tmp = (char*)ft_memalloc(4096);
	clGetProgramBuildInfo(kernel->program, opencl->gpus[0],
			CL_PROGRAM_BUILD_LOG, 4096, tmp, &tmp2);
	ft_printf("Build log :\n%.*s\n", tmp2, tmp);
	free(tmp);
	if (err != CL_SUCCESS)
		return (err);
	kernel->cores[0] = clCreateKernel(kernel->program, "draw_vbo", &err);
	if (err != CL_SUCCESS)
		return (err);
	kernel->cores[1] = clCreateKernel(kernel->program, "clear_buf", &err);
	return (err);
}

void			create_kernel(t_ptr *p)
{
	t_kernel	*kernel;
	t_img		*img;
	t_ocl		*opencl;
	int			err;

	if ((kernel = (t_kernel*)ft_memalloc(sizeof(t_kernel))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	img = &p->win->img;
	opencl = p->opencl;
	kernel->opencl = opencl;
	if ((err = build_program(opencl, kernel)) != CL_SUCCESS)
	{
		ft_printf("[Error] Could not build kernel program"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
		free(kernel);
		return ;
	}
	clSetKernelArg(kernel->cores[0], 3, sizeof(unsigned int), &img->px_size);
	clSetKernelArg(kernel->cores[0], 4, sizeof(unsigned int), &img->line);
	clSetKernelArg(kernel->cores[0], 5,
			sizeof(int[2]), (int[2]){img->size.x, img->size.y});
	clSetKernelArg(kernel->cores[0], 7,
			sizeof(int[1]), (int[1]){p->is_perspective_active});
	p->draw_vbo = kernel;
	create_memobjs(p, opencl, kernel, img);
}
