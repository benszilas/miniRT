/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_shadow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:56:10 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/17 12:08:06 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

float	dropoff_factor(float distance)
{
	return (1 / pow((distance + DROPOFF_DISTANCE) / DROPOFF_DISTANCE, 2));
}

void	trace_lights(t_scene *sc, t_pixel *px, t_hit_point hit)
{
	uint	color_from_lights;
	uint	color_from_ambient;
	uint	i;

	i = 0;
	color_from_lights = 0;
	while (i < sc->light_count)
	{
		calc_phong_vectors(&hit, sc->light + i);
		apply_shadow_bias(&hit.p, hit.n, 1);
		if (!shadow(hit.p, sc->light[i], sc->body, sc))
		{
			color_from_lights = add_color(color_from_lights, \
			diffuse_reflection(px, hit, sc->light[i]));
			color_from_lights = add_color(color_from_lights, \
			specular_reflection(px, hit, sc->light[i]));
		}
		i++;
	}
	color_from_ambient = get_color(*px->color, sc->ambient.color, 1);
	*px->color = add_color(color_from_lights, color_from_ambient);
	pixel_fill(px, sc);
}

double	sphere_shadow(t_vector p, t_light l, t_sphere *sphere)
{
	int	misc;

	if (fabs((vector_length(vector_subtract(l.position, sphere->center))) \
		- sphere->radius) < SHADOW_BIAS)
		return (SHADOW_BIAS);
	return (sphere_hit_distance(l.ray, vector_subtract(\
						sphere->center, p), *sphere, &misc));
}

double	cylinder_shadow(t_vector p, t_light l, t_cylinder *cylinder)
{
	if (fabs(vector_length(cross_product(vector_subtract(l.position, \
		cylinder->center), cylinder->normal)) - cylinder->radius) < SHADOW_BIAS)
		return (SHADOW_BIAS);
	return (cyl_components_shadow(*cylinder, l.ray, p));
}

bool	shadow(t_vector p, t_light l, t_body *body, t_scene *scene)
{
	uint	j;
	double	shadow;
	int		misc;

	j = 0;
	shadow = -1;
	while (body[j].type != BODY_END && j < scene->body_cursor \
	&& (shadow < SHADOW_BIAS || shadow > l.obj_distance + SHADOW_BIAS))
	{
		if (body[j].type == BODY_SPHERE)
			shadow = sphere_hit_distance(l.ray, vector_subtract(\
						body[j].sphere.center, p), body[j].sphere, &misc);
		else if (body[j].type == BODY_PLANE)
			shadow = plane_hit_distance(body[j].plane, p, l.ray, &misc);
		else if (body[j].type == BODY_CYLINDER)
			shadow = cyl_components_shadow(body[j].cylinder, l.ray, p);
		else if (body[j].type == BODY_DISK)
			shadow = disk_hit_distance(body[j].disk, l.ray, p, &misc);
		else if (body[j].type == BODY_CONE)
			shadow = cone_components_shadow(body[j].cone, l.ray, p);
		j++;
	}
	if (shadow < SHADOW_BIAS || shadow > l.obj_distance + SHADOW_BIAS)
		return (false);
	return (true);
}
