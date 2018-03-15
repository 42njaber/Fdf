/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/15 01:57:02 by njaber            #+#    #+#             */
/*   Updated: 2018/03/15 05:05:20 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

char	*join_free(char *s1, char *s2, size_t src_len)
{
	size_t	dst_len;
	char	*ret;

	dst_len = ft_strlen(s1);
	ret = ft_strnew(dst_len + src_len);
	ft_memcpy(ret, s1, dst_len);
	ft_memcpy(ret + dst_len, s2, src_len);
	free(s1);
	return (ret);
}

cl_program		createProgramFromFile(cl_context context, char *file)
{
	cl_program	ret;
	char		*kernel;
	char		buf[4096];
	int			fd;
	int			rd;

	if ((fd = open(file, O_RDONLY)) < 0)
		ft_error("[Erreur] Impossible d'ouvrir le fichier.");
	rd = read(fd, buf, 4096);
	kernel = ft_strnew(rd);
	ft_memcpy(kernel, buf, rd);
	while ((rd = read(fd, buf, 4096)) > 0) 
		kernel = join_free(kernel, buf, rd);
	if (rd == -1)
		ft_error("[Erreur] Erreur durant la lecture du fichier.");
	ret = clCreateProgramWithSource(context, 1,
			(const char**)&kernel, NULL, NULL);
	return (ret);
}

static void		create_img_cpy_kernel(t_kernel *kernel,
		cl_context gpu_context)
{
	kernel->program = createProgramFromFile(gpu_context,
			"libgxns/img_cpy.cl");
}

t_ocl			*init_opencl(void)
{
	t_ocl	*opencl;

	if ((opencl = (t_ocl*)ft_memalloc(sizeof(t_ocl))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	opencl->gpu_context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU,
			NULL, NULL, NULL);
	clGetContextInfo(opencl->gpu_context, CL_CONTEXT_NUM_DEVICES,
			sizeof(unsigned int), &opencl->gpu_nbr, NULL);
	clGetContextInfo(opencl->gpu_context, CL_CONTEXT_DEVICES,
			sizeof(cl_device_id[16]), opencl->gpus, NULL);
	opencl->gpu_command_queue = clCreateCommandQueue(opencl->gpu_context,
			opencl->gpus[0], 0, NULL);
	create_img_cpy_kernel(&opencl->img_cpy, opencl->gpu_context);
	return (opencl);
}
