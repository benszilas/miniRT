/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 13:42:14 by victor            #+#    #+#             */
/*   Updated: 2024/11/08 03:33:20 by bszilas          ###   ########.fr       */
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
	if ((shadow < SHADOW_BIAS || shadow > l.obj_distance + SHADOW_BIAS))
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

float	dropoff_factor(float distance)
{
	return (1 / pow((distance + DROPOFF_DISTANCE) / DROPOFF_DISTANCE, 2));
}

uint	phong_reflection(uint obj, float attenuation, t_light l, float gloss)
{
	if (attenuation <= 0 || gloss == 0)
		return (0);
	attenuation *= l.intensity * dropoff_factor((1 / gloss) * l.obj_distance);
	return (get_color(obj, l.color, pow(attenuation, gloss)));
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
