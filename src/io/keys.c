/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:56:44 by victor            #+#    #+#             */
/*   Updated: 2024/11/03 14:04:39 by bszilas          ###   ########.fr       */
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

uint	key_change_res(int keycode, t_scene *scene)
{
	if (keycode == XK_equal)
	{
		if (scene->resolution_x == 1 )
			return (scene->resolution_x = 16);
		if (scene->resolution_y == 1)
			return (scene->resolution_y = 9);
		if (scene->resolution_x >= SCENE_START_RESOLUTION_X * SCENE_START_RESOLUTION_CAP || scene->resolution_y >= SCENE_START_RESOLUTION_Y * SCENE_START_RESOLUTION_CAP) 
			return (0);
		scene->resolution_x *= 2;
		scene->resolution_y *= 2;
	}
	else if (keycode == XK_minus)
	{
		if (scene->resolution_x == 16 )
			return (scene->resolution_x = 1);
		if (scene->resolution_y == 9)
			return (scene->resolution_y = 1);
		if (scene->resolution_x > 1 && scene->resolution_y > 1)
		{
			scene->resolution_x /= 2;
			scene->resolution_y /= 2;
		}
	}
	else if (keycode == XK_Return)
		return (scene->resolution_x = scene->resolution_y = 1);
	else if (keycode == XK_BackSpace)
		return (scene->camera.position = (t_vector){0, 0, 0}, \
				scene->camera.fov = 1);
	return (0);
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
	else if (keycode == XK_g)
		scene->gloss = !scene->gloss;
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

void	key_press_distribute(int keycode, t_data *data, t_scene *scene)
{
	if (move_body(keycode, scene->body_focus) || \
	key_move_focused(keycode, &scene->camera.position) || \
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
