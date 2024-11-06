/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:17:12 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/05 12:02:48 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

double	ray_distance_from_point_squared(t_vector ray, t_vector point)
{
	double	projection_len;

	projection_len = dot_product(point, ray);
	return ((dot_product(point, point) + 1e-10) \
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

void	calc_object_space(t_vector surface_n, t_vector *right, t_vector *up)
{
	if (fabs(surface_n.y) > 0.9)
	{
		*right = (t_vector){1, 0, 0};
		*up = (t_vector){0, 0, 1};
	}
	else
	{
		*right = cross_product(surface_n, (t_vector){0, 1, 0});
		*up = cross_product(*right, surface_n);
	}
	normalize_vector(right);
	normalize_vector(up);
}

void	apply_shadow_bias(t_vector *p, t_vector normal, double scale)
{
	*p = add_vector(*p, scale_vector(normal, SHADOW_BIAS * scale));
}
