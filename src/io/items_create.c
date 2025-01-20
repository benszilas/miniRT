/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   items_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:45:56 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 17:40:02 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

t_item	container_item_button_create(const char *title, void *param, \
								void (*func_ptr)(void *, void *))
{
	t_item	item;

	item.type = ITEM_BUTTON;
	if (title && ft_strlen(title) < ITEM_TITLE_LEN)
		ft_strlcpy(item.description, title, ft_strlen(title) + 1);
	if (param)
		item.button.param = param;
	else
		item.button.param = NULL;
	if (func_ptr)
		item.button.func_ptr = func_ptr;
	return (item);
}

t_item	container_item_slider_create(const char *title, uint8_t min, \
									uint8_t max, uint8_t *value)
{
	t_item	item;

	item.type = ITEM_SLIDER;
	if (title && ft_strlen(title) < ITEM_TITLE_LEN)
		ft_strlcpy(item.description, title, ft_strlen(title) + 1);
	item.slider.min = min;
	item.slider.max = max;
	item.slider.value = value;
	return (item);
}

static void	container_internal_button_add_grid(t_container *container, \
												t_item *item)
{
	uint	item_cursor;
	t_rect	item_attr;

	item_cursor = container->item_count;
	item_attr = (t_rect) \
	{
		.x = container->attr.x + CONTAINER_PADDING \
				+ ((container->attr.width / 2) * (item_cursor % 2)),
		.y = (container->attr.y + ITEM_HEIGHT) + ITEM_PADDING + (ITEM_HEIGHT \
				* (item_cursor / 2)),
		.width = container->attr.width / 2 - (ITEM_PADDING * 2),
		.height = ITEM_HEIGHT,
	};
	item->rect = item_attr;
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
		.y = ITEM_PADDING + ITEM_HEIGHT \
			+ container->item[item_cursor - (item_cursor > 0)].rect.y,
		.width = container->attr.width - ITEM_PADDING,
		.height = ITEM_HEIGHT,
	};
	item->rect = item_attr;
	if (item->type == ITEM_SLIDER)
		container_internal_slider_config(item);
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
		if (container->format == CONTAINER_GRID && item->type == ITEM_BUTTON)
			container_internal_button_add_grid(container, item);
		else
			container_internal_add_list(container, item);
		container->item_count++;
	}
}
