/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:56:44 by victor            #+#    #+#             */
/*   Updated: 2024/11/05 15:29:42 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	decrease_resolution(t_scene *scene)
{
	if (scene->resolution_x == SCENE_START_RESOLUTION_X && \
		scene->resolution_y == SCENE_START_RESOLUTION_Y)
		return (scene->resolution_x = 1, scene->resolution_y = 1);
	if (scene->resolution_x > SCENE_START_RESOLUTION_X && \
		scene->resolution_y > SCENE_START_RESOLUTION_Y)
		return (scene->resolution_x /= 2, scene->resolution_y /= 2);
	else
		return (false);
}

uint	key_change_res(int keycode, t_scene *scene)
{
	if (keycode == XK_equal)
	{
		if (scene->resolution_x >= SCENE_START_RESOLUTION_X * \
		SCENE_START_RESOLUTION_CAP && scene->resolution_y >= \
		SCENE_START_RESOLUTION_Y * SCENE_START_RESOLUTION_CAP)
			return (false);
		if (scene->resolution_x == 1 && scene->resolution_y == 1)
			return (scene->resolution_x = SCENE_START_RESOLUTION_X, \
					scene->resolution_y = SCENE_START_RESOLUTION_Y);
		return (scene->resolution_x *= 2, scene->resolution_y *= 2);
	}
	else if (keycode == XK_minus)
		return (decrease_resolution(scene));
	else if (keycode == XK_Return)
		return (scene->resolution_x = scene->resolution_y = 1);
	else if (keycode == XK_BackSpace)
		return (scene->camera.position = (t_vector){0, 0, 0}, \
				scene->camera.fov = 90);
	return (0);
}

uint	key_misc_function(int keycode, t_scene *scene, t_data *data)
{
	if (keycode == XK_p)
		scene_save(scene);
	else if (keycode == XK_m)
		return (data->func_ptr = container_draw, \
				data->param = &data->menu[MENU_MAIN], true);
	else if (keycode == XK_question)
		return (data->func_ptr = help_menu_draw, true);
	else if (keycode == XK_Delete)
	{
		if (data->mouse.grabbed)
			((t_body *)data->mouse.grabbed)->type = BODY_DELETED;
		data->mouse.grabbed = NULL;
		return (true);
	}
	return (false);
}

void	key_press_distribute(int keycode, t_data *data, t_scene *scene)
{
	if (move_body(keycode, scene->focus2) || \
	key_move_focused(keycode, scene->focus) || \
	key_rotate_cam(keycode, scene) || \
	key_misc_function(keycode, scene, data) || \
	key_change_res(keycode, scene) || \
	key_move_light(keycode, scene) || \
	key_change_fov(keycode, &scene->camera))
		return ;
}

int	key_press(int keycode, void *data_ptr)
{
	t_scene	*scene;
	t_data	*data;

	data = data_ptr;
	scene = &data->scene;
	if (keycode == XK_Escape)
	{
		lst_memory(NULL, NULL, END);
		exit(0);
	}
	key_press_distribute(keycode, data, scene);
	mlx_flush_event(data->mlx);
	rendering_loop(data);
	return (1);
}
