/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 17:12:41 by njaber            #+#    #+#             */
/*   Updated: 2018/03/13 00:56:02 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <fcntl.h>

int		main(int argc, char **argv)
{
	int		fd;
	t_ptr	p;

	if (argc < 2)
		ft_error("Usage: ./fdf <map_file>.");
	if (argc > 2)
		ft_error("[Erreur] Trop d'arguments.");
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		ft_error("[Erreur] Impossible d'ouvrir le fichier.");
	if (((p.map = (t_map*)ft_memalloc(sizeof(t_map))) == NULL && !(p.win = NULL)) ||
		(p.win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	init_map(fd, &p);
	close(fd);
	if ((init_new_win(p.win, (t_ivec){800, 800}, "FdF")) == 0)
		ft_error("[Erreur] Echec de l'intialization de la fenètre");
		set_hooks(&p);
	mlx_loop(p.win->mlx);
}
