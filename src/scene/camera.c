/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:41:36 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/04 17:06:25 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	calc_camera_tilt(t_camera *camera)
{
	camera->right = \
	(t_vector){.x = camera->normal.x, .y = 0, .z = camera->normal.z};
	normalize_vector(&camera->right);
	camera->tilt = acos(dot_product(camera->normal, camera->right) - \
	SHADOW_BIAS) * RAD_TO_DEG * ((signbit(camera->normal.y) * 2 - 1));
}

void	set_world_matrix(t_camera *camera)
{
	*(t_vector *)&camera->to_world[0][0] = camera->right;
	*(t_vector *)&camera->to_world[1][0] = camera->up;
	*(t_vector *)&camera->to_world[2][0] = camera->normal;
}

t_vector	vec_by_matrix(t_vector vec, t_matrix4 m)
{
	return ((t_vector) \
	{.x = m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0], \
	.y = m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1], \
	.z = m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2]});
}

void	ray_to_world(t_vector *ray, t_camera *camera)
{
	normalize_vector(ray);
	*ray = vec_by_matrix(*ray, camera->to_world);
}

t_vector	set_ray(float x, float y, t_scene *scene, t_camera *camera)
{
	t_vector	ray;

	ray.x = ((2 * x / WI - 1) * camera->fov_f * ASPECT_RATIO);
	ray.y = ((1 - 2 * y / HI) * camera->fov_f);
	ray.z = 1;
	ray_to_world(&ray, camera);
	scene->depth = 0;
	return (ray);
}
