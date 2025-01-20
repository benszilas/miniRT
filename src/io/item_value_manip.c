/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item_value_manip.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:29:13 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/11 12:29:42 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	item_bool_toggle(void *null, void *param)
{
	(void)null;
	*((bool *)((t_param *)param)->param) ^= 1;
}

void	item_normal_inc(void *null, void *param)
{
	(void)null;
	*(double *)((t_param *)param)->param += .1;
	if (*(double *)((t_param *)param)->param > 1)
		*(double *)((t_param *)param)->param = 1;
}

void	item_normal_dec(void *null, void *param)
{
	(void)null;
	*(double *)((t_param *)param)->param -= .1;
	if (*(double *)((t_param *)param)->param < -1)
		*(double *)((t_param *)param)->param = -1;
}

void	item_double_inc(void *null, void *param)
{
	(void)null;
	*(double *)((t_param *)param)->param = \
		*(double *)((t_param *)param)->param + 1;
}

void	item_double_dec(void *null, void *param)
{
	(void)null;
	if (*(double *)((t_param *)param)->param - 1.0 <= 0 \
		&& ((t_param *)param)->type == PARAM_RADIUS)
		return ;
	*(double *)((t_param *)param)->param = \
			*(double *)((t_param *)param)->param - 1;
}
