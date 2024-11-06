/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_menu_item.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:17:21 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/04 15:17:59 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	item_normal_inc(void *null, void *value)
{
	(void)null;
	*(double *)value += .1;
	if (*(double *)value > 1)
		*(double *)value = 1;
}

void	item_normal_dec(void *null, void *value)
{
	(void)null;
	*(double *)value -= .1;
	if (*(double *)value < -1)
		*(double *)value = -1;
}

void	item_double_inc(void *null, void *value)
{
	(void)null;
	*(double *)value += *(double *)value / 10;
}

void	item_double_dec(void *null, void *value)
{
	(void)null;
	*(double *)value -= *(double *)value / 10;
}

void	item_bool_toggle(void *null, void *value)
{
	(void)null;
	*(bool *)value ^= 1;
}
