/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:17:12 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/29 16:05:13 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

double	ray_distance_from_point_squared(t_vector ray, t_vector point)
{
	double	projection_len;

	projection_len = dot_product(point, ray);
	return ((dot_product(point, point) + SHADOW_BIAS) \
			- projection_len * projection_len);
}

double	smaller_non_negative(double a, double b)
{
	if (a < 0)
	{
		if (b < 0)
			return (-1);
		else
			return (b);
	}
	if (b < 0 || b > a)
		return (a);
	return (b);
}
