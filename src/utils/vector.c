/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:42:35 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/05 12:58:18 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

double	vector_length(t_vector vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

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
