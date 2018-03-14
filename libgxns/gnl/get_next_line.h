/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/20 19:13:53 by njaber            #+#    #+#             */
/*   Updated: 2018/01/27 20:25:18 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFF_SIZE 32

# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>

typedef struct	s_stock {
	int		fd;
	char	buf[BUFF_SIZE];
	size_t	len;
}				t_stock;

int				get_next_line(const int fd, char **line);

#endif
