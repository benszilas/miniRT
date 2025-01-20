/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_body_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:29:46 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/11 12:21:44 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	sphere_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->sphere.center);
	menu_body_map_float(&menu->item[6], &body->sphere.radius);
	menu_body_map_bool_toggle(&menu->item[8], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[9], &body->checker_board);
	menu_body_map_bool_toggle(&menu->item[10], &body->textured);
	menu_body_map_bool_toggle(&menu->item[11], &body->bump);
	menu_body_map_texture(&menu->item[12], &body->texture);
	menu_body_map_bump(&menu->item[13], &body->map);
	menu_body_map_color(&menu->item[14], color);
	menu_body_slider_map_value(&menu->item[17], &body->surface_smoothness);
}

void	disk_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->disk.point);
	menu_body_map_vector(&menu->item[6], &body->disk.normal);
	menu_body_map_float(&menu->item[12], &body->disk.radius);
	menu_body_map_bool_toggle(&menu->item[14], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[15], &body->textured);
	menu_body_map_bool_toggle(&menu->item[16], &body->bump);
	menu_body_map_texture(&menu->item[17], &body->texture);
	menu_body_map_bump(&menu->item[18], &body->map);
	menu_body_map_color(&menu->item[19], color);
	menu_body_slider_map_value(&menu->item[22], &body->surface_smoothness);
}

void	plane_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->plane.point);
	menu_body_map_vector(&menu->item[6], &body->plane.normal);
	menu_body_map_bool_toggle(&menu->item[12], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[13], &body->checker_board);
	menu_body_map_bool_toggle(&menu->item[14], &body->textured);
	menu_body_map_bool_toggle(&menu->item[15], &body->bump);
	menu_body_map_texture(&menu->item[16], &body->texture);
	menu_body_map_bump(&menu->item[17], &body->map);
	menu_body_map_color(&menu->item[18], color);
	menu_body_slider_map_value(&menu->item[21], &body->surface_smoothness);
}

void	cone_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->cone.vertex);
	menu_body_map_vector(&menu->item[6], &body->cone.normal);
	menu_body_map_float(&menu->item[12], &body->cone.radius);
	menu_body_map_float(&menu->item[14], &body->cone.height);
	menu_body_map_bool_toggle(&menu->item[16], &body->reflect);
	menu_body_map_color(&menu->item[17], color);
	menu_body_slider_map_value(&menu->item[20], &body->surface_smoothness);
	menu->param.param = &body->cone;
	menu->param.func = (void (*)(void *))calc_cone_data;
	menu->param.type = PARAM_CONE;
}

void	cylinder_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_float(&menu->item[0], &body->cylinder.radius);
	menu_body_map_float(&menu->item[2], &body->cylinder.height);
	menu_body_map_bool_toggle(&menu->item[4], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[5], &body->checker_board);
	menu_body_map_color(&menu->item[6], color);
	menu_body_slider_map_value(&menu->item[9], &body->surface_smoothness);
	menu->param.param = &body->cylinder;
	menu->param.func = (void (*)(void *))calc_cyl_data;
	menu->param.type = PARAM_CYL;
}
