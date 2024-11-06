/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:13:10 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/05 11:26:03 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	move_cylinder(int keycode, t_cylinder *cyl)
{
	if (keycode == XK_Up || keycode == XK_Down)
		cyl->normal = rot_x(cyl->normal, keycode - XK_Up - 1);
	else if (keycode == XK_Left || keycode == XK_Right)
		cyl->normal = rot_y(cyl->normal, keycode - XK_Left - 1);
	else if (keycode == XK_1 || keycode == XK_3)
		cyl->normal = rot_z(cyl->normal, keycode - XK_1 - 1);
	else if (!key_move_focused(keycode, &cyl->center))
		return (false);
	calc_cyl_data(cyl);
	return (true);
}

bool	parse_cylinder(char *entry, uint line_c, t_body *body, uint body_cursor)
{
	char	*params[11];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	if (!collect_param_comma(&entry, params, 2) \
		|| !collect_param_comma(&entry, &params[3], 2) \
		|| !collect_param_single(&entry, &params[6]) \
		|| !collect_param_single(&entry, &params[7]) \
		|| !collect_param_comma(&entry, &params[8], 2))
		return (err("cylinder", line_c), lst_memory(NULL, NULL, FAIL), false);
	body->type = BODY_CYLINDER;
	body->cylinder = (t_cylinder){\
		.center = set_vector(ft_atod(params[0]), ft_atod(params[1]), \
		ft_atod(params[2])), .normal = set_vector(ft_atod(params[3]), \
		ft_atod(params[4]), ft_atod(params[5])), \
		.radius = ft_atod(params[6]) / 2, .height = ft_atod(params[7])};
	body->color = parse_body_color(params + 8, &error);
	body->id = id_set(ID_GROUP_CYLINDER, body_cursor);
	if (error || body->cylinder.radius <= 0 || body->cylinder.height <= 0)
		return (err("cone", line_c), lst_memory(NULL, NULL, FAIL), false);
	calc_cyl_data(&body->cylinder);
	return (true);
}

void	get_color_checker_cylinder(double u, double v, t_pixel *pixel)
{
	int	checker_u;
	int	checker_v;

	checker_u = (int)(u * 16);
	checker_v = (int)(v * 8);
	if ((checker_u + checker_v) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

void	get_color_cylinder(t_body *body, t_vector intersect, t_pixel *pixel)
{
	double		u;
	double		v;
	t_vector	local;
	double		theta;

	if (!body->checker_board && !body->textured)
	{
		*pixel->color = body->color;
		return ;
	}
	local = get_local(&body->cylinder, intersect);
	theta = atan2(local.z, local.x);
	u = (theta + M_PI) / (2 * M_PI);
	v = (local.y - (body->cylinder.height / 2)) / body->cylinder.height;
	v = fmod(v, 1.0);
	if (v < 0)
		v += 1.0;
	if (body->checker_board)
		get_color_checker_cylinder(u, v, pixel);
}

void	trace_cyl(t_pixel *pixel, t_vector ray, t_body *body, t_scene *sc)
{
	t_vector	p;
	double		attn;
	double		dist;
	t_cylinder	cy;
	int			flip;

	calc_cyl_data(&body->cylinder);
	cy = body->cylinder;
	flip = 1;
	dist = cyl_hit_distance \
	(&cy, ray, sc->camera.position, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		p = add_vector(sc->camera.position, scale_vector(ray, dist));
		pixel->id = body->id;
		get_color_cylinder(body, p, pixel);
		if (body->reflect == true)
			get_color_reflect(p, add_vector(cy.bottom, \
						scale_vector(cy.normal, cy.hit_h)), sc, pixel);
		cy.normal = scale_vector(get_normal(p, add_vector(cy.bottom, \
						scale_vector(cy.normal, cy.hit_h))), flip);
		attn = get_color_attenuation(p, cy.normal, sc->light, sc);
		set_hit_pixel(sc, pixel, attn, dist);
	}
	trace_cyl_caps(pixel, ray, body, sc);
}
