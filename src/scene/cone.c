/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 10:54:59 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/03 22:04:32 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	move_cone(int keycode, t_cone *cone)
{
	if (keycode == XK_Up || keycode == XK_Down)
		cone->normal = rot_x(cone->normal, keycode - XK_Up - 1);
	else if (keycode == XK_Left || keycode == XK_Right)
		cone->normal = rot_y(cone->normal, keycode - XK_Left - 1);
	else if (keycode == XK_1 || keycode == XK_3)
		cone->normal = rot_z(cone->normal, keycode - XK_1 - 1);
	else if (!key_move_focused(keycode, &cone->vertex))
		return (false);
	calc_cone_data(cone);
	return (true);
}

bool	parse_cone(char *entry, uint line_count, t_body *body, uint body_count)
{
	char	*params[11];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	if (!collect_param_comma(&entry, params, 2) || !collect_param_comma\
	(&entry, &params[3], 2) || !collect_param_single(&entry, &params[6]) \
	|| !collect_param_single(&entry, &params[7]) || !collect_param_comma\
	(&entry, &params[8], 2))
		return (err("cone", line_count), lst_memory(NULL, NULL, FAIL), false);
	body->type = BODY_CONE;
	body->cone = (t_cone) {.vertex = set_vector(ft_atod(params[0]), \
	ft_atod(params[1]), ft_atod(params[2])), .normal = set_vector(\
	ft_atod(params[3]), ft_atod(params[4]), ft_atod(params[5])),
	.radius = ft_atod(params[6]) / 2, .height = ft_atod(params[7]),
	};
	body->color = parse_body_color(params + 8, &error);
	body->id = id_set(ID_GROUP_CYLINDER, body_count);
	if (error || body->cone.radius <= 0 || body->cone.height <= 0)
		return (err("cone", line_count), lst_memory(NULL, NULL, FAIL), false);
	calc_cone_data(&body->cone);
	return (true);
}

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

void	solve_cone_equation(t_cone *cn, t_vector ray, t_vector cam_delta)
{
	double	discriminant;
	double	a2;

	cone_equation_coefficients(cn, ray, cam_delta);
	cn->t[0] = -1;
	cn->t[1] = -1;
	discriminant = pow(cn->coeffs.y, 2) - 4 * cn->coeffs.x * cn->coeffs.z;
	if (discriminant < 0)
		return;
	discriminant = sqrt(discriminant);
	a2 = 1 / (2 * cn->coeffs.x);
	cn->t[0] = (-(cn->coeffs.y) - discriminant) * a2;
	cn->t[1] = (-(cn->coeffs.y) + discriminant) * a2;
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

double	cone_hit_distance(t_cone *cn, t_vector ray, t_vector cam, int *flip)
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
		*flip = -1;
	return (t);
}

t_vector	cone_surface_normal(t_cone cn, t_vector p, int flip)
{
	return (scale_vector(get_normal(p, add_vector(cn.vertex, \
	scale_vector(cn.normal, cn.hit_h * cn.tan_a2p1))), flip));
}

void	trace_cone(t_pixel *pixel, t_vector ray, t_body *body, t_scene *sc)
{
	t_hit_point	hit;
	double		dist;
	t_cone		cn;
	int			flip;

	cn = body->cone;
	flip = 1;
	dist = cone_hit_distance(&cn, ray, sc->camera.position, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		*pixel->color = body->color;
		hit.p = add_vector(sc->camera.position, scale_vector(ray, dist));
		calc_hit_point_vectors(&hit, ray, cone_surface_normal(cn, hit.p, flip));
		if (body->reflect && sc->depth < MAX_DEPTH)
			trace_reflection(pixel, hit, *sc);
		else
			trace_lights(sc, pixel, hit);
		pixel->id = body->id;
		pixel->dist = dist;
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

double	cone_components_shadow(t_cone cn, t_vector ray, t_vector p)
{
	int		misc;
	double	dist;

	misc = 0;
	dist = cone_hit_distance(&cn, ray, p, &misc);
	return (smaller_non_negative(dist, disk_hit_distance(cn.bottom_cap, \
				ray, p, &misc)));
}
