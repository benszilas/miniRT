/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_info.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:51:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/17 11:37:18 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	camera_save(t_camera *camera, int fd)
{
	ft_fprintf(fd, "C %f,%f,%f %f,%f,%f %d\n", \
		camera->position.x, camera->position.y, camera->position.z, \
		camera->normal.x, camera->normal.y, camera->normal.z, \
		(int)camera->fov);
}

void	body_camera_print(t_camera camera)
{
	ft_printf("camera:\nx: %f\ny: %f\nz: %f\nnormal: \n" \
		"x: %f\ny: %f\nz: %f\nfov: %d\n\n", \
		camera.position.x, camera.position.y, camera.position.z, \
		camera.normal.x, camera.normal.y, camera.normal.z, (int)camera.fov);
}

bool	parse_camera(	char *entry, \
						uint line_count, \
						t_camera *camera, \
						bool	*was_parsed)
{
	char		*params[7];
	int			error;

	error = false;
	ft_bzero(params, sizeof(params));
	ft_bzero(camera, sizeof(*camera));
	while (ft_isspace(*entry))
		entry++;
	if (!collect_param_comma(&entry, params, 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[3], 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_single(&entry, &params[6], COLLECT_MODE_END) \
		|| *was_parsed == true)
		return (err("Camera", line_count), false);
	*camera = (t_camera){.position.x = ft_atod(params[0]), \
		.position.y = ft_atod(params[1]), .position.z = ft_atod(params[2]), \
		.normal.x = ft_atod(params[3]), .normal.y = ft_atod(params[4]), \
		.normal.z = ft_atod(params[5]), .fov = \
		set_signed_int(params[6], 0, 179, &error)};
	if (error == true || !check_normal_vector(&camera->normal))
		return (err("Camera", line_count), false);
	normalize_vector(&camera->normal);
	calc_camera_tilt(camera);
	calc_camera_space(camera);
	camera->fov_f = tan(camera->fov / 2 * M_PI / 180);
	return (*was_parsed = true);
}

void	calc_camera_space(t_camera *camera)
{
	camera->right = cross_product((t_vector){0, 1, 0}, camera->normal);
	if (camera->right.x == 0 && camera->right.y == 0 && camera->right.z == 0)
	{
		camera->right = cross_product((t_vector){1, 0, 0}, camera->normal);
	}
	normalize_vector(&camera->right);
	camera->up = cross_product(camera->normal, camera->right);
	normalize_vector(&camera->up);
}
