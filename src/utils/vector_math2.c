/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:02:37 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/26 20:03:58 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

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
