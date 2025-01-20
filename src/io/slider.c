/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:43:03 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 19:52:43 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	menu_slider_value_calculate(t_slider *slider)
{
	uint	min;
	uint	current_value;

	if (!slider->value)
		return ;
	min = slider->bar.x;
	current_value = slider->cursor.x + slider->cursor.width / 2 - min;
	*slider->value = (current_value * 100) / slider->max;
}

bool	menu_slider_is_cursor(uint x, uint y, t_rect *slider)
{
	if (x > slider->x && x < slider->x + slider->width \
		&& y > slider->y && y < slider->y + slider->height)
		return (true);
	return (false);
}

void	container_internal_slider_config(t_item *item)
{
	item->slider.bar.height = item->rect.height / 3;
	item->slider.bar.width = item->rect.width - ITEM_PADDING;
	item->slider.bar.x = item->rect.x + ITEM_PADDING / 2;
	item->slider.bar.y = item->rect.y + item->slider.bar.height;
	item->slider.cursor.height = 16;
	item->slider.cursor.width = 16;
	item->slider.cursor.y = item->slider.bar.y \
		+ item->slider.bar.height / 2 - item->slider.cursor.height / 2;
	item->slider.cursor.x = item->slider.bar.x;
}

void	menu_body_slider_add(t_container *menu, char *name)
{
	t_item	item;

	item = container_item_slider_create(name, 0, 255, NULL);
	container_item_add(menu, &item);
}

void	menu_body_slider_map_value(t_item *item, uint8_t *value)
{
	uint	min;
	uint	max;
	uint	range;

	min = item->slider.bar.x;
	max = item->slider.bar.x + item->slider.bar.width;
	range = max - min;
	item->slider.cursor.x = item->slider.bar.x + ((*value * range) / 255) \
						- item->slider.cursor.width / 2;
	item->slider.value = value;
}
