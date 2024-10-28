/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_body.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:34:35 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/21 13:35:58 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	scene_add_sphere_func(void *data_ptr, void *null)
{
	t_body	sphere_new;
	t_data	*data;

	(void)null;
	ft_putendl_fd("Adding sphere", 2);
	data = data_ptr;
	sphere_new = (t_body){\
		.type = BODY_SPHERE, \
		.id = id_set(ID_GROUP_SPHERE, data->scene.body_cursor), \
		.sphere = (t_sphere){.radius = 1, .center = \
				(t_vector){.x = data->scene.camera.position.x, \
							.y = data->scene.camera.position.y, \
							.z = data->scene.camera.position.z + 10} \
								}};
	sphere_new.color = 0x20ff20;
	sphere_new.texture = &data->scene.texture[0];
	scene_body_add(sphere_new, &data->scene);
}

void	scene_add_plane_func(void *data_ptr, void *null)
{
	t_body	plane_new;
	t_data	*data;

	(void)null;
	ft_putendl_fd("Adding plane", 2);
	data = data_ptr;
	plane_new = (t_body){\
		.type = BODY_PLANE, \
		.id = id_set(ID_GROUP_PLANE, data->scene.body_cursor), \
		.plane = (t_plane){\
			.point = (t_vector){\
				.x = data->scene.camera.position.x, \
				.y = data->scene.camera.position.y, \
				.z = data->scene.camera.position.z + 10}, \
			.normal = (t_vector){.x = 0, .y = 0, .z = -1}}};
	plane_new.color = 0x20ff20;
	plane_new.texture = &data->scene.texture[0];
	scene_body_add(plane_new, &data->scene);
}

void	scene_add_cylinder(void *data_ptr, void *null)
{
	t_body	cylinder;
	t_data	*data;

	(void)null;
	ft_putendl_fd("Adding cylinder", 2);
	data = data_ptr;
	cylinder = (t_body){\
			.type = BODY_CYLINDER, \
			.id = id_set(ID_GROUP_CYLINDER, data->scene.body_cursor), \
			.cylinder = (t_cylinder){\
				.radius = 5, \
				.center = (t_vector){.x = data->scene.camera.position.x + 1, \
									.y = data->scene.camera.position.y + 1, \
									.z = data->scene.camera.position.z + 10}, \
				.height = 10, \
				.normal = (t_vector){.x = 0, .y = 1, .z = 0}}};
	cylinder.color = 0x20ff20;
	calc_cyl_data(&cylinder.cylinder);
	scene_body_add(cylinder, &data->scene);
}
