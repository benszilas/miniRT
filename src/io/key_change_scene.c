/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_change_scene.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:12:48 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/10 14:18:15 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

uint	key_change_res_minus(t_scene *scene)
{
	if (scene->resolution_x == 1 && scene->resolution_y == 1)
		return (scene->anti_aliasing = ANTI_ALIASING_FACTOR, 1);
	if (scene->resolution_x == RESOLUTION_SCALE_X \
	&& scene->resolution_y == RESOLUTION_SCALE_Y)
		return (scene->resolution_x = 1, scene->resolution_y = 1, 1);
	if (scene->resolution_x > 1 && scene->resolution_y > 1)
		return (scene->resolution_x /= 2, scene->resolution_y /= 2, 1);
	return (1);
}

uint	key_change_res(int keycode, t_scene *scene)
{
	if (keycode == XK_KP_Subtract)
	{
		if (scene->anti_aliasing)
			return (scene->anti_aliasing = false, 1);
		if (scene->resolution_x == 1 && scene->resolution_y == 1)
			return (scene->resolution_x = RESOLUTION_SCALE_X, \
					scene->resolution_y = RESOLUTION_SCALE_Y, 1);
		if (scene->resolution_x >= RESOLUTION_SCALE_X * \
				SCENE_START_RESOLUTION_CAP || scene->resolution_y >= \
				RESOLUTION_SCALE_Y * SCENE_START_RESOLUTION_CAP)
			return (false);
		scene->resolution_x *= 2;
		scene->resolution_y *= 2;
	}
	else if (keycode == XK_KP_Add)
		key_change_res_minus(scene);
	else if (keycode == XK_Return)
		return (scene->resolution_x = scene->resolution_y = 1, 1);
	else if (keycode == XK_BackSpace)
		return (scene->camera.position = (t_vector){0, 0, 0}, \
				scene->camera.fov = 1, 1);
	return (false);
}

bool	key_move_focused(int keycode, t_vector *focus)
{
	if (keycode == XK_d)
		return (focus->x += 0.1, true);
	else if (keycode == XK_a)
		return (focus->x -= 0.1, true);
	else if (keycode == XK_e)
		return (focus->y += 0.1, true);
	else if (keycode == XK_q)
		return (focus->y -= 0.1, true);
	else if (keycode == XK_w)
		return (focus->z += .2, true);
	else if (keycode == XK_s)
		return (focus->z -= .2, true);
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
		return (camera->fov_f = tan(camera->fov / 2 * M_PI / 180));
	}
	else if (keycode == XK_period)
	{
		if (camera->fov > 5)
		{
			camera->fov -= 5;
		}
		return (camera->fov_f = tan(camera->fov / 2 * M_PI / 180));
	}
	return (false);
}
