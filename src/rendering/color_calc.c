/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_calc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:44:57 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 13:03:16 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	shadow_hit_distance(t_vector p, t_light l, t_body *body, uint cursor)
{
	uint	j;
	double	dist;
	int		misc;

	j = 0;
	dist = -1;
	while (body[j].type != BODY_END && j < cursor \
	&& (dist < SHADOW_BIAS || dist > l.obj_distance + SHADOW_BIAS))
	{
		if (body[j].type == BODY_SPHERE)
			dist = sphere_hit_distance(l.normal, vector_subtract(\
						body[j].sphere.center, p), body[j].sphere, &misc);
		else if (body[j].type == BODY_PLANE)
			dist = plane_hit_distance(body[j].plane, p, l.normal, &misc);
		else if (body[j].type == BODY_CYLINDER)
			dist = cyl_components_shadow(body[j].cylinder, l.normal, p);
		else if (body[j].type == BODY_DISK)
			dist = disk_hit_distance(body[j].disk, l.normal, p, &misc);
		else if (body[j].type == BODY_CONE)
			dist = cone_components_shadow(body[j].cone, l.normal, p);
		j++;
	}
	if ((dist < SHADOW_BIAS || dist > l.obj_distance + SHADOW_BIAS))
		return (false);
	return (true);
}

uint	mix_colors(uint base_color, uint reflected_color, double reflectivity)
{
	int	final_color;

	final_color = ((uint)((1.0 - reflectivity) * ((base_color >> 16) & 0xFF) \
					+ reflectivity * ((reflected_color >> 16) & 0xFF)) << 16) \
					| ((uint)((1.0 - reflectivity) * ((base_color >> 8) & 0xFF) \
					+ reflectivity * ((reflected_color >> 8) & 0xFF)) << 8) \
					| (uint)((1.0 - reflectivity) * (base_color & 0xFF) \
					+ reflectivity * (reflected_color & 0xFF));
	if (final_color > 0xffffff)
		final_color = 0xffffff;
	else if (final_color < 0)
		final_color = 0;
	return (final_color);
}

void	get_color_reflect(t_vector new_center, t_vector normal, \
						t_scene *scene, t_pixel *pixel)
{
	uint		j;
	uint		color;
	t_vector	ray_reflect;
	t_vector	campos_store;
	t_vector	light_direction;

	j = 0;
	color = *pixel->color;
	light_direction = vector_subtract(new_center, scene->light.position);
	light_direction = vector_subtract(new_center, scene->light.position);
	normalize_vector(&light_direction);
	ray_reflect = reflect_vector(light_direction, normal);
	campos_store = scene->camera.position;
	scene->camera.position = new_center;
	while (scene->body[j].type != BODY_END && j < scene->body_cursor)
	{
		if (scene->body[j].type == BODY_SPHERE)
			pixel_sphere_set(pixel, ray_reflect, &scene->body[j], scene);
		j++;
	}
	if (*pixel->color != color)
		*pixel->color = get_color(color, *pixel->color, 0.7);
	scene->camera.position = campos_store;
}

double	get_color_attenuation(t_vector p, t_vector surface_n, \
								t_light l, t_scene *sc)
{
	double		dropoff_factor;
	double		attn;

	p = add_vector(p, scale_vector(surface_n, SHADOW_BIAS));
	l.normal = vector_subtract(l.position, p);
	l.obj_distance = vector_length(l.normal);
	normalize_vector(&l.normal);
	if (shadow_hit_distance(p, l, sc->body, sc->body_cursor))
		return (0);
	attn = dot_product(surface_n, l.normal);
	if (attn < 0)
		attn = 0;
	dropoff_factor = (1 / pow((l.obj_distance + DROPOFF_DISTANCE) \
				/ DROPOFF_DISTANCE, 2));
	attn *= (l.intensity + sc->ambient.intensity) * dropoff_factor;
	return (attn);
}
