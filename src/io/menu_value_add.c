/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_value_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:34:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 16:26:22 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	menu_body_bool_add(t_container *menu, char *name)
{
	t_item	item;

	item = container_item_button_create(name, NULL, item_bool_toggle);
	container_item_add(menu, &item);
}

void	menu_body_vector_position_add(t_container *menu)
{
	t_item	item;

	item = container_item_button_create("xpos++", NULL, item_double_inc);
	container_item_add(menu, &item);
	item = container_item_button_create("xpos--", NULL, item_double_dec);
	container_item_add(menu, &item);
	item = container_item_button_create("ypos++", NULL, item_double_inc);
	container_item_add(menu, &item);
	item = container_item_button_create("ypos--", NULL, item_double_dec);
	container_item_add(menu, &item);
	item = container_item_button_create("zpos++", NULL, item_double_inc);
	container_item_add(menu, &item);
	item = container_item_button_create("zpos--", NULL, item_double_dec);
	container_item_add(menu, &item);
}

void	menu_body_vector_normal_add(t_container *menu)
{
	t_item	item;

	item = container_item_button_create("xnorm++", NULL, item_normal_inc);
	container_item_add(menu, &item);
	item = container_item_button_create("xnorm--", NULL, item_normal_dec);
	container_item_add(menu, &item);
	item = container_item_button_create("ynorm++", NULL, item_normal_inc);
	container_item_add(menu, &item);
	item = container_item_button_create("ynorm--", NULL, item_normal_dec);
	container_item_add(menu, &item);
	item = container_item_button_create("znorm++", NULL, item_normal_inc);
	container_item_add(menu, &item);
	item = container_item_button_create("znorm--", NULL, item_normal_dec);
	container_item_add(menu, &item);
}

void	menu_body_colors_add(t_container *menu)
{
	t_item	item;

	item = container_item_slider_create("r", 0, 255, NULL);
	container_item_add(menu, &item);
	item = container_item_slider_create("g", 0, 255, NULL);
	container_item_add(menu, &item);
	item = container_item_slider_create("b", 0, 255, NULL);
	container_item_add(menu, &item);
}

void	menu_body_float_add(t_container *menu, char *name_prefix)
{
	t_item	item;
	char	name[255];

	ft_strlcpy(name, name_prefix, ft_strlen(name_prefix) + 2);
	ft_strlcat(name, "++", ft_strlen(name_prefix) + 3);
	item = container_item_button_create(name, NULL, item_double_inc);
	container_item_add(menu, &item);
	ft_strlcpy(name, name_prefix, ft_strlen(name_prefix) + 2);
	ft_strlcat(name, "--", ft_strlen(name_prefix) + 3);
	item = container_item_button_create(name, NULL, item_double_dec);
	container_item_add(menu, &item);
}
