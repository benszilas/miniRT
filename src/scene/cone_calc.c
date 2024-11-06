/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_calc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:00:57 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:13:13 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	calc_cone_data(t_cone *cn)
{
	t_disk	*bottom;

	bottom = &cn->bottom_cap;
	normalize_vector(&cn->normal);
	cn->tan_a2p1 = pow(cn->radius / cn->height, 2) + 1;
	bottom->point = add_vector(cn->vertex, \
	scale_vector(cn->normal, cn->height));
	bottom->normal = cn->normal;
	bottom->inverse_normal = scale_vector(cn->normal, -1);
	bottom->radius = cn->radius;
}

void	cone_equation_coefficients(t_cone *cn, t_vector ray, \
t_vector cam_delta)
{
	cn->coeffs.x = dot_product(ray, ray) - cn->tan_a2p1 * \
	pow(dot_product(ray, cn->normal), 2);
	cn->coeffs.y = 2 * (dot_product(cam_delta, ray) - cn->tan_a2p1 * \
	dot_product(cam_delta, cn->normal) * dot_product(ray, cn->normal));
	cn->coeffs.z = dot_product(cam_delta, cam_delta) - cn->tan_a2p1 * \
	pow(dot_product(cam_delta, cn->normal), 2);
}

bool	finite_cone_hit(double cone_height, double h)
{
	return (h >= 0 && h <= cone_height);
}

bool	cam_inside_cone(t_cone *cn, t_vector cam, double cam_h)
{
	t_vector	p;
	double		r;

	p = add_vector(cn->vertex, scale_vector(cn->normal, cam_h));
	r = cn->radius * (cam_h / cn->height);
	return (vector_length(vector_subtract(p, cam)) < r);
}

void	solve_cone_equation(t_cone *cn, t_vector ray, t_vector cam_delta)
{
	double	discriminant;
	double	a2;

	cone_equation_coefficients(cn, ray, cam_delta);
	cn->t[0] = -1;
	cn->t[1] = -1;
	discriminant = pow(cn->coeffs.y, 2) - 4 * cn->coeffs.x * cn->coeffs.z;
	if (discriminant < 0)
		return ;
	discriminant = sqrt(discriminant);
	a2 = 1 / (2 * cn->coeffs.x);
	cn->t[0] = (-(cn->coeffs.y) - discriminant) * a2;
	cn->t[1] = (-(cn->coeffs.y) + discriminant) * a2;
}
