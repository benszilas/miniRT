/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 00:01:14 by victor            #+#    #+#             */
/*   Updated: 2024/11/27 11:14:52 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	mouse_grab(t_mouse *mouse, t_body *body)
{
	if (body)
	{
		if (mouse->grabbed)
			((t_body *)mouse->grabbed)->color = mouse->color_store;
		mouse->grabbed = body;
		mouse->color_store = ((t_body *)mouse->grabbed)->color;
		((t_body *)mouse->grabbed)->color = 0x0000ffff;
	}
	else
	{
		mouse->grabbed = NULL;
	}
}

void	mouse_slider_move(uint x, t_mouse *mouse)
{
	if (x > mouse->slider->bar.x)
		mouse->slider->cursor.x = (int)x;
	if (mouse->slider->cursor.x + mouse->slider->cursor.width \
			> mouse->slider->bar.x + mouse->slider->bar.width)
		mouse->slider->cursor.x = mouse->slider->bar.x \
	+ mouse->slider->bar.width - mouse->slider->cursor.width;
	menu_slider_value_calculate(mouse->slider);
}

void	mouse_move_body(double dx, double dy, int id_group, t_mouse *mouse)
{
	if (id_group == ID_GROUP_SPHERE)
		return ((mouse->grabbed)->sphere.center.x += dx / 10, \
				(mouse->grabbed)->sphere.center.y -= dy / 10, (void)0);
	else if (id_group == ID_GROUP_PLANE)
	{
		(mouse->grabbed)->plane.point.x += dx / 10;
		(mouse->grabbed)->plane.point.y -= dy / 10;
	}
	else if (id_group == ID_GROUP_CYLINDER)
	{
		(mouse->grabbed)->cylinder.center.x += dx / 10;
		(mouse->grabbed)->cylinder.center.y -= dy / 10;
		calc_cyl_data(&mouse->grabbed->cylinder);
	}
	else if (id_group == ID_GROUP_DISC)
	{
		(mouse->grabbed)->disk.point.x += dx / 10;
		(mouse->grabbed)->disk.point.y -= dy / 10;
	}
	else if (id_group == ID_GROUP_CONE)
	{
		(mouse->grabbed)->cone.vertex.x += dx / 10;
		(mouse->grabbed)->cone.vertex.y -= dy / 10;
		calc_cone_data(&mouse->grabbed->cone);
	}
}

void	decrease_resolution(t_scene *scene)
{
	if (scene->resolution_x == 1 && scene->resolution_y == 1)
	{
		scene->anti_aliasing = false;
		scene->resolution_x = RESOLUTION_SCALE_X;
		scene->resolution_y = RESOLUTION_SCALE_Y;
	}
}

void	mouse_left_move(int x, int y, t_mouse *mouse, t_scene *scene)
{
	double	dx;
	double	dy;
	uint	id_group;

	dx = x - mouse->prev_x;
	dy = y - mouse->prev_y;
	if (mouse->grabbed == NULL && !mouse->slider)
	{
		scene->camera.position.x -= dx / 10;
		scene->camera.position.y += dy / 10;
	}
	else if (mouse->slider)
		mouse_slider_move(x, mouse);
	else
	{
		id_group = id_group_get(mouse->grabbed->id);
		mouse_move_body(dx, dy, id_group, mouse);
	}
}

int	mouse_move(int x, int y, t_data *data)
{
	static uint	fake_frames = 0;

	if (data->mouse.left_is_pressed)
	{
		decrease_resolution(&data->scene);
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
