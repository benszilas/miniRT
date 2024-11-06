/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container_item.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:26:55 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/04 15:50:04 by bszilas          ###   ########.fr       */
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
