/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_body_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:32:11 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 18:15:45 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	cylinder_menu_create(t_container *menu)
{
	t_data	*data;

	data = menu->data;
	*menu = container_create("cylinder", NULL, CONTAINER_GRID);
	menu->data = data;
	menu_body_float_add(menu, "r");
	menu_body_float_add(menu, "h");
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
	menu_body_colors_add(menu);
	menu_body_slider_add(menu, "smoothness");
}

void	cone_menu_create(t_container *menu)
{
	t_data	*data;

	data = menu->data;
	*menu = container_create("cone", NULL, CONTAINER_GRID);
	menu->data = data;
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "h");
	menu_body_float_add(menu, "r");
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "texture");
	menu_body_bool_add(menu, "bump");
	menu_body_texture_add(menu);
	menu_body_bump_map_add(menu);
	menu_body_colors_add(menu);
	menu_body_slider_add(menu, "smoothness");
}

void	disc_menu_create(t_container *menu)
{
	t_data	*data;

	data = menu->data;
	*menu = container_create("disc", NULL, CONTAINER_GRID);
	menu->data = data;
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "texture");
	menu_body_bool_add(menu, "bump");
	menu_body_texture_add(menu);
	menu_body_bump_map_add(menu);
	menu_body_colors_add(menu);
	menu_body_slider_add(menu, "smoothness");
}

void	sphere_menu_create(t_container *menu)
{
	t_data	*data;

	data = menu->data;
	*menu = container_create("sphere", NULL, CONTAINER_GRID);
	menu->data = data;
	menu_body_vector_position_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
	menu_body_bool_add(menu, "texture");
	menu_body_bool_add(menu, "bump");
	menu_body_texture_add(menu);
	menu_body_bump_map_add(menu);
	menu_body_colors_add(menu);
	menu_body_slider_add(menu, "smoothness");
}

void	plane_menu_create(t_container *menu)
{
	t_data	*data;

	data = menu->data;
	*menu = container_create("plane", NULL, CONTAINER_GRID);
	menu->data = data;
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
	menu_body_bool_add(menu, "texture");
	menu_body_bool_add(menu, "bump");
	menu_body_texture_add(menu);
	menu_body_bump_map_add(menu);
	menu_body_colors_add(menu);
	menu_body_slider_add(menu, "smoothness");
}
