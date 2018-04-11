/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 17:12:41 by njaber            #+#    #+#             */
/*   Updated: 2018/04/11 16:23:31 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <fcntl.h>

int				main(int argc, char **argv)
{
	int		fd;
	t_ptr	p;

	if (argc < 2)
		ft_error("Usage: ./fdf <map_file>.");
	if (argc > 2)
		ft_error("[Erreur] Trop d'arguments.\n");
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		ft_error("[Erreur] Impossible d'ouvrir le fichier.\n");
	if ((p.win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.\n");
	if ((p.map = (t_map*)ft_memalloc(sizeof(t_map))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.\n");
	init_struct(fd, &p);
	close(fd);
	set_hooks(&p);
	mlx_loop(p.win->mlx);
}
