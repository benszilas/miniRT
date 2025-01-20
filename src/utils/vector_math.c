/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:36:20 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/26 20:03:10 by vvobis           ###   ########.fr       */
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

double	vector_length(t_vector vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}
