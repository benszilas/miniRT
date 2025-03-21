/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:12:28 by victor            #+#    #+#             */
/*   Updated: 2024/09/17 17:51:10 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	rt_draw_rect(t_rect rect, t_pixel *pixel, uint id, uint rgb_color)
{
	uint	x;
	uint	y;

	y = 0;
	while (y < rect.height && rect.y + y < HI)
	{
		x = 0;
		while (x < rect.width && rect.x + x < WI)
		{
			*(int *)pixel[(rect.y + y) * WI + (rect.x + x)].color = rgb_color;
			pixel[(rect.y + y) * WI + (rect.x + x)].id = id;
			x++;
		}
		y++;
	}
}

void	rt_draw_rect_blend(t_rect rect, t_pixel *pixel, uint id, t_color color)
{
	uint	x;
	uint	y;

	y = 0;
	while (y < rect.height && rect.y + y < HI)
	{
		x = 0;
		while (x < rect.width && rect.x + x < WI)
		{
			uint coord = (rect.y + y) * WI + (rect.x + x);
			uint* colorptr = pixel[coord].color;
			*colorptr = float_to_rgb(color_blend(set_color(colorptr->r, colorptr->g, colorptr->b), color));
			pixel[coord].id = id;
			x++;
		}
		y++;
	}
}
