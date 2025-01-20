/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:30:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 17:12:37 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

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
	else if (keycode == XK_b)
	{
		scene->sky_sphere = !scene->sky_sphere;
		swap_2_ints(&scene->sky_color, &scene->ambient.color);
	}
	return (false);
}

int	key_move_camera(int keycode, t_camera *camera)
{
	t_vector	translation;

	calc_camera_space(camera);
	if (keycode == XK_a || keycode == XK_d)
		translation = scale_vector(camera->right, \
				(keycode - XK_a - 1.5) * 0.66);
	else if (keycode == XK_w || keycode == XK_s)
		translation = scale_vector(camera->normal, (keycode - XK_s - 2) * 0.5);
	else if (keycode == XK_q || keycode == XK_e)
		translation = scale_vector(camera->up, (keycode - XK_e - 6) * 0.33);
	else
		return (false);
	camera->position = add_vector(camera->position, translation);
	calc_camera_space(camera);
	return (true);
}

void	key_press_distribute(int keycode, t_data *data, t_scene *scene)
{
	if (move_body(keycode, scene->body_focus) || \
	key_move_camera(keycode, &scene->camera) || \
	calc_camera_rotation(keycode, &scene->camera) || \
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
