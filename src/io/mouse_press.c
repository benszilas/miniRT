/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_press.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:36:13 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 18:28:32 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	mouse_click_left(int x, int y, t_scene *scene, t_mouse *mouse)
{
	uint	id_group;
	t_body	*body;

	mouse->prev_x = x;
	mouse->prev_y = y;
	mouse->left_is_pressed = true;
	mouse->data->scene.body_focus = NULL;
	id_group = id_group_get(scene->pixel[y * WI + x].id);
	mouse->data->func_ptr = NULL;
	if (id_group == ID_GROUP_ITEM)
	{
		container_item_get_by_id(mouse->data->param, \
			scene->pixel[y * WI + x].id, x, y);
		mouse->data->func_ptr = container_draw;
	}
	else if (id_group > 0 && id_group < ID_GROUP_ITEM)
	{
		body = body_get_by_id(scene->pixel[y * WI + x].id, scene);
		scene->body_focus = NULL;
		mouse_grab(mouse, body);
	}
}

void	mouse_click_right(int x, int y, t_data *data, t_mouse *mouse)
{
	uint	id_group;
	t_body	*body;

	id_group = id_group_get(data->pixel[y * WI + x].id);
	data->scene.body_focus = NULL;
	if (id_group >= ID_GROUP_SPHERE && id_group <= ID_GROUP_CONE)
	{
		body = body_get_by_id(data->pixel[y * WI + x].id, &data->scene);
		data->scene.body_focus = body;
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

void	menu_scroll_items(t_container *param, uint scaler)
{
	uint	i;

	i = 0;
	while (i < param->item_count)
	{
		param->item[i].rect.y += scaler;
		i++;
	}
}

int	mouse_press(int button, int x, int y, t_data *data)
{
	uint	id_group;

	id_group = id_group_get(data->scene.pixel[y * WI + x].id);
	if (button == LEFT_CLICK)
		mouse_click_left(x, y, &data->scene, &data->mouse);
	else if (button == RIGHT_CLICK)
		mouse_click_right(x, y, data, &data->mouse);
	else if (button == SCROLL_UP)
	{
		if (id_group == ID_GROUP_MENU_MAIN || id_group == ID_GROUP_ITEM)
			menu_scroll_items(data->param, SCROLL_UP_DISTANCE);
		else
			key_change_fov(XK_slash, &data->scene.camera);
	}
	else if (button == SCROLL_DOWN)
	{
		if (id_group == ID_GROUP_MENU_MAIN || id_group == ID_GROUP_ITEM)
			menu_scroll_items(data->param, SCROLL_DOWN_DISTANCE);
		else
			key_change_fov(XK_period, &data->scene.camera);
	}
	rendering_loop(data);
	return (0);
}

int	mouse_release(int button, int x, int y, t_data *data)
{
	if (button == 1)
	{
		data->mouse.left_is_pressed = false;
		data->scene.resolution_x = 1, data->scene.resolution_y = 1;
		if (data->mouse.grabbed)
		{
			(void)x;
			(void)y;
			((t_body *)data->mouse.grabbed)->color = data->mouse.color_store;
			rendering_loop(data);
		}
		data->mouse.grabbed = NULL;
		data->mouse.slider = NULL;
	}
	else if (button == 3)
	{
		data->mouse.right_is_pressed = false;
	}
	return (0);
}
