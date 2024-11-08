/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:50:16 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/08 06:39:12 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

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

void	get_background_color(t_scene *sc, t_pixel *px, t_vector v)
{
	t_body	skysphere;

	if (sc->sky_sphere && sc->texture + SKYSPHERE)
	{
		ft_bzero(&skysphere, sizeof(t_body));
		skysphere.textured = true;
		skysphere.texture = sc->texture + SKYSPHERE;
		skysphere.sphere.radius = 1;
		get_color_sphere(&skysphere, v, px);
	}
	else
		*px->color = get_color(sc->ambient.color, 0xFFFFFF, sc->ambient.intensity);
}

void	trace_reflection(t_pixel *pixel, t_hit_point hit, t_scene new_scene)
{
	uint	i;
	uint	mirror_id;
	double	mirror_dist;

	i = 0;
	while (i < new_scene.light_count)
		new_scene.light[i++].intensity *= 0.9;
	new_scene.camera.position = hit.p;
	new_scene.depth += 1;
	mirror_id = pixel->id;
	mirror_dist = pixel->dist;
	pixel->dist = -1;
	ray_check_bodys(pixel, hit.r, &new_scene);
	pixel->id = mirror_id;
	if (pixel->dist == -1)
		get_background_color(&new_scene, pixel, hit.r);
	pixel->dist = mirror_dist;
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
	if (!pixel->id)
	{
		get_background_color(scene, pixel, ray);
		pixel_fill(pixel, scene);
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
	/*unblock threads so they can lock read*/
	pthread_rwlock_unlock(&data->rwlock);
	/*synchronize until all threads lock read*/
	pthread_barrier_wait(&data->barrier);
	/*block until threads make the image and release all read locks*/
	pthread_rwlock_wrlock(&data->rwlock);
	/*tell threads they can now wait for read locks*/
	pthread_barrier_wait(&data->barrier);
	if (data->func_ptr)
		data->func_ptr(data, data->param);
	else
		help_menu_draw(data, NULL);
	mlx_put_image_to_window(data->mlx, data->win, &data->image, 0, 0);
	mlx_do_sync(data->mlx);
	return (0);
}
