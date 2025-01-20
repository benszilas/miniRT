/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:33:34 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 16:35:09 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

uint	phong_reflection(uint obj, float attenuation, t_light l, float gloss)
{
	if (attenuation <= 0)
		return (0);
	attenuation *= l.intensity * dropoff_factor((1 / gloss) \
			* l.obj_distance);
	return (get_color(obj, l.color, pow(attenuation, gloss)));
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

uint	diffuse_reflection(t_pixel *px, t_hit_point hit, t_light l)
{
	uint	color;

	color = phong_reflection(*px->color, dot_product(hit.n, l.ray), l, 1);
	return (color_brightness(color, 1 - px->surface_smoothness * 0.001953125));
}

uint	specular_reflection(t_pixel *px, t_hit_point hit, t_light l)
{
	uint	color;

	color = phong_reflection(0xFFFFFF, dot_product(hit.r, hit.v), l, \
			GLOSSINESS);
	return (color_brightness(color, px->surface_smoothness * 0.00390625));
}
