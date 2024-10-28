/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:41:36 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/27 12:35:01 by bszilas          ###   ########.fr       */
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

bool	parse_camera(	char *entry, \
						uint line_count, \
						t_camera *camera)
{
	char	*params[7];
	static bool	was_parsed = false;

	ft_bzero(params, sizeof(params));
	ft_bzero(camera, sizeof(*camera));
	while (ft_isspace(*entry))
		entry++;
	if (!collect_param_comma(&entry, params, 2) \
		|| !collect_param_comma(&entry, &params[3], 2) \
		|| !collect_param_single(&entry, &params[6]) \
		|| was_parsed == true)
	{
		err("Camera", line_count);
		return (false);
	}
	*camera = (t_camera){.position.x = ft_atod(params[0]), \
		.position.y = ft_atod(params[1]), .position.z = ft_atod(params[2]), \
		.normal.x = ft_atod(params[3]), .normal.y = ft_atoi(params[4]), \
		.normal.z = ft_atoi(params[5]),	.fov = ft_atoi(params[6])};
	normalize_vector(&camera->normal);
	return (was_parsed = true);
}

void	calc_camera_tilt(t_camera *camera)
{
	camera->right = \
	(t_vector){.x = camera->normal.x, .y = 0, .z = camera->normal.z};
	normalize_vector(&camera->right);
	camera->tilt = acos(dot_product(camera->normal, camera->right)) * \
	RAD_TO_DEG * (signbit(camera->normal.y) * 2 - 1);
}

void	set_world_matrix(t_camera *camera)
{
	camera->right = \
	cross_product((t_vector){.x = 0, .y = 1, .z = 0}, camera->normal);
	normalize_vector(&camera->right);
	camera->up = cross_product(camera->normal, camera->right);
	normalize_vector(&camera->up);
	*(t_vector *)&camera->to_world[0][0] = camera->right;
	*(t_vector *)&camera->to_world[1][0] = camera->up;
	*(t_vector *)&camera->to_world[2][0] = camera->normal;
}

t_vector	vec_by_matrix(t_vector vec, t_matrix4 m)
{
	return ((t_vector)\
	{.x = m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0], \
	.y = m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1], \
	.z = m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2]});
}

void	ray_to_world(t_vector *ray, t_camera *camera)
{
	normalize_vector(ray);
	*ray = vec_by_matrix(*ray, camera->to_world);
}

void	define_camera_rays(	t_pixel *pixel, \
							t_camera *camera, \
							t_scene *scene)
{
	double		aspect_ratio;
	uint		x;
	uint		y;
	double		fov;
	t_vector	ray;

	y = 0;
	aspect_ratio = (double)WI / (double)HI;
	fov = tan(camera->fov / 2 * M_PI / 180);
	set_world_matrix(camera);
	while (y < HI)
	{
		x = 0;
		while (x <= WI - scene->resolution_x)
		{
			ray.x = ((2 * (x + 0.5) / WI - 1) * fov * aspect_ratio);
			ray.y = ((1 - 2 * (y + 0.5) / HI) * fov);
			ray.z = 1;
			ray_to_world(&ray, camera);
			ray_check_bodys(&pixel[y * WI + x], ray, scene);
			x += scene->resolution_x;
		}
		y += scene->resolution_y;
	}
}
