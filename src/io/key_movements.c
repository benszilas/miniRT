/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_movements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:24:42 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/04 15:33:54 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	key_move_light(int keycode, t_scene *scene)
{
	if (keycode == XK_h)
		return (scene->light.position.x -= 0.1);
	else if (keycode == XK_l)
		return (scene->light.position.x += 0.1);
	else if (keycode == XK_j)
		return (scene->light.position.y += 0.1);
	else if (keycode == XK_k)
		return (scene->light.position.y -= 0.1);
	else if (keycode == XK_i)
		return (scene->light.position.z += .2);
	else if (keycode == XK_o)
		return (scene->light.position.z -= .2);
	return (false);
}

bool	key_move_focused(int keycode, t_vector *focus)
{
	if (keycode == XK_d)
		return (focus->x += 0.1, true);
	else if (keycode == XK_a)
		return (focus->x -= 0.1, true);
	else if (keycode == XK_w)
		return (focus->y += 0.1, true);
	else if (keycode == XK_s)
		return (focus->y -= 0.1, true);
	else if (keycode == XK_e)
		return (focus->z += .2, true);
	else if (keycode == XK_q)
		return (focus->z -= .2, true);
	return (false);
}

bool	key_rotate_cam(int key, t_scene *scene)
{
	t_camera	*camera;

	camera = &scene->camera;
	if ((key == XK_Up && camera->tilt > -80) || \
	(key == XK_Down && camera->tilt < 80))
	{
		camera->normal = rot_x(camera->normal, key - XK_Up - 1);
		calc_camera_tilt(camera);
		return (true);
	}
	else if (key == XK_Left || key == XK_Right)
	{
		camera->normal = rot_y(camera->normal, key - XK_Left - 1);
		return (true);
	}
	else if ((key == XK_1 && camera->tilt > -80) || \
	(key == XK_3 && camera->tilt < 80))
	{
		camera->normal = rot_z(camera->normal, key - XK_1 - 1);
		calc_camera_tilt(camera);
		return (true);
	}
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

bool	key_change_fov(int keycode, t_camera *camera)
{
	if (keycode == XK_slash)
	{
		if (camera->fov < 175)
		{
			camera->fov += 5;
		}
		return (true);
	}
	else if (keycode == XK_period)
	{
		if (camera->fov > 5)
		{
			camera->fov -= 5;
		}
		return (true);
	}
	return (false);
}
