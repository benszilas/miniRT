/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:13:10 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 17:16:38 by bszilas          ###   ########.fr       */
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

double	cyl_components_shadow(t_cylinder cy, t_vector ray, t_vector p)
{
	int		misc;
	double	dist;
	bool	above_cylinder;
	bool	below_cylinder;

	misc = 0;
	dist = cyl_hit_distance(&cy, ray, p, &misc);
	if (finite_cylinder_hit(&cy, p) && dist > 0)
		return (dist);
	above_cylinder = cy.hit_h > cy.height;
	below_cylinder = cy.hit_h < 0;
	if (!below_cylinder)
		dist = smaller_non_negative(dist, disk_hit_distance((t_disk){\
			.point = cy.top, .normal = cy.normal, .radius = cy.radius, \
			.inverse_normal = scale_vector(cy.normal, -1)}, ray, p, &misc));
	if (!above_cylinder)
		dist = smaller_non_negative(dist, disk_hit_distance((t_disk){\
			.point = cy.bottom, .normal = cy.normal, .radius = cy.radius, \
			.inverse_normal = scale_vector(cy.normal, -1)}, ray, p, &misc));
	return (dist);
}

t_vector	cyl_normal(t_cylinder cy, t_vector p, int invert)
{
	return (scale_vector(get_normal(p, add_vector(cy.bottom, \
						scale_vector(cy.normal, cy.hit_h))), invert));
}

bool	cylinder_caps_visible(t_body *body, t_vector cam_pos, uint id)
{
	if (!finite_cylinder_hit(&body->cylinder, cam_pos) && id == body->id)
		return (true);
	return (true);
}

void	trace_cyl(t_pixel *pixel, t_vector ray, t_body *body, t_scene *sc)
{
	t_hit_point	hit;
	double		dist;
	t_cylinder	cy;
	int			invert;

	cy = body->cylinder;
	calc_cyl_data(&cy);
	invert = 1;
	dist = cyl_hit_distance(&cy, ray, sc->camera.position, &invert);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		set_info_to_pixel(pixel, body, dist);
		hit.p = add_vector(sc->camera.position, scale_vector(ray, dist));
		calc_hit_point_vectors(&hit, ray, cyl_normal(cy, hit.p, invert));
		get_color_cylinder(body, hit.p, pixel);
		if (body->reflect && sc->depth < MAX_DEPTH && invert == 1)
			trace_reflection(pixel, hit, *sc);
		else
			trace_lights(sc, pixel, hit);
	}
	trace_cyl_caps(pixel, ray, body, sc);
}
