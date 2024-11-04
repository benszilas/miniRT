/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:50:16 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/03 21:32:07 by bszilas          ###   ########.fr       */
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

uint	get_background_color(t_scene *sc)
{
	return (0); //we can add different backgrounds later to make mirrors look better
}

void	trace_reflection(t_pixel *pixel, t_hit_point hit, t_scene new_scene)
{
	uint	i;

	i = 0;
	while (i < new_scene.light_count)
		new_scene.light[i++].intensity *= 0.9;
	new_scene.camera.position = hit.p;
	new_scene.depth += 1;
	pixel->dist = -1;
	ray_check_bodys(pixel, hit.r, &new_scene);
	if (pixel->dist == -1)
		*pixel->color = get_background_color(&new_scene);
}

void	ray_check_bodys(t_pixel *pixel, t_vector ray, t_scene *scene)
{
	uint	j;
	t_body	*body;

	j = 0;
	pixel->id = 0;
	body = scene->body;
	while (body[j].type != BODY_END && j < scene->body_cursor)
	{
		if (body[j].type == BODY_SPHERE)
			pixel_sphere_set(pixel, ray, body + j, scene);
		else if (body[j].type == BODY_PLANE)
			trace_plane(pixel, ray, body + j, scene);
		else if (body[j].type == BODY_CYLINDER)
			trace_cyl(pixel, ray, body + j, scene);
		else if (body[j].type == BODY_DISK)
			trace_disk(pixel, ray, body + j, scene);
		else if (body[j].type == BODY_CONE)
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
	pthread_mutex_lock(&data->mutex);
	thread_scene_update(data);
	data->go = true;
	pthread_cond_broadcast(&data->cond);
	pthread_mutex_unlock(&data->mutex);
	pthread_barrier_wait(&data->barrier);
	pthread_mutex_lock(&data->mutex);
	data->go = false;
	if (data->func_ptr)
		data->func_ptr(data, data->param);
	mlx_put_image_to_window(data->mlx, data->win, &data->image, 0, 0);
	mlx_do_sync(data->mlx);
	pthread_mutex_unlock(&data->mutex);
	return (0);
}

/*uint	rendering_loop(t_data *data)*/
/*{*/
/*	define_camera_rays(data->pixel, &data->scene.camera, &data->scene);*/
/*	if (data->func_ptr)*/
/*	{*/
/*		data->func_ptr(data, data->param);*/
/*	}*/
/*	mlx_put_image_to_window(data->mlx, data->win, &data->image, 0, 0);*/
/*	mlx_do_sync(data->mlx);*/
/*	pixels_clear(data->pixel);*/
/*	return (0);*/
/*}*/
