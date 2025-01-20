/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:47:20 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/11 12:06:40 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	container_item_draw(t_item *item, t_pixel *pixel)
{
	if (item->type == ITEM_BUTTON || item->type == ITEM_DIRENT)
	{
		rt_draw_rect(item->rect, pixel, item->id, MENU_ITEM_BG);
		glyph_print(item->rect.x, item->rect.y + ITEM_MARGIN, \
				item->description, pixel);
	}
	else if (item->type == ITEM_SLIDER)
	{
		rt_draw_rect(item->rect, pixel, item->id, MENU_ITEM_BG);
		rt_draw_rect(item->slider.bar, pixel, item->id, 0x0f0f0f);
		rt_draw_rect(item->slider.cursor, pixel, item->id, 0x202020);
		glyph_print(item->slider.cursor.x - 6, item->slider.cursor.y, \
			item->description, pixel);
	}
}

void	container_item_get_by_id(t_container *cont, uint id, uint x, uint y)
{
	uint	i;

	i = 0;
	while (i < cont->item_count)
	{
		if (cont->item[i].id == id)
		{
			if (cont->item[i].type == ITEM_BUTTON \
					&& cont->item[i].button.func_ptr)
			{
				cont->item[i].button.func_ptr(cont->data, &cont->item[i].param);
				if (cont->param.type == PARAM_CONE \
					|| cont->param.type == PARAM_CYL)
					cont->param.func(cont->param.param);
			}
			else if (cont->item[i].type == ITEM_SLIDER && \
					menu_slider_is_cursor(x, y, &cont->item[i].slider.cursor))
				cont->data->mouse.slider = &cont->item[i].slider;
			else if (cont->item[i].type == ITEM_DIRENT \
					&& cont->item[i].dirent.func)
				cont->item[i].dirent.func(&cont->item[i], cont->data);
			return ;
		}
		i++;
	}
}

void	container_draw(void *data_ptr, void *menu_ptr)
{
	uint		i;
	t_data		*data;
	t_container	*menu;

	i = 0;
	data = data_ptr;
	menu = menu_ptr;
	rt_draw_rect_blend(menu->attr, data->pixel, menu->id, MENU_ITEM_BG);
	glyph_print(menu->attr.x + ITEM_PADDING, \
			menu->attr.y + ITEM_PADDING, menu->title, data->pixel);
	while (i < menu->item_count)
	{
		container_item_draw(&menu->item[i], data->pixel);
		i++;
	}
}

/* Title limit = 1024; format see t_format enum */
t_container	container_create(const char *title, t_rect *attr, uint format)
{
	t_container	container;
	static uint	container_id_count = 0;

	ft_bzero(&container, sizeof(container));
	if (ft_strlen(title) < sizeof(container.title))
		ft_strlcpy(container.title, title, ft_strlen(title) + 1);
	else
		ft_fprintf(STDERR_FILENO, "[CONTAINER] Container Title Too Long! "\
					"TITLE_LIMIT is %d", CONTAINER_TITLE_LEN);
	if (attr)
		container.attr = *attr;
	else
	{
		container.attr.width = WI / 3;
		container.attr.x = 0;
		container.attr.height = HI - CONTAINER_PADDING * 2;
		container.attr.y = HI / 2 - container.attr.height / 2;
	}
	container.format = format;
	container.id = id_set(ID_GROUP_MENU_MAIN, container_id_count);
	container.item_count = 0;
	return (container);
}
