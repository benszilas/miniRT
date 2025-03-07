/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_ray_define.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:49:39 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/26 19:50:06 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	average_color(t_pixel *pixel, uint *color, uint anti_aliasing)
{
	uint	i;
	int		r;
	int		g;
	int		b;

	if (!anti_aliasing)
		return ;
	r = color[0] >> 16 & 0xFF;
	g = color[0] >> 8 & 0xFF;
	b = color[0] & 0xFF;
	i = 1;
	while (i < anti_aliasing)
	{
		r += color[i] >> 16 & 0xFF;
		g += color[i] >> 8 & 0xFF;
		b += color[i] & 0xFF;
		i++;
	}
	*pixel->color = \
	set_color(r / anti_aliasing, g / anti_aliasing, b / anti_aliasing);
}

float	aa_x(uint x, uint i)
{
	return (x + (i % SQRT_AA_FACTOR) * REC_SQRT_AA_FACTOR);
}

float	aa_y(uint y, uint i)
{
	return (y + ((float)i / SQRT_AA_FACTOR) * REC_SQRT_AA_FACTOR);
}

bool	requires_supersampling(uint x, uint y, t_pixel *pixel, bool aa_on)
{
	uint	color;
	uint	color_left;
	uint	color_above;
	float	limit;
	
	if (!aa_on)
		return false;
	if (x == 0 && y == 0) //top left pixel in thread
		return true;
	color = *pixel->color;
	color_above = (y != 0) ? *pixel[(y - 1) * WI + x].color : color;
	color_left = (x != 0) ? *pixel[y * WI + (x - 1)].color : color;

	if (!color_above || !color_left)
		return true;
	limit = 10;
	if (limit < color_distance(color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff, \
	color_above >> 16 & 0xff, color_above >> 8 & 0xff, color_above & 0xff))
		return true;

	return (limit < color_distance(color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff, \
	color_left >> 16 & 0xff, color_left >> 8 & 0xff, color_left & 0xff));
}

void	anti_aliasing_loop(t_scene *scene, uint x, uint y, t_pixel *pixel)
{
	uint		i;
	t_vector	ray;
	uint		color[MAX_ANTI_ALIASING];

	i = 0;
	while (i < scene->anti_aliasing + 1)
	{
		ray = set_ray(aa_x(x, i), aa_y(y, i), scene, &scene->camera);
		pixels_clear(&pixel[y * WI + x], scene->resolution_x, \
				scene->resolution_y);
		ray_check_bodys(&pixel[y * WI + x], ray, scene);
		color[i++] = *pixel[y * WI + x].color;
	}
	average_color(&pixel[y * WI + x], color, scene->anti_aliasing);
}

void	thread_define_camera_rays(t_thread *thread, t_pixel *pixel, \
		t_scene *scene, t_camera *camera)
{
	uint	x;
	uint	y;

	y = thread->starty;
	set_world_matrix(camera);
	while (y < thread->starty + THREAD_HEIGHT)
	{
		x = 0;
		while (x < WI)
		{
			anti_aliasing_loop(scene, x, y, pixel);
			x += scene->resolution_x;
		}
		y += scene->resolution_y;
	}
}
