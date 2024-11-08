/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:47:20 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/13 09:16:57 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	container_item_draw(t_item *item, t_pixel *pixel)
{
	rt_draw_rect(item->attr, pixel, item->id, MENU_ITEM_BG);
	glyph_print(item->attr.x, item->attr.y + ITEM_MARGIN, \
			item->description, pixel);
}

void	container_item_get_by_id(t_container *cont, uint id)
{
	uint	i;

	i = 0;
	while (i < cont->item_count)
	{
		if (cont->item[i].id == id)
		{
			if (cont->item[i].func_ptr)
				cont->item[i].func_ptr(cont->data, cont->item[i].param);
			return ;
		}
		i++;
	}
}

t_item	container_item_create(const char *title, void *param, \
								void (*func_ptr)(void *, void *))
{
	t_item	item;

	if (title && ft_strlen(title) < CONTAINER_TITLE_LEN)
		ft_strlcpy(item.description, title, ft_strlen(title) + 1);
	if (param)
		item.param = param;
	else
		item.param = NULL;
	if (func_ptr)
		item.func_ptr = func_ptr;
	return (item);
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
	while (i < menu->item_count)
	{
		container_item_draw(&menu->item[i], data->pixel);
		i++;
	}
}

static void	container_internal_add_grid(t_container *container, t_item *item)
{
	uint	item_cursor;
	t_rect	item_attr;

	item_cursor = container->item_count;
	item_attr = (t_rect) \
	{
		.x = container->attr.x + CONTAINER_PADDING \
				+ ((container->attr.width / 2) * (item_cursor % 2)),
		.y = container->attr.y + ITEM_PADDING + ((ITEM_HEIGHT) \
				* ((item_cursor) / 2)),
		.width = container->attr.width / 2 - (ITEM_PADDING * 2),
		.height = ITEM_HEIGHT,
	};
	item->attr = item_attr;
	container->item[item_cursor] = *item;
}

static void	container_internal_add_list(t_container *container, t_item *item)
{
	uint	item_cursor;
	t_rect	item_attr;

	item_cursor = container->item_count;
	item_attr = (t_rect) \
	{
		.x = container->attr.x + CONTAINER_PADDING,
		.y = container->attr.y + ITEM_PADDING + ((ITEM_HEIGHT) \
				* ((item_cursor))),
		.width = container->attr.width - (ITEM_PADDING * 3),
		.height = ITEM_HEIGHT,
	};
	item->attr = item_attr;
	container->item[item_cursor] = *item;
}

void	container_item_add(t_container *container, t_item *item)
{
	if (!container)
		return (ft_fprintf(STDERR_FILENO, \
					"[CONTAINER] Container Invalid!\n"), (void)0);
	if (container->item_count > CONTAINER_ITEM_COUNT - 1)
		return (ft_fprintf(STDERR_FILENO, \
					"[CONTAINER] Container '%s' creation Failed!: " \
					"Too many items (%u)! MAX_ITEM is %d\n", \
					container->title, container->item_count, \
					CONTAINER_ITEM_COUNT), (void)0);
	if (item)
	{
		item->id = id_set(ID_GROUP_ITEM, container->item_count);
		if (container->format == CONTAINER_GRID)
			container_internal_add_grid(container, item);
		else if (container->format == CONTAINER_LIST)
			container_internal_add_list(container, item);
		container->item_count++;
	}
}

/* Title limit = 1024; format see t_format enum */
t_container	container_create(const char *title, t_rect *attr, uint format)
{
	t_container	container;

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
	container.id = id_set(ID_GROUP_MENU_MAIN, 0);
	container.item_count = 0;
	return (container);
}
