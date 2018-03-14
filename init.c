/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/19 20:17:07 by njaber            #+#    #+#             */
/*   Updated: 2018/03/14 01:21:08 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns/libgxns.h"
#include "fdf.h"

static char		**read_file(int fd, t_map *map)
{
	char	**ret;
	char	*tmp;
	size_t	b_size;
	size_t	i;
	int		rd;

	b_size = 16;
	if ((ret = (char**)ft_memalloc(b_size * sizeof(char*))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire");
	i = 0;
	while ((rd = get_next_line(fd, &tmp)))
	{
		if (i >= b_size && (b_size *= 4))
			ft_realloc((void**)&ret, b_size / 4 * sizeof(char*), b_size * sizeof(char*));
		ret[i] = tmp;
		i++;
	}
	map->y = i;
	return (ret);
}

static size_t	count_col(char *line)
{
	size_t	ret;

	ret = 0;
	while (*line != '\0')
	{
		if ((ret == 0 || ft_isblank(*(line - 1))) && !ft_isblank(*line))
			ret++;
		line++;
	}
	return (ret);
}

static void			get_map(int fd, t_map *map)
{
	char	**buf;
	size_t	i;
	size_t	j;
	size_t	digit;

	buf = read_file(fd, map);
	if ((map->height = (int**)ft_memalloc(sizeof(int*) * map->y)) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire");
	i = 0;
	map->x = 0;
	while (i < map->y)
		if ((j = count_col(buf[i++])) > map->x)
			map->x = j;
	i = 0;
	while (i < map->y && (j = -1) != 0 && (digit = 0) == 0)
	{
		if ((map->height[i] = (int*)ft_memalloc(sizeof(int) * map->x)) == NULL)
			ft_error("[Erreur] Echec d'allocation mémoire");
		while (buf[i][++j] != '\0' && digit < map->x)
		{
			if (!ft_isblank(buf[i][j]) && (j == 0 || ft_isblank(buf[i][j - 1])))
				map->height[i][digit++] = ft_atoi(buf[i] + j);
		}
		i++;
	}
}

void			init_map(int fd, t_ptr *p)
{
	get_map(fd, p->map);
	p->rot = (t_vec2){0, 0};
	p->dest_rot = (t_vec2){80, 45};
	p->z_size = 0.3;
	p->zoom = 50;
	p->dest_zoom = 5;
	p->button = -1;
	p->pos = (t_vec2){0,0};
	p->dest_pos = (t_vec2){0,0};
	p->far = 0.1;
	p->near = 100;
	p->fov = 90;
	ft_bzero(p->keys, 256);
}

void			set_hooks(t_ptr *p)
{
	mlx_loop_hook(p->win->mlx, loop_hook, (void*)p);
	mlx_hook(p->win->win, MotionNotify, 0, motion_hook, (void*)p);
	mlx_hook(p->win->win, ButtonPress, 0, button_press_hook, (void*)p);
	mlx_hook(p->win->win, ButtonRelease, 0, button_release_hook, (void*)p);
	mlx_hook(p->win->win, KeyPress, 0, key_press_hook, (void*)p);
	mlx_hook(p->win->win, KeyRelease, 0, key_release_hook, (void*)p);
}
