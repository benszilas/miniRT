/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:50:16 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/21 19:09:04 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	pixel_fill(t_pixel *pixel, t_scene *scene)
{
	uint		i;
	uint		j;
	t_pixel		pixel_new;

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

void	ray_check_bodys(t_pixel *pixel, t_vector ray, t_scene *scene)
{
	uint	j;
	t_body	*body;

	j = 0;
	body = scene->body;
	pixel->id = 0;
	while (body[j].type != BODY_END && j < scene->body_cursor)
	{
		if (scene->body[j].type == BODY_SPHERE)
			pixel_sphere_set(pixel, ray, body + j, scene);
		else if (scene->body[j].type == BODY_PLANE)
			pixel_plane_set(pixel, ray, body + j, scene);
		else if (scene->body[j].type == BODY_CYLINDER)
			trace_cyl(pixel, ray, body + j, scene);
		else if (scene->body[j].type == BODY_DISK)
			trace_disk(pixel, ray, body + j, scene);
		else if (scene->body[j].type == BODY_CONE)
			trace_cone(pixel, ray, body + j, scene);
		j++;
	}
}

int	time_value_substract(	struct timeval time_minuend, \
							struct timeval time_substrahend)
{
	double	time_minuend_total_value;
	double	time_substrahend_total_value;

	time_minuend_total_value = (time_minuend.tv_sec * 1000) \
								+ (time_minuend.tv_usec / 1000.);
	time_substrahend_total_value = (time_substrahend.tv_sec * 1000.) \
								+ (time_substrahend.tv_usec / 1000.);
	return (time_minuend_total_value - time_substrahend_total_value);
}

uint	rendering_loop(t_data *data)
{
	define_camera_rays(data->pixel, &data->scene.camera, &data->scene);
	if (data->func_ptr)
	{
		data->func_ptr(data, data->param);
	}
	mlx_put_image_to_window(data->mlx, data->win, &data->image, 0, 0);
	mlx_do_sync(data->mlx);
	pixels_clear(data->pixel);
	return (0);
}
