/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:58:11 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 19:03:19 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

uint	set_color(uint r, uint g, uint b)
{
	return (r << 16 | g << 8 | b);
}

void	color_print(uint color, int fd)
{
	ft_fprintf(fd, " %u,%u,%u\n", (color >> 16) & 0xff, \
			(color >> 8) & 0xff, color & 0xff);
}

uint	mix_colors(uint base_color, uint reflected_color, double reflectivity)
{
	int	final_color;

	final_color = ((uint)((1.0 - reflectivity) * ((base_color >> 16) & 0xFF) \
					+ reflectivity * ((reflected_color >> 16) & 0xFF)) << 16) \
					| ((uint)((1.0 - reflectivity) * ((base_color >> 8) & 0xFF) \
					+ reflectivity * ((reflected_color >> 8) & 0xFF)) << 8) \
					| (uint)((1.0 - reflectivity) * (base_color & 0xFF) \
					+ reflectivity * (reflected_color & 0xFF));
	if (final_color > 0xffffff)
		final_color = 0xffffff;
	else if (final_color < 0)
		final_color = 0;
	return (final_color);
}

uint	parse_body_color(char *params[], int *error)
{
	return (set_color(set_signed_int(params[0], 0, UCHAR_MAX, error), \
				set_signed_int(params[1], 0, UCHAR_MAX, error), \
				set_signed_int(params[2], 0, UCHAR_MAX, error)));
}
