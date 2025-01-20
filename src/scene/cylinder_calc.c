/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_calc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:07:39 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 17:17:15 by bszilas          ###   ########.fr       */
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
							t_vector cam_delta, int *invert)
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
	t2 -= SHADOW_BIAS;
	if (t1 > 0 && t2 < 0)
		*invert = -1;
	return (smaller_non_negative(t1, t2));
}

double	cyl_hit_distance(t_cylinder *cy, t_vector ray, \
							t_vector cam, int *invert)
{
	double		t;
	t_vector	p;

	t = solve_cyl_equation(cy, ray, vector_subtract(cam, cy->bottom), invert);
	if (t > 0)
	{
		p = add_vector(cam, scale_vector(ray, t));
		if (finite_cylinder_hit(cy, p))
			return (t);
	}
	return (-1);
}

void	trace_cyl_caps(t_pixel *px, t_vector ray, t_body *cyl, t_scene *sc)
{
	t_body	cap;

	cap = *cyl;
	cap.disk = (t_disk){.point = cyl->cylinder.top, \
		.normal = cyl->cylinder.normal, .radius = cyl->cylinder.radius, \
			.inverse_normal = scale_vector(cyl->cylinder.normal, -1)};
	trace_disk(px, ray, &cap, sc);
	cap.disk.point = cyl->cylinder.bottom;
	trace_disk(px, ray, &cap, sc);
}
