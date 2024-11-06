/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_body_menu.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:12:54 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/04 15:12:59 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	sphere_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->sphere.center);
	menu_body_map_float(&menu->item[6], &body->sphere.radius);
	menu_body_map_color(&menu->item[8], color);
	menu_body_map_bool_toggle(&menu->item[14], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[15], &body->checker_board);
	menu_body_map_bool_toggle(&menu->item[16], &body->textured);
}

void	disk_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->disk.point);
	menu_body_map_vector(&menu->item[6], &body->disk.normal);
	menu_body_map_float(&menu->item[12], &body->disk.radius);
	menu_body_map_color(&menu->item[14], color);
	menu_body_map_bool_toggle(&menu->item[20], &body->reflect);
}

void	plane_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->plane.point);
	menu_body_map_vector(&menu->item[6], &body->plane.normal);
	menu_body_map_color(&menu->item[12], color);
	menu_body_map_bool_toggle(&menu->item[18], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[19], &body->checker_board);
	menu_body_map_bool_toggle(&menu->item[20], &body->textured);
}

void	cone_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->cone.vertex);
	menu_body_map_vector(&menu->item[6], &body->cone.normal);
	menu_body_map_float(&menu->item[12], &body->cone.radius);
	menu_body_map_float(&menu->item[14], &body->cone.height);
	menu_body_map_color(&menu->item[16], color);
	menu_body_map_bool_toggle(&menu->item[22], &body->reflect);
}

void	cylinder_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->cylinder.center);
	menu_body_map_vector(&menu->item[6], &body->cylinder.normal);
	menu_body_map_float(&menu->item[12], &body->cylinder.radius);
	menu_body_map_float(&menu->item[14], &body->cylinder.height);
	menu_body_map_color(&menu->item[16], color);
	menu_body_map_bool_toggle(&menu->item[22], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[23], &body->checker_board);
}
