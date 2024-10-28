/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:44:38 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/23 08:59:50 by bszilas          ###   ########.fr       */
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

void	pixels_clear(t_pixel *pixel)
{
	uint	x;
	uint	y;

	y = 0;
	while (y < HI)
	{
		x = 0;
		while (x < WI)
		{
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

void	set_hit_pixel(t_scene *sc, t_pixel *px, double attn, double dist)
{
	uint	color_from_light;
	uint	color_from_ambient;

	color_from_light = get_color(*(uint *)px->color, sc->light.color, attn);
	color_from_ambient = get_color(*(uint *)px->color, sc->ambient.color, 1);
	*px->color = add_color(color_from_light, color_from_ambient);
	px->dist = dist;
	pixel_fill(px, sc);
}
