/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:41:36 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/05 10:56:57 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	parse_camera(	char *entry, \
						uint line_count, \
						t_camera *camera)
{
	char		*params[7];
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
		.normal.z = ft_atoi(params[5]), .fov = ft_atoi(params[6])};
	normalize_vector(&camera->normal);
	return (was_parsed = true);
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
