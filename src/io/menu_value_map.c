/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_value_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:36:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/11 12:30:10 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	menu_body_map_color(t_item *item, uint *color)
{
	menu_body_slider_map_value(&item[0], (uint8_t *)color + 2);
	menu_body_slider_map_value(&item[1], (uint8_t *)color + 1);
	menu_body_slider_map_value(&item[2], (uint8_t *)color + 0);
}

void	menu_body_map_vector(t_item *item, t_vector *vector)
{
	item[0].param.param = &vector->x;
	item[1].param.param = &vector->x;
	item[2].param.param = &vector->y;
	item[3].param.param = &vector->y;
	item[4].param.param = &vector->z;
	item[5].param.param = &vector->z;
}

void	menu_body_map_float(t_item *item, double *value)
{
	item[0].param.param = value;
	item[0].param.type = PARAM_RADIUS;
	item[1].param.param = value;
	item[1].param.type = PARAM_RADIUS;
}

void	menu_body_map_bool_toggle(t_item *item, bool *value)
{
	item[0].param.param = value;
}
