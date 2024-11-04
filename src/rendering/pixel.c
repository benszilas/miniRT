/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:44:38 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/03 19:32:02 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	set_pixel_distances(t_pixel *array, uint size, double dist)
{
	uint	i;

	i = 0;
	while (i < size)
	{
		array[i].dist = dist;
		i++;
	}
}

void	pixel_clear_id(t_pixel *pixel)
{
	uint	x;
	uint	y;

	y = 0;
	while (y < HI)
	{
		x = 0;
		while (x < WI)
		{
			pixel[y * WI + x].id = 0;
			x++;
		}
		y++;
	}
}

void	pixels_clear(t_pixel *pixel, uint wi, uint hi)
{
	uint	x;
	uint	y;

	y = 0;
	while (y < hi)
	{
		x = 0;
		while (x < wi)
		{
			pixel[y * WI + x].id = 0;
			*pixel[y * WI + x].color = 0x000000;
			pixel[y * WI + x].dist = -1;
			x++;
		}
		y++;
	}
}

t_pixel	*pixel_plane_create(void)
{
	t_pixel	*pixels;

	pixels = ft_calloc(HI * WI, sizeof(*pixels));
	lst_memory(pixels, free, ADD);
	set_pixel_distances(pixels, WI * HI, -1);
	return (pixels);
}

void	calc_phong_vectors(t_hit_point *hit, t_light *l)
{
	apply_shadow_bias(&hit->p, hit->n, 1);
	l->ray = vector_subtract(hit->p, l->position);
	l->obj_distance = vector_length(l->ray);
	normalize_vector(&l->ray);
	hit->r = reflect_vector(l->ray, hit->n);
	l->ray = scale_vector(l->ray, -1);
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
		if (!shadow(hit.p, sc->light[i], sc->body, sc))
		{
			color_from_lights = add_color(color_from_lights, \
			phong_reflection(*(uint *)px->color, \
			dot_product(hit.n, sc->light[i].ray), sc->light[i], 1));
			if (sc->gloss)
				color_from_lights = add_color(color_from_lights, \
			phong_reflection(0xFFFFFF, dot_product(hit.r, hit.v), \
			sc->light[i], sc->gloss * GLOSSINESS));
		}
		i++;
	}
	color_from_ambient = get_color(*(uint *)px->color, sc->ambient.color, 1);
	*px->color = add_color(color_from_lights, color_from_ambient);
	pixel_fill(px, sc);
}
