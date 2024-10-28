/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:07:36 by vvobis            #+#    #+#             */
/*   Updated: 2024/05/27 17:43:21 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE

#  define BUFFER_SIZE 50

# endif

# define MAX_FD 1024

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdint.h>

char	*get_next_line(int fd);
char	*g_strjoin(char const *s1, char const *s2);
size_t	g_strlen(char const *str);
void	*g_calloc(size_t n, size_t s);
char	*g_substr(char const *s, unsigned int start, size_t len);
char	*line_handle(char **buf_fetch);
size_t	find_newline(char *buf);

#endif
