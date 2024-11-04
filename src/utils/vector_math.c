/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:36:20 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/03 20:23:51 by bszilas          ###   ########.fr       */
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

t_vector	rot_x(t_vector vec, int dir)
{
	return ((t_vector){vec.x, vec.y * COS_10 - vec.z * SIN_10 * dir, \
	vec.y * SIN_10 * dir + vec.z * COS_10});
}

t_vector	rot_y(t_vector vec, int dir)
{
	return ((t_vector){vec.x * COS_10 + vec.z * SIN_10 * dir, vec.y, \
	vec.x * dir * -SIN_10 + vec.z * COS_10});
}

t_vector	rot_z(t_vector vec, int dir)
{
	return ((t_vector){vec.x * COS_10 - vec.y * SIN_10 * dir, \
	vec.x * SIN_10 * dir + vec.y * COS_10, vec.z});
}

t_vector	cross_product(t_vector a, t_vector b)
{ 
	return ((t_vector){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, \
	a.x * b.y - a.y * b.x});
}

t_vector	reflect_vector(t_vector incoming, t_vector axis)
{
	return (vector_subtract(incoming, \
	scale_vector(axis, 2 * dot_product(axis, incoming))));
}

void	calc_hit_point_vectors(t_hit_point *hit, t_vector ray, t_vector n)
{
	hit->n = n;
	hit->v = scale_vector(ray, -1);
	hit->r = reflect_vector(ray, n);
}
