/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:58:26 by victor            #+#    #+#             */
/*   Updated: 2024/12/05 14:40:32 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

int	close_window(void *data_ptr)
{
	key_press(XK_Escape, data_ptr);
	return (0);
}

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
	glyph_print(10, 10, \
			"controls\nmove: wasdqe\n" \
						"rotate: arrow keys\n" \
						"move light: hjklio\t" \
						"next light: n\n\n" \
						"drag object: lmouse\nchange object: rmouse + use menu\n\n" \
						"resolution and anti-aliasing:\nnumpad + or -; Enter to reset\n" \
						"backsp: reset_cam\n" \
						"p: save file\nesc: exit\n", \
						data->pixel);
	data->func_ptr = NULL;
}
