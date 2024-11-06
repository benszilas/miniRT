/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 00:01:14 by victor            #+#    #+#             */
/*   Updated: 2024/11/04 15:38:02 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

int	mouse_press(int button, int x, int y, t_data *data)
{
	if (button == LEFT_CLICK)
		mouse_click_left(x, y, &data->scene, &data->mouse);
	else if (button == RIGHT_CLICK)
		mouse_click_right(x, y, data, &data->mouse);
	else
		mouse_scroll(button, &data->scene, &data->mouse);
	rendering_loop(data);
	return (0);
}

int	mouse_release(int button, int x, int y, t_data *data)
{
	if (button == 1)
	{
		data->mouse.left_is_pressed = false;
		if (data->mouse.grabbed)
		{
			(void)x;
			(void)y;
			((t_body *)data->mouse.grabbed)->color = data->mouse.color_store;
			data->scene.focus = &data->scene.camera.position;
		}
		data->mouse.grabbed = NULL;
		rendering_loop(data);
	}
	else if (button == 3)
	{
		data->mouse.right_is_pressed = false;
	}
	return (0);
}

void	mouse_left_move(int x, int y, t_mouse *mouse, t_scene *scene)
{
	double	dx;
	double	dy;

	dx = x - mouse->prev_x;
	dy = y - mouse->prev_y;
	if (mouse->grabbed == NULL)
	{
		scene->camera.position.x -= dx / 10;
		scene->camera.position.y += dy / 10;
	}
	else
		move_grabbed_object(mouse->grabbed, dx, dy);
}

void	mouse_right_move(int x, int y, t_mouse *mouse, t_scene *scene)
{
	double	dx;
	double	dy;

	dx = x - mouse->prev_x;
	dy = y - mouse->prev_y;
	if (mouse->grabbed == NULL)
	{
		scene->camera.position.x -= dx;
		scene->camera.position.y += dy;
	}
}

int	mouse_move(int x, int y, t_data *data)
{
	static uint	fake_frames = 0;

	if (data->mouse.left_is_pressed)
	{
		if (fake_frames > 2)
		{
			mouse_left_move(x, y, &data->mouse, &data->scene);
			fake_frames = 0;
			rendering_loop(data);
		}
		else
			fake_frames++;
	}
	data->mouse.prev_x = x;
	data->mouse.prev_y = y;
	return (0);
}
