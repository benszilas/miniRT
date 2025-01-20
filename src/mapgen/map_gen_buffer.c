/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 11:07:46 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/17 13:36:09 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h" 

static void	comment_skip(t_buffer *buffer, int infile)
{
	if (buffer->bytes[buffer->start] != '#')
		return ;
	while (buffer->bytes[buffer->start] != '\n')
	{
		if (buffer->start == buffer->end)
			buffer_read(buffer, infile);
		buffer->start++;
	}
	while (ft_isspace(buffer->bytes[buffer->start]) \
			&& buffer->start != buffer->end)
		buffer->start++;
}

uint	buffer_int_extract(t_buffer *buffer, int fd)
{
	uint	color[3];
	int		i;
	double	gray;

	i = 0;
	while (i < 3)
	{
		comment_skip(buffer, fd);
		color[i++] = ft_atoi(&buffer->bytes[buffer->start]);
		buffer_move_next_whitespace(buffer);
		if (buffer->empty == true)
			buffer_read(buffer, fd);
	}
	gray = 0.299 * color[0] + 0.587 * color[1] + 0.114 * color[2];
	return ((uint)round(gray));
}

void	map_setup(t_buffer *buffer, int outfile, int infile, char *bytes)
{
	*buffer = buffer_init(READ_BUFFER_SIZE, bytes);
	buffer_read(buffer, infile);
	if (ft_strncmp("P3", buffer->bytes, 2) != 0)
	{
		close(infile);
		ft_fprintf(STDERR_FILENO, "Invalid PPM Format\n");
		exit (1);
	}
	comment_skip(buffer, infile);
	buffer_move_next_whitespace(buffer);
	comment_skip(buffer, infile);
	ft_fprintf(outfile, "%d\n", ft_atoi(&buffer->bytes[buffer->start]));
	buffer_move_next_whitespace(buffer);
	comment_skip(buffer, infile);
	ft_fprintf(outfile, "%d\n", ft_atoi(&buffer->bytes[buffer->start]));
	buffer_move_next_whitespace(buffer);
	comment_skip(buffer, infile);
	buffer_move_next_whitespace(buffer);
	comment_skip(buffer, infile);
}

void	map_read_int(t_buffer *buffer, int infile, int outfile)
{
	uint	i;

	i = 0;
	if (buffer->start != buffer->end)
	{
		while (!ft_isspace(buffer->bytes[buffer->start + i]) \
				&& buffer->start + i != buffer->end)
		{
			if (!ft_isdigit(buffer->bytes[buffer->start + i++]))
			{
				close(infile);
				close(outfile);
				ft_fprintf(STDERR_FILENO, \
					"Invalid PPM format, found non Digit in String\n");
				exit(EXIT_FAILURE);
			}
		}
		ft_fprintf(outfile, "%d\n", buffer_int_extract(buffer, infile));
	}
	else
		buffer->empty = true;
}

void	map_create_from_ppm(int infile, int outfile)
{
	t_buffer	buffer;
	char		bytes[READ_BUFFER_SIZE];

	map_setup(&buffer, outfile, infile, bytes);
	while (buffer.empty == false)
	{
		while (buffer.empty == false)
			map_read_int(&buffer, infile, outfile);
		buffer_read(&buffer, infile);
		if (contains_nan(buffer.bytes, buffer.end))
		{
			ft_fprintf(STDERR_FILENO, "Invalid PPM Format\n");
			break ;
		}
	}
	close(infile);
	close(outfile);
}
