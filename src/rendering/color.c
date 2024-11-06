/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 13:42:14 by victor            #+#    #+#             */
/*   Updated: 2024/11/05 10:44:42 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

uint	set_color(uint r, uint g, uint b)
{
	return (0xFF << 24 | r << 16 | g << 8 | b);
}

void	color_print(uint color, int fd)
{
	ft_fprintf(fd, " %u,%u,%u\n", (color >> 16) & 0xff, \
			(color >> 8) & 0xff, color & 0xff);
}

uint	get_color(uint obj, uint light, double attn)
{
	int32_t	r;
	int32_t	g;
	int32_t	b;

	r = (double)((light >> 16 & 0xFF) * (obj >> 16 & 0xFF)) / 255 * attn;
	if (r > 255)
		r = 255;
	g = (double)((light >> 8 & 0xFF) * (obj >> 8 & 0xFF)) / 255 * attn;
	if (g > 255)
		g = 255;
	b = (double)((light & 0xFF) * (obj & 0xFF)) / 255 * attn;
	if (b > 255)
		b = 255;
	return (set_color(r, g, b));
}

uint	color_blend(uint color1, uint color2)
{
	int	r;
	int	g;
	int	b;

	r = ((color1 >> 16) & 0xff) - ((color2 >> 16) & 0xff);
	if (r < 0)
		r = 0;
	else if (r > 255)
		r = 255;
	g = ((color1 >> 8) & 0xff) - ((color2 >> 8) & 0xff);
	if (g < 0)
		g = 0;
	else if (g > 255)
		g = 255;
	b = ((color1) & 0xff) - ((color2) & 0xff);
	if (b < 0)
		b = 0;
	else if (b > 255)
		b = 255;
	return (set_color(r, g, b));
}

uint	add_color(uint color1, uint color2)
{
	uint	r;
	uint	g;
	uint	b;

	r = ((color1 >> 16) & 0xff) + ((color2 >> 16) & 0xff);
	if (r > 255)
		r = 255;
	g = ((color1 >> 8) & 0xff) + ((color2 >> 8) & 0xff);
	if (g > 255)
		g = 255;
	b = ((color1) & 0xff) + ((color2) & 0xff);
	if (b > 255)
		b = 255;
	return (set_color(r, g, b));
}
