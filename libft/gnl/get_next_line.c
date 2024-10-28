/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:20:17 by vvobis            #+#    #+#             */
/*   Updated: 2024/05/27 17:44:59 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*buffer_extend(void *ptr_old, size_t size_new, size_t size_old)
{
	void	*ptr_new;
	void	*ptr_back;
	void	*ptr_old_free;

	ptr_new = g_calloc(size_new + 1, 1);
	if (!ptr_new)
		return (free(ptr_old), ptr_old = NULL, NULL);
	ptr_back = ptr_new;
	ptr_old_free = ptr_old;
	while (size_old--)
		*(char *)ptr_new++ = *(char *)ptr_old++;
	return (free(ptr_old_free), ptr_old_free = NULL, ptr_back);
}

char	*handle_no_nl(char **buf, char **left)
{
	if (left)
	{
		free(*left);
		*left = NULL;
	}
	if (!buf)
		return (NULL);
	if (g_strlen(*buf))
		return (*buf);
	free(*buf);
	*buf = NULL;
	return (NULL);
}

char	*line_extract(char **buf_joined, char **left, size_t line_len)
{
	char	*line;

	line = g_substr(*buf_joined, 0, line_len);
	if (!line)
		return (free(*buf_joined), *buf_joined = NULL, NULL);
	*left = g_substr(*buf_joined, line_len, g_strlen(*buf_joined) - line_len);
	free(*buf_joined);
	*buf_joined = NULL;
	if (!*left)
		return (free(line), line = NULL, NULL);
	return (line);
}

char	*line_handle(char **buf_fetch)
{
	char		*buf_joined;
	size_t		line_len;
	static char	*left;

	if (!buf_fetch)
		return (free(left), NULL);
	buf_joined = g_strjoin(left, *buf_fetch);
	free(*buf_fetch);
	*buf_fetch = NULL;
	free(left);
	left = NULL;
	if (!buf_joined)
		return (NULL);
	line_len = find_newline(buf_joined);
	if (line_len)
		return (line_extract(&buf_joined, &left, line_len));
	return (handle_no_nl(&buf_joined, &left));
}

char	*get_next_line(int fd)
{
	char		*buf;
	ssize_t		bytes_read;
	size_t		buf_size_cur;
	size_t		buf_size_prev;

	if (fd < 0)
		return (line_handle(NULL), NULL);
	buf = g_calloc(sizeof(*buf), BUFFER_SIZE + 1);
	buf_size_prev = 0;
	buf_size_cur = BUFFER_SIZE;
	while (1)
	{
		if (!buf)
			return (line_handle(NULL));
		bytes_read = read(fd, buf + buf_size_prev, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(buf), buf = NULL, line_handle(NULL));
		if (find_newline(buf) || bytes_read == 0)
			break ;
		buf_size_prev = buf_size_cur;
		buf_size_cur += BUFFER_SIZE;
		buf = buffer_extend(buf, buf_size_cur, buf_size_prev);
	}
	return (line_handle(&buf));
}
