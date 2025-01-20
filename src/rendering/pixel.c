/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:44:38 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 16:35:35 by vvobis           ###   ########.fr       */
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

void	pixel_fill(t_pixel *pixel, t_scene *scene)
{
	uint	i;
	uint	j;
	t_pixel	pixel_new;

	i = 0;
	pixel_new = *pixel;
	while (i < scene->resolution_y)
	{
		j = 0;
		while (j < scene->resolution_x)
		{
			*pixel[i * WI + j].color = *pixel_new.color;
			pixel[i * WI + j].id = pixel_new.id;
			pixel[i * WI + j].dist = pixel_new.dist;
			j++;
		}
		i++;
	}
}

void	set_info_to_pixel(t_pixel *pixel, t_body *body, double dist)
{
	pixel->id = body->id;
	pixel->dist = dist;
	pixel->surface_smoothness = body->surface_smoothness;
}
