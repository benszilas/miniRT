/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:45:43 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/17 13:36:39 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

#define BUMP_BUFFER_SIZE 10000

static void	bump_map_create(t_bump_map *map, t_buffer *buffer, int fd)
{
	uint	buffer_ptr;

	buffer_ptr = 0;
	while (buffer->empty == false)
	{
		while (buffer->empty == false)
		{
			map->map[buffer_ptr++] = ft_atoi(&buffer->bytes[buffer->start]);
			buffer_move_next_whitespace(buffer);
		}
		buffer_read(buffer, fd);
		if (contains_nan(buffer->bytes, buffer->end) \
				|| buffer_ptr - 1 > map->width * map->height)
			return (ft_fprintf(2, "Invalid Bump Map Format\n"), \
					close(fd), lst_memory(NULL, NULL, FAIL));
		buffer_allignment_set(buffer);
	}
	if (map->width * map->height != buffer_ptr)
	{
		close(fd);
		ft_fprintf(STDERR_FILENO, \
				"Invalid Bump Map format for %s\n", map->path);
		lst_memory(NULL, NULL, FAIL);
	}
}

void	bump_map_read(char *map_path, t_bump_map *map)
{
	int			fd;
	t_buffer	buffer;
	char		stack_buffer[BUMP_BUFFER_SIZE];

	if (map->map)
		lst_memory(map->map, NULL, FREE);
	ft_bzero(map, sizeof(*map));
	ft_open(&fd, map_path, O_RDONLY, 0644);
	ft_memcpy(map->path, map_path, ft_strlen(map_path));
	ft_bzero(stack_buffer, sizeof(stack_buffer));
	buffer = buffer_init(BUMP_BUFFER_SIZE, stack_buffer);
	buffer_read(&buffer, fd);
	if (contains_nan(buffer.bytes, buffer.end))
	{
		ft_fprintf(2, "Invalid Bump Map Format\n");
		return (close(fd), lst_memory(NULL, NULL, FAIL));
	}
	map->width = ft_atoi(&buffer.bytes[buffer.start]);
	buffer_move_next_whitespace(&buffer);
	map->height = ft_atoi(&buffer.bytes[buffer.start]);
	buffer_move_next_whitespace(&buffer);
	map->map = ft_calloc(map->width * map->height, sizeof(double));
	lst_memory(map->map, free, ADD);
	bump_map_create(map, &buffer, fd);
	ft_close(fd);
}

double	bilinear_sample(double u, double v, t_bump_map *map)
{
	int		x2;
	int		y2;
	double	f0;
	double	f1;

	u = u * (map->width - 1);
	v = v * (map->height - 1);
	x2 = (int)u;
	y2 = (int)v;
	if ((int)u + 1 < (int)map->width)
		x2++;
	if ((int)v + 1 < (int)map->height)
		y2++;
	f0 = (double)(map->map[(int)v * map->width + (int)u] * 0.5) \
			* (1 - (u - (int)u)) + map->map[(int)v * map->width + x2] \
			* (u - (int)u);
	f1 = map->map[y2 * map->width + (int)u] \
		* (1 - (u - (int)u)) + (double)(map->map[y2 * map->width + x2] * 0.5) \
			* (u - (int)u);
	return (f0 * (1 - (v - (int)v)) + f1 * (v - (int)v));
}

void	perturb_normal(t_vector *normal, t_bump_map *map, double u, double v)
{
	double	h00;
	double	h01;
	double	h10;
	double	dhdu;
	double	dhdv;

	if (u < 0)
		u += 1.0;
	if (v < 0)
		v += 1.0;
	h00 = bilinear_sample(u, v, map);
	h01 = bilinear_sample(u + 1.0 / map->width, v, map);
	h10 = bilinear_sample(u, v + 1.0 / map->height, map);
	dhdu = -(h01 - h00);
	dhdv = -(h10 - h00);
	*normal = (t_vector){\
		normal->x + 1 * dhdu + 0 * dhdv, \
		normal->y + 0 * dhdu + 1 * dhdv, \
		normal->z + 0 * dhdu + 0 * dhdv \
	};
	normalize_vector(normal);
}
