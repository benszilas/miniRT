/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:36:20 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 12:57:18 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

t_vector	set_vector(double x, double y, double z)
{
	return ((t_vector){x, y, z});
}

t_vector	scale_vector(t_vector vec, double scale)
{
	return ((t_vector){vec.x * scale, vec.y * scale, vec.z * scale});
}

t_vector	add_vector(t_vector vec, t_vector add)
{
	return ((t_vector){vec.x + add.x, vec.y + add.y, vec.z + add.z});
}

t_vector	cross_product(t_vector a, t_vector b)
{
	return ((t_vector){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, \
	a.x * b.y - a.y * b.x});
}

t_vector	reflect_vector(t_vector v, t_vector n)
{
	return (vector_subtract(v, scale_vector(n, 2 * dot_product(v, n))));
}
