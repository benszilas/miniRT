/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_tracing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:18:07 by bszilas           #+#    #+#             */
/*   Updated: 2024/12/05 16:41:31 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

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

double	cone_hit_distance(t_cone *cn, t_vector ray, t_vector cam, int *invert)
{
	double	h[3];
	double	t;

	solve_cone_equation(cn, ray, vector_subtract(cam, cn->vertex));
	h[0] = dot_product(vector_subtract(add_vector(cam, \
	scale_vector(ray, cn->t[0])), cn->vertex), cn->normal);
	h[1] = dot_product(vector_subtract(add_vector(cam, \
	scale_vector(ray, cn->t[1])), cn->vertex), cn->normal);
	if (!finite_cone_hit(cn->height, h[0]))
		cn->t[0] = -1;
	if (!finite_cone_hit(cn->height, h[1]))
		cn->t[1] = -1;
	t = smaller_non_negative(cn->t[0], cn->t[1]);
	if (t == cn->t[0])
		cn->hit_h = h[0];
	else
		cn->hit_h = h[1];
	h[2] = dot_product(vector_subtract(cam, cn->vertex), cn->normal);
	if (finite_cone_hit(cn->height, h[2]) && cam_inside_cone(cn, cam, h[2]))
		*invert = -1;
	return (t);
}

void	trace_cone(t_pixel *pixel, t_vector ray, t_body *body, t_scene *sc)
{
	t_hit_point	hit;
	double		dist;
	t_cone		cn;
	int			invert;

	cn = body->cone;
	invert = 1;
	dist = cone_hit_distance(&cn, ray, sc->camera.position, &invert);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		*pixel->color = body->color;
		pixel->id = body->id;
		pixel->dist = dist;
		hit.p = add_vector(sc->camera.position, scale_vector(ray, dist));
		calc_hit_point_vectors(&hit, ray, \
				cone_surface_normal(cn, hit.p, invert));
		if (body->reflect && sc->depth < MAX_DEPTH)
			trace_reflection(pixel, hit, *sc);
		else
			trace_lights(sc, pixel, hit);
	}
	trace_cone_bottom(pixel, ray, body, sc);
}

void	trace_cone_bottom(t_pixel *px, t_vector ray, t_body *cone, t_scene *sc)
{
	t_body	bottom;

	ft_bzero(&bottom, sizeof (bottom));
	bottom.id = cone->id;
	bottom.color = cone->color;
	bottom.disk = cone->cone.bottom_cap;
	trace_disk(px, ray, &bottom, sc);
}
