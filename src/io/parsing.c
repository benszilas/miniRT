/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:12:18 by victor            #+#    #+#             */
/*   Updated: 2024/10/21 14:08:43 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	collect_param_comma(char **entry_position, char *params[], uint count)
{
	char	*tmp;
	uint	i;

	i = 0;
	tmp = *entry_position;
	params[i] = tmp;
	while (i < count)
	{
		tmp = ft_strchr(tmp, ',');
		if (!tmp)
			return (false);
		*tmp++ = 0;
		i++;
		params[i] = tmp;
	}
	while (*tmp && !ft_isspace((*tmp)))
		tmp++;
	if (*tmp)
		*tmp = 0;
	*entry_position = tmp + 1;
	return (true);
}

bool	collect_param_single(char **entry_position, char *params[])
{
	char	*tmp;

	tmp = *entry_position;
	while (*tmp && ft_isspace(*tmp))
		tmp++;
	if (!*tmp)
		return (false);
	while (*tmp && !ft_isspace(*tmp))
		tmp++;
	*tmp = 0;
	params[0] = *entry_position;
	*entry_position = tmp + 1;
	return (true);
}

uint	id_set(uint group, int id)
{
	return ((group << 24) | (id & 0x00ffffff));
}

uint	id_group_get(int id)
{
	if (id < 0)
		return (-1);
	return ((id >> 24) & 0xff);
}

void	err(char *body_type, uint line)
{
	ft_fprintf(STDERR_FILENO, "Error\n"\
	"Invalid %s configuration occured at line: %d\n", body_type, line);
}

uint	parse_body_color(char *params[], int *error)
{
	return (set_color(set_signed_int(params[0], 0, UCHAR_MAX, error), \
				set_signed_int(params[1], 0, UCHAR_MAX, error), \
				set_signed_int(params[2], 0, UCHAR_MAX, error)));
}

int	ppm_read_number(int fd, bool *eof)
{
	char	c;
	uint	i;
	char	buffer[10];
	int		bytes_read;

	i = 0;
	c = ' ';
	ft_bzero(buffer, sizeof(buffer));
	bytes_read = 1;
	while (ft_isspace(c) && bytes_read > 0)
		bytes_read = ft_read(fd, &c, 1);
	while (!ft_isspace(c))
	{
		if (!ft_isdigit(c))
			return (ft_printf("Invalid Character in PPM, Only Digits Are Allowed\n"), -1);
		buffer[i] = c;
		i++;
		if (i == sizeof(buffer))
			return (ft_printf("Invalid Character in PPM, Too Long\n"), -1);
		if (ft_read(fd, &c, 1) == 0)
		{
			if (eof)
				*eof = true;
			break;
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
	*width = ppm_read_number(fd, NULL);
	*height = ppm_read_number(fd, NULL);
	if (ppm_read_number(fd, NULL) != 255)
		return (ft_printf("Invalid PPM format, only 255 scale is accepted\n"), false);
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
		r = ppm_read_number(fd, &eof);
		g = ppm_read_number(fd, &eof);
		b = ppm_read_number(fd, &eof);
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
