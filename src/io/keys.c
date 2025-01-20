/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:56:44 by victor            #+#    #+#             */
/*   Updated: 2024/12/05 16:30:58 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	key_move_light(int keycode, t_scene *scene)
{
	uint	i;

	i = scene->light_focus;
	if (keycode == XK_h)
		return (scene->light[i].position.x -= 0.1);
	else if (keycode == XK_l)
		return (scene->light[i].position.x += 0.1);
	else if (keycode == XK_j)
		return (scene->light[i].position.y += 0.1);
	else if (keycode == XK_k)
		return (scene->light[i].position.y -= 0.1);
	else if (keycode == XK_i)
		return (scene->light[i].position.z += .2);
	else if (keycode == XK_o)
		return (scene->light[i].position.z -= .2);
	else if (keycode == XK_n)
		scene->light_focus = (scene->light_focus + 1) % scene->light_count;
	return (false);
}

int	move_body(int keycode, t_body *body)
{
	if (!body)
		return (false);
	if (body->type == BODY_PLANE)
		return (move_plane(keycode, &body->plane));
	else if (body->type == BODY_SPHERE)
		return (key_move_focused(keycode, &body->sphere.center));
	else if (body->type == BODY_CYLINDER)
		return (move_cylinder(keycode, &body->cylinder));
	else if (body->type == BODY_CONE)
		return (move_cone(keycode, &body->cone));
	else if (body->type == BODY_DISK)
		return (move_disk(keycode, &body->disk));
	return (false);
}

void	rodrigues_rotation(t_vector *vector, t_vector rotation_axis, \
							float angle)
{
	*vector = add_vector(scale_vector(*vector, cos(angle)), \
	add_vector(scale_vector(cross_product(rotation_axis, *vector), \
			sin(angle)), scale_vector(rotation_axis, \
			dot_product(rotation_axis, *vector) * (1 - cos(angle)))));
}

void	rotate_camera(float rotation_angle, t_vector perpendicular_normal, \
		t_camera *camera)
{
	t_vector	rotation_axis;

	rotation_axis = cross_product(camera->normal, perpendicular_normal);
	normalize_vector(&rotation_axis);
	rodrigues_rotation(&camera->normal, rotation_axis, \
			rotation_angle * M_PI / 180);
	calc_camera_tilt(camera);
	calc_camera_space(camera);
}

bool	calc_camera_rotation(int key, t_camera *camera)
{
	float		rotation_angle;
	t_vector	perpendicular_normal;

	rotation_angle = 10;
	if (key == XK_Up)
	{
		perpendicular_normal = camera->up;
		if (camera->tilt <= -80)
			rotation_angle = 89 + camera->tilt;
		camera->tilt -= rotation_angle;
	}
	else if (key == XK_Down)
	{
		perpendicular_normal = scale_vector(camera->up, -1);
		if (camera->tilt >= 80)
			rotation_angle = 89 - camera->tilt;
		camera->tilt += rotation_angle;
	}
	else if (key == XK_Left || key == XK_Right)
		perpendicular_normal = scale_vector(camera->right, key - XK_Left - 1);
	else
		return (false);
	rotate_camera(rotation_angle, perpendicular_normal, camera);
	return (true);
}
