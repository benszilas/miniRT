/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 00:01:14 by victor            #+#    #+#             */
/*   Updated: 2024/10/21 16:17:22 by bszilas          ###   ########.fr       */
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
}

void	mouse_click_left(int x, int y, t_scene *scene, t_mouse *mouse)
{
	uint	id_group;
	t_body	*body;

	mouse->prev_x = x;
	mouse->prev_y = y;
	mouse->left_is_pressed = true;
	id_group = id_group_get(scene->pixel[y * WI + x].id);
	mouse->data->func_ptr = NULL;
	if (id_group == ID_GROUP_SPHERE || id_group == ID_GROUP_DISC \
			|| id_group == ID_GROUP_PLANE || id_group == ID_GROUP_CYLINDER \
			|| id_group == ID_GROUP_DISC || id_group == ID_GROUP_CONE)
	{
		body = body_get_by_id(scene->pixel[y * WI + x].id, scene);
		mouse_grab(mouse, body);
		return ;
	}
	else if (id_group == ID_GROUP_ITEM)
		return (container_item_get_by_id(mouse->data->param, \
				scene->pixel[y * WI + x].id), \
				mouse->data->func_ptr = container_draw, (void)0);
	if (mouse->grabbed)
		return (((t_body *)mouse->grabbed)->color = mouse->color_store, \
				mouse->grabbed = NULL, (void)0);
}

void	mouse_click_right(int x, int y, t_data *data, t_mouse *mouse)
{
	uint	id_group;
	t_body	*body;

	id_group = id_group_get(data->pixel[y * WI + x].id);
	data->scene.focus2 = NULL;
	if (id_group >= ID_GROUP_SPHERE && id_group <= ID_GROUP_CONE)
	{
		body = body_get_by_id(data->pixel[y * WI + x].id, &data->scene);
		data->scene.focus2 = body;
		if (id_group == ID_GROUP_PLANE)
			plane_menu_map(&data->menu[ID_GROUP_PLANE], body, &body->color);
		else if (id_group == ID_GROUP_SPHERE)
			sphere_menu_map(&data->menu[ID_GROUP_SPHERE], body, &body->color);
		else if (id_group == ID_GROUP_CYLINDER)
			cylinder_menu_map(&data->menu[ID_GROUP_CYLINDER], \
					body, &body->color);
		else if (id_group == ID_GROUP_CONE)
			cone_menu_map(&data->menu[ID_GROUP_CONE], body, &body->color);
		else if (id_group == ID_GROUP_DISC)
			disk_menu_map(&data->menu[ID_GROUP_DISC], body, &body->color);
	}
	else
		mouse->grabbed = NULL;
	data->param = &data->menu[id_group];
	data->func_ptr = container_draw;
}

void	mouse_scroll(int button, t_scene *scene, t_mouse *mouse)
{
	if (button == SCROLL_UP)
	{
		if (mouse->grabbed)
		{
			if (((t_body *)mouse->grabbed)->type == BODY_SPHERE)
				((t_body *)mouse->grabbed)->sphere.radius += 0.1;
			else if (((t_body *)mouse->grabbed)->type == BODY_PLANE)
				((t_body *)mouse->grabbed)->plane.normal.y += 0.01;
			return ;
		}
		key_change_fov(XK_slash, &scene->camera);
	}
	else if (button == SCROLL_DOWN)
	{
		if (mouse->grabbed)
		{
			if (((t_body *)mouse->grabbed)->type == BODY_SPHERE)
				((t_body *)mouse->grabbed)->sphere.radius -= 0.1;
			else if (((t_body *)mouse->grabbed)->type == BODY_PLANE)
				((t_body *)mouse->grabbed)->plane.normal.y -= 0.01;
			return ;
		}
		key_change_fov(XK_period, &scene->camera);
	}
}

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
	uint	id_group;

	dx = x - mouse->prev_x;
	dy = y - mouse->prev_y;
	if (mouse->grabbed == NULL)
	{
		scene->camera.position.x -= dx / 10;
		scene->camera.position.y += dy / 10;
	}
	else
	{
		id_group = id_group_get(((t_body *)mouse->grabbed)->id);
		if (id_group == ID_GROUP_SPHERE)
		{
			(mouse->grabbed)->sphere.center.x += dx / 10;
			(mouse->grabbed)->sphere.center.y -= dy / 10;
		}
		else if (id_group == ID_GROUP_PLANE)
		{
			(mouse->grabbed)->plane.normal.x += dx / 10;
			(mouse->grabbed)->plane.normal.y -= dy / 10;
		}
		else if (id_group == ID_GROUP_CYLINDER)
		{
			(mouse->grabbed)->cylinder.center.x += dx / 10;
			(mouse->grabbed)->cylinder.center.y -= dy / 10;
		}
	}
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
