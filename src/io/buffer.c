/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:47:39 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/26 18:06:23 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	contains_nan(char buffer[], uint size)
{
	uint	i;

	i = 0;
	while (i < size && buffer[i])
	{
		if (!ft_isdigit(buffer[i]) && !ft_isspace(buffer[i]) \
				&& buffer[i] != '.')
		{
			return (true);
		}
		i++;
	}
	return (false);
}

void	buffer_move_next_whitespace(t_buffer *buffer)
{
	if (buffer->start == buffer->end)
	{
		buffer->empty = true;
		return ;
	}
	while (buffer->start != buffer->end \
			&& ft_isspace(buffer->bytes[buffer->start]))
		buffer->start++;
	while (buffer->start != buffer->end \
			&& !ft_isspace(buffer->bytes[buffer->start]))
		buffer->start++;
	while (buffer->start != buffer->end \
			&& ft_isspace(buffer->bytes[buffer->start]))
		buffer->start++;
	if (buffer->start == buffer->end)
		buffer->empty = true;
}

void	buffer_allignment_set(t_buffer *buffer)
{
	if (buffer->start == buffer->end)
	{
		buffer->empty = true;
		return ;
	}
	while (!ft_isspace(buffer->bytes[buffer->end]) \
			&& buffer->end != buffer->start)
		buffer->end--;
	while (ft_isspace(buffer->bytes[buffer->start]) \
			&& buffer->end != buffer->start)
		buffer->start++;
	if (buffer->start == buffer->end)
	{
		buffer->end = buffer->size;
		buffer->start = 0;
		buffer->empty = true;
	}
}

void	buffer_read(t_buffer *buffer, int fd)
{
	uint	leftover_bytes;
	uint	bytes_read;

	leftover_bytes = 0;
	if (buffer->end < buffer->size)
	{
		leftover_bytes = buffer->size - buffer->end;
		ft_memmove(buffer->bytes, &buffer->bytes[buffer->end + 1], \
				leftover_bytes);
	}
	bytes_read = ft_read(fd, &buffer->bytes[leftover_bytes], \
			buffer->capacity - leftover_bytes);
	buffer->size = bytes_read + leftover_bytes - (bytes_read > 0);
	buffer->end = buffer->size;
	buffer->start = 0;
	buffer_allignment_set(buffer);
	buffer->empty = (bytes_read == 0 && leftover_bytes == 0);
}

t_buffer	buffer_init(uint capacity, void *bytes)
{
	t_buffer	buffer;

	if (capacity == 0 || bytes == NULL)
		return ((t_buffer){0});
	ft_bzero(bytes, capacity);
	ft_bzero(&buffer, sizeof(buffer));
	buffer.bytes = bytes;
	buffer.capacity = capacity;
	buffer.size = 0;
	buffer.start = 0;
	buffer.end = 0;
	buffer.empty = true;
	return (buffer);
}
