/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 10:54:59 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:27:33 by bszilas          ###   ########.fr       */
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
	ft_bzero(body, sizeof(t_body));
	error = false;
	if (!collect_param_comma(&entry, params, 2) || \
	!collect_param_comma(&entry, &params[3], 2) || \
	!collect_param_single(&entry, &params[6]) || \
	!collect_param_single(&entry, &params[7]) || \
	!collect_param_comma(&entry, &params[8], 2))
		return (err("cone", line_count), lst_memory(NULL, NULL, FAIL), false);
	body->type = BODY_CONE;
	body->cone = (t_cone){.vertex = set_vector(ft_atod(params[0]), \
		ft_atod(params[1]), ft_atod(params[2])), .normal = set_vector(\
		ft_atod(params[3]), ft_atod(params[4]), ft_atod(params[5])),
		.radius = ft_atod(params[6]) / 2, .height = ft_atod(params[7])};
	body->color = parse_body_color(params + 8, &error);
	body->id = id_set(ID_GROUP_CYLINDER, body_count);
	if (error || body->cone.radius <= 0 || body->cone.height <= 0)
		return (err("cone", line_count), lst_memory(NULL, NULL, FAIL), false);
	calc_cone_data(&body->cone);
	return (true);
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

void	trace_cone(t_pixel *pixel, t_vector ray, t_body *body, t_scene *sc)
{
	t_vector	p;
	double		attn;
	double		dist;
	t_cone		cn;
	int			flip;

	cn = body->cone;
	flip = 1;
	dist = cone_hit_distance(&cn, ray, sc->camera.position, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		p = add_vector(sc->camera.position, scale_vector(ray, dist));
		*pixel->color = body->color;
		pixel->id = body->id;
		cn.normal = scale_vector(get_normal(p, add_vector(cn.vertex, \
		scale_vector(cn.normal, cn.hit_h * cn.tan_a2p1))), flip);
		attn = get_color_attenuation(p, cn.normal, sc->light, sc);
		set_hit_pixel(sc, pixel, attn, dist);
	}
	trace_cone_bottom(pixel, ray, body, sc);
}
