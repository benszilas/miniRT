/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:50:16 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 16:35:46 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	get_background_color(t_scene *sc, t_pixel *px, t_vector v)
{
	t_body		skysphere;
	t_hit_point	hit;

	if (sc->sky_sphere)
	{
		hit.p = v;
		ft_bzero(&skysphere, sizeof(t_body));
		skysphere.textured = true;
		skysphere.texture = &sc->sky;
		skysphere.sphere.radius = 1;
		get_color_sphere(&skysphere, &hit, px);
	}
	else
		*px->color = 0x000000;
}

void	reduce_light_intensity(t_light *light, uint light_count)
{
	uint	i;

	i = 0;
	while (i < light_count)
		light[i++].intensity *= 0.9;
}

void	trace_reflection(t_pixel *pixel, t_hit_point hit, t_scene new_scene)
{
	uint	mirror_id;
	double	mirror_dist;

	reduce_light_intensity(new_scene.light, new_scene.light_count);
	new_scene.camera.position = hit.p;
	new_scene.depth += 1;
	mirror_id = pixel->id;
	mirror_dist = pixel->dist;
	pixel->dist = -1;
	ray_check_bodys(pixel, hit.r, &new_scene);
	if (pixel->dist == -1)
		get_background_color(&new_scene, pixel, hit.r);
	pixel->id = mirror_id;
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

uint	rendering_loop(t_data *data)
{
	pthread_rwlock_unlock(&data->rwlock);
	pthread_barrier_wait(&data->barrier);
	pthread_rwlock_wrlock(&data->rwlock);
	pthread_barrier_wait(&data->barrier);
	if (data->func_ptr)
		data->func_ptr(data, data->param);
	mlx_put_image_to_window(data->mlx, data->win, &data->image, 0, 0);
	mlx_do_sync(data->mlx);
	return (0);
}
