/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:25:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 14:25:43 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

static void	comment_skip(t_buffer *buffer, int infile)
{
	while (buffer->bytes[buffer->start] != '\n')
	{
		if (buffer->start == buffer->end)
			buffer_read(buffer, infile);
		if (buffer->empty == true)
			return ;
		buffer->start++;
	}
	while (ft_isspace(buffer->bytes[buffer->start]) \
			&& buffer->start != buffer->end)
		buffer->start++;
}

void	ppm_read_loop(t_buffer *buffer, uint color[], \
						int fd, t_texture *texture)
{
	uint		j;

	j = 0;
	while (j < 3)
	{
		if (buffer->bytes[buffer->start] == '#')
			comment_skip(buffer, fd);
		if (buffer->empty == false)
		{
			color[j++] = ft_atoi(&buffer->bytes[buffer->start]);
			buffer_move_next_whitespace(buffer);
		}
		if (buffer->empty == true)
			buffer_read(buffer, fd);
		if (buffer->empty == true && j < 3)
		{
			ft_fprintf(STDERR_FILENO, \
					"Invalide File Format for %s\n", texture->path);
			lst_memory(NULL, NULL, FAIL);
		}
	}
}

void	ppm_pixels_read(t_buffer *buffer, int fd, t_texture *texture)
{
	int			i;
	uint		color[3];

	i = 0;
	while (buffer->empty == false && i < texture->width * texture->height)
	{
		ppm_read_loop(buffer, color, fd, texture);
		texture->pixel[i++] = set_color(color[0], color[1], color[2]);
	}
	if (i != texture->width * texture->height)
	{
		ft_fprintf(STDERR_FILENO, "Invalide File Format for %s", texture->path);
		lst_memory(NULL, NULL, FAIL);
	}
}

void	ppm_header_read(t_buffer *buffer, t_texture *texture, int fd)
{
	uint	limit_value;

	if (ft_strncmp("P3", buffer->bytes, 2) != 0)
		return (ft_fprintf(STDERR_FILENO, "[PPM] Invalid PPM Header!\n"), \
				lst_memory(NULL, NULL, FAIL));
	buffer_move_next_whitespace(buffer);
	if (buffer->bytes[buffer->start] == '#')
		comment_skip(buffer, fd);
	texture->width = ft_atoi(&buffer->bytes[buffer->start]);
	buffer_move_next_whitespace(buffer);
	if (buffer->bytes[buffer->start] == '#')
		comment_skip(buffer, fd);
	texture->height = ft_atoi(&buffer->bytes[buffer->start]);
	buffer_move_next_whitespace(buffer);
	limit_value = ft_atoi(&buffer->bytes[buffer->start]);
	if (limit_value != 255)
		return (ft_fprintf(STDERR_FILENO, "[PPM] Invalid PPM Header: " \
					"Only a Range from 255 is supported!\n"), \
				lst_memory(NULL, NULL, FAIL));
	buffer_move_next_whitespace(buffer);
}

void	ppm_image_read(const char *path, t_texture *texture)
{
	int			fd;
	t_buffer	buffer;
	char		buffer_buffer[1000];

	if (texture->pixel)
		lst_memory(texture->pixel, NULL, FREE);
	buffer = buffer_init(1000, buffer_buffer);
	ft_open(&fd, path, O_RDONLY, 0644);
	ft_memcpy(texture->path, path, ft_strlen(path));
	buffer_read(&buffer, fd);
	ppm_header_read(&buffer, texture, fd);
	texture->pixel = ft_calloc(texture->width * texture->height, \
			sizeof(*texture->pixel));
	lst_memory(texture->pixel, free, ADD);
	ppm_pixels_read(&buffer, fd, texture);
	ft_close(fd);
}
