/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 13:42:14 by victor            #+#    #+#             */
/*   Updated: 2024/10/22 19:28:11 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

uint	set_color(uint r, uint g, uint b)
{
	return (0xFF << 24 | r << 16 | g << 8 | b);
}

void	color_print(uint color, int fd)
{
	ft_fprintf(fd, " %u,%u,%u\n", (color >> 16) & 0xff, \
			(color >> 8) & 0xff, color & 0xff);
}

bool	shadow_hit_distance(t_vector p, t_vector shadow_ray, t_light l, t_scene *scene)
{
	uint	j;
	t_body	*body;
	double	shadow_hit;
	int		misc;

	j = 0;
	body = scene->body;
	shadow_hit = -1;
	while (body[j].type != BODY_END && j < scene->body_cursor \
			&& (shadow_hit < 0 || shadow_hit > l.obj_distance + SHADOW_BIAS))
	{
		if (body[j].type == BODY_SPHERE)
			shadow_hit = sphere_hit_distance(shadow_ray, vector_subtract(\
						body[j].sphere.center, p), body[j].sphere, &misc);
		else if (body[j].type == BODY_PLANE)
			shadow_hit = plane_hit_distance(body[j].plane, p, shadow_ray, &misc);
		else if (body[j].type == BODY_CYLINDER)
			shadow_hit = cyl_components_shadow(body[j].cylinder, shadow_ray, p);
		else if (body[j].type == BODY_DISK)
			shadow_hit = disk_hit_distance(body[j].disk, shadow_ray, p, &misc);
		else if (body[j].type == BODY_CONE)
			shadow_hit = cone_components_shadow(body[j].cone, shadow_ray, p);
		j++;
	}
	if ((shadow_hit < 0 || shadow_hit > l.obj_distance + SHADOW_BIAS))
		return (false);
	return (true);
}

t_vector	reflect_vector(t_vector v, t_vector n)
{
	t_vector	result;
	double		dot;

	dot = dot_product(v, n);
	result.x = v.x - 2 * dot * n.x;
	result.y = v.y - 2 * dot * n.y;
	result.z = v.z - 2 * dot * n.z;
	return (result);
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
	t_body		*body;
	uint		color;
	t_vector	ray_reflect;
	t_vector	campos_store;
	t_vector	light_direction;

	j = 0;
	body = scene->body;
	color = *pixel->color;
	light_direction = vector_subtract(new_center, scene->light.position);
	light_direction = vector_subtract(new_center, scene->light.position);
	normalize_vector(&light_direction);
	ray_reflect = reflect_vector(light_direction, normal);
	campos_store = scene->camera.position;
	scene->camera.position = new_center;
	while (body[j].type != BODY_END && j < scene->body_cursor)
	{
		if (body[j].type == BODY_SPHERE)
			pixel_sphere_set(pixel, ray_reflect, &body[j], scene);
		j++;
	}
	if (*pixel->color != color)
		*pixel->color = get_color(color, *pixel->color, 0.7);
	scene->camera.position = campos_store;
}

double	get_color_attenuation(t_vector p, t_vector surface_n, \
								t_light l, t_scene *sc)
{
	t_vector	light_n;
	double		dropoff_factor;
	double		attn;

	p = add_vector(p, scale_vector(surface_n, SHADOW_BIAS));
	light_n = vector_subtract(l.position, p);
	l.obj_distance = vector_length(light_n);
	normalize_vector(&light_n);
	if (shadow_hit_distance(p, light_n, l, sc))
		return (0);
	attn = dot_product(surface_n, light_n);
	if (attn < 0)
		attn = 0;
	dropoff_factor = (1 / pow((l.obj_distance + DROPOFF_DISTANCE) \
				/ DROPOFF_DISTANCE, 2));
	attn *= (l.intensity + sc->ambient.intensity) * dropoff_factor;
	return (attn);
}

uint	get_color(uint obj, uint light, double attn)
{
	int32_t	r;
	int32_t	g;
	int32_t	b;

	r = (double)((light >> 16 & 0xFF) * (obj >> 16 & 0xFF)) / 255 * attn;
	if (r > 255)
		r = 255;
	g = (double)((light >> 8 & 0xFF) * (obj >> 8 & 0xFF)) / 255 * attn;
	if (g > 255)
		g = 255;
	b = (double)((light & 0xFF) * (obj & 0xFF)) / 255 * attn;
	if (b > 255)
		b = 255;
	return (set_color(r, g, b));
}

uint	color_blend(uint color1, uint color2)
{
	int	r;
	int	g;
	int	b;

	r = ((color1 >> 16) & 0xff) - ((color2 >> 16) & 0xff);
	if (r < 0)
		r = 0;
	else if (r > 255)
		r = 255;
	g = ((color1 >> 8) & 0xff) - ((color2 >> 8) & 0xff);
	if (g < 0)
		g = 0;
	else if (g > 255)
		g = 255;
	b = ((color1) & 0xff) - ((color2) & 0xff);
	if (b < 0)
		b = 0;
	else if (b > 255)
		b = 255;
	return (set_color(r, g, b));
}

uint	add_color(uint color1, uint color2)
{
	uint	r;
	uint	g;
	uint	b;

	r = ((color1 >> 16) & 0xff) + ((color2 >> 16) & 0xff);
	if (r > 255)
		r = 255;
	g = ((color1 >> 8) & 0xff) + ((color2 >> 8) & 0xff);
	if (g > 255)
		g = 255;
	b = ((color1) & 0xff) + ((color2) & 0xff);
	if (b > 255)
		b = 255;
	return (set_color(r, g, b));
}
