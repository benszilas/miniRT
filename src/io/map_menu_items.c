/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_meu_items.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:15:31 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/04 15:15:37 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	menu_body_map_color(t_item *item, uint *color)
{
	item[0].param = (uint8_t *)color + 2;
	item[1].param = (uint8_t *)color + 2;
	item[2].param = (uint8_t *)color + 1;
	item[3].param = (uint8_t *)color + 1;
	item[4].param = (uint8_t *)color + 0;
	item[5].param = (uint8_t *)color + 0;
}

void	menu_body_map_vector(t_item *item, t_vector *vector)
{
	item[0].param = &vector->x;
	item[1].param = &vector->x;
	item[2].param = &vector->y;
	item[3].param = &vector->y;
	item[4].param = &vector->z;
	item[5].param = &vector->z;
}

void	menu_body_map_float(t_item *item, double *value)
{
	item[0].param = value;
	item[1].param = value;
}

void	menu_body_map_bool_toggle(t_item *item, bool *value)
{
	item[0].param = value;
}
