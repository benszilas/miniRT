/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_file_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:21:39 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 14:40:53 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	menu_body_map_texture(t_item *item, t_texture **texture)
{
	item->param.param = texture;
	item->param.type = PARAM_TEXTURE;
}

void	menu_body_map_bump(t_item *item, t_bump_map **bump)
{
	item->param.param = bump;
	item->param.type = PARAM_BUMP;
}

void	menu_body_bump_map_add(t_container *container)
{
	t_item	item;

	item = container_item_button_create("Load Bump", NULL, explorer_read_dir);
	container_item_add(container, &item);
}

void	menu_body_texture_add(t_container *container)
{
	t_item	item;

	item = container_item_button_create("Load Tex", NULL, explorer_read_dir);
	container_item_add(container, &item);
}
