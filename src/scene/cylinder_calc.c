/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_calc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:24:06 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:24:48 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	calc_cyl_data(t_cylinder *cy)
{
	t_vector	axle;

	normalize_vector(&cy->normal);
	axle = scale_vector(cy->normal, cy->height / 2);
	cy->bottom = vector_subtract(cy->center, axle);
	cy->top = add_vector(cy->center, axle);
}

void	cyl_equation_coefficients(t_cylinder *cy, \
									t_vector ray, \
									t_vector cam_delta)
{
	cy->coeffs.x = ray_distance_from_point_squared(cy->normal, ray);
	cy->coeffs.y = 2 * (dot_product(cam_delta, ray) - \
	dot_product(cam_delta, cy->normal) * dot_product(ray, cy->normal));
	cy->coeffs.z = ray_distance_from_point_squared(cy->normal, cam_delta) - \
	pow(cy->radius, 2);
}

double	solve_cyl_equation(t_cylinder *cy, t_vector ray, \
							t_vector cam_delta, int *flip)
{
	double	discriminant;
	double	a2;
	double	t1;
	double	t2;

	cyl_equation_coefficients(cy, ray, cam_delta);
	discriminant = pow(cy->coeffs.y, 2) - 4 * cy->coeffs.x * cy->coeffs.z;
	if (discriminant < 0)
		return (-1);
	discriminant = sqrt(discriminant);
	a2 = 1 / (2 * cy->coeffs.x);
	t2 = (-(cy->coeffs.y) - discriminant) * a2;
	t1 = (-(cy->coeffs.y) + discriminant) * a2;
	if (t1 > 0 && t2 < 0)
		*flip = -1;
	return (smaller_non_negative(t1, t2));
}

double	cyl_hit_distance(t_cylinder *cy, t_vector ray, t_vector cam, int *flip)
{
	double		t;
	t_vector	p;

	t = solve_cyl_equation(cy, ray, vector_subtract(cam, cy->bottom), flip);
	if (t > 0)
	{
		p = add_vector(cam, scale_vector(ray, t));
		cy->hit_h = dot_product(vector_subtract(p, cy->bottom), cy->normal);
		if (cy->hit_h <= cy->height && cy->hit_h >= 0)
			return (t);
	}
	return (-1);
}
