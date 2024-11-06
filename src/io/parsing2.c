/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:29:43 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 10:32:13 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

int	ppm_read_number(int fd, bool *eof, uint	i)
{
	char	c;
	char	buffer[10];
	int		bytes_read;

	c = ' ';
	ft_bzero(buffer, sizeof(buffer));
	bytes_read = 1;
	while (ft_isspace(c) && bytes_read > 0)
		bytes_read = ft_read(fd, &c, 1);
	while (!ft_isspace(c))
	{
		if (!ft_isdigit(c))
			return (ft_printf("Invalid Character in PPM, Only Digits\n"), -1);
		buffer[i] = c;
		i++;
		if (i == sizeof(buffer))
			return (ft_printf("Invalid Character in PPM, Too Long\n"), -1);
		if (ft_read(fd, &c, 1) == 0)
		{
			if (eof)
				*eof = true;
			break ;
		}
	}
	return (ft_atoi(buffer));
}

bool	ppm_check(int fd, int *width, int *height)
{
	char	unit[10];

	ft_bzero(unit, sizeof(unit));
	ft_read(fd, unit, 3);
	if (ft_strncmp(unit, "P3\n", 3) != 0)
		return (ft_printf("Invalid PPM format, only P3 is accepted\n" \
					"Correct = P3 \nWIDTH HEIGHT\nRANGE[0..255]\nR G B"), \
					false);
	*width = ppm_read_number(fd, NULL, 0);
	*height = ppm_read_number(fd, NULL, 0);
	if (ppm_read_number(fd, NULL, 0) != 255)
		return (ft_printf("Invalid PPM format, out of RGB range\n"), false);
	return (true);
}

void	ppm_pixels_read(int fd, t_texture *texture)
{
	bool	eof;
	int		i;
	int		r;
	int		g;
	int		b;

	eof = 0;
	i = 0;
	while (i < texture->width * texture->height && !eof)
	{
		r = ppm_read_number(fd, &eof, 0);
		g = ppm_read_number(fd, &eof, 0);
		b = ppm_read_number(fd, &eof, 0);
		texture->pixel[i++] = set_color(r, g, b);
	}
}

t_texture	ppm_image_read(const char *path)
{
	int			fd;
	t_texture	texture;
	int			width;
	int			height;

	ft_open(&fd, path, O_RDONLY, 0644);
	ppm_check(fd, &width, &height);
	texture = (t_texture){0};
	if (width <= 0 || height <= 0)
		return (texture);
	texture.pixel = ft_calloc(width * height, sizeof(*texture.pixel));
	texture.width = width;
	texture.height = height;
	lst_memory(texture.pixel, free, ADD);
	ppm_pixels_read(fd, &texture);
	ft_close(fd);
	return (texture);
}

void	err(char *body_type, uint line)
{
	ft_fprintf(STDERR_FILENO, "Error\n"\
	"Invalid %s configuration occured at line: %d\n", body_type, line);
}
