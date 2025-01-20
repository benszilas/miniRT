/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:42:35 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/29 12:34:24 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

double	dot_product(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vector	get_normal(t_vector from, t_vector to)
{
	t_vector	n;

	n = vector_subtract(from, to);
	normalize_vector(&n);
	return (n);
}

void	normalize_vector(t_vector *vec)
{
	float	len;

	len = vector_length(*vec);
	if (len)
		*vec = scale_vector(*vec, 1 / len);
}

t_vector	vector_subtract(t_vector vec1, t_vector vec2)
{
	return ((t_vector){vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z});
}

void	vector_print(t_vector vector, int fd)
{
	ft_fprintf(fd, "%f", vector.x);
	ft_putchar_fd(',', fd);
	ft_fprintf(fd, "%f", vector.y);
	ft_putchar_fd(',', fd);
	ft_fprintf(fd, "%f", vector.z);
}
