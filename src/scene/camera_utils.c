/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:55:40 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 10:56:53 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	camera_save(t_camera *camera, int fd)
{
	ft_fprintf(fd, "C %f,%f,%f %f,%f,%f %f\n", \
			camera->position.x, camera->position.y, camera->position.z, \
			camera->normal.x, camera->normal.y, camera->normal.z, \
			camera->fov);
}

void	body_camera_print(t_camera camera)
{
	ft_printf("camera:\nx: %f\ny: %f\nz: %f\nnormal: \n" \
			"x: %f\ny: %f\nz: %f\nfov: %f\n\n", \
			camera.position.x, camera.position.y, camera.position.z, \
			camera.normal.x, camera.normal.y, camera.normal.z, camera.fov);
}

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
	camera->right = cross_product(set_vector(0, 1, 0), camera->normal);
	normalize_vector(&camera->right);
	camera->up = cross_product(camera->normal, camera->right);
	normalize_vector(&camera->up);
	*(t_vector *)&camera->to_world[0][0] = camera->right;
	*(t_vector *)&camera->to_world[1][0] = camera->up;
	*(t_vector *)&camera->to_world[2][0] = camera->normal;
}

void	ray_to_world(t_vector *ray, t_camera *camera)
{
	normalize_vector(ray);
	*ray = vec_by_matrix(*ray, camera->to_world);
}
