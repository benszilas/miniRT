/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_body.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:11:56 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/04 15:12:04 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	cylinder_menu_create(t_container *menu)
{
	*menu = container_create("cylinder", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_float_add(menu, "h");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
}

void	cone_menu_create(t_container *menu)
{
	*menu = container_create("cone", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "h");
	menu_body_float_add(menu, "r");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
}

void	disc_menu_create(t_container *menu)
{
	*menu = container_create("disc", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
}

void	sphere_menu_create(t_container *menu)
{
	*menu = container_create("sphere", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
	menu_body_bool_add(menu, "texture");
}

void	plane_menu_create(t_container *menu)
{
	*menu = container_create("plane", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
	menu_body_bool_add(menu, "texture");
}
