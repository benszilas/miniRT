/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:58:26 by victor            #+#    #+#             */
/*   Updated: 2024/11/04 15:17:33 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	color_dec(void *null, void *color)
{
	(void)null;
	if (*(uint8_t *)color > 15)
		*(uint8_t *)color -= 15;
	else
		*(uint8_t *)color = 0;
}

void	color_inc(void *null, void *color)
{
	(void)null;
	if (*(uint8_t *)color < 255 - 16)
		*(uint8_t *)color += 15;
	else
		*(uint8_t *)color = 255;
}

void	help_menu_draw(void *data_ptr, void *pixel)
{
	t_rect	background;
	t_data	*data;

	background = (t_rect){.x = 5, .y = 5, .width = 400, .height = 220, };
	(void)pixel;
	data = data_ptr;
	rt_draw_rect_blend(background, data->pixel, 0, HELP_MENU_BG);
	glyph_print(10, 10, "controls\na: focus.x-- d: focus.x++\n" \
						"s: focus.y-- w: focus.y++\n" \
						"q: focus.z-- e: focus.z++\n" \
						"lmouse: grab \nrmouse: new focus\n" \
						"h: light.x-- l: light.x++\n" \
						"k: light.y-- j: light.y++\n" \
						"o: light.z-- i: light.z++\n" \
						"+: res++     -: res--\n" \
						"enter: sharp\nbacksp: reset_cam\n" \
						"p: save  esc: exit\n", data->pixel);
	data->func_ptr = NULL;
}
