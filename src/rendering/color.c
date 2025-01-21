/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 13:42:14 by victor            #+#    #+#             */
/*   Updated: 2024/11/30 06:17:06 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

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

uint	color_brightness(uint original, float brightness)
{
	uint	r;
	uint	g;
	uint	b;

	r = ((original >> 16) & 0xff) * brightness;
	if (r > 255)
		r = 255;
	g = ((original >> 8) & 0xff) * brightness;
	if (g > 255)
		g = 255;
	b = ((original) & 0xff) * brightness;
	if (b > 255)
		b = 255;
	return (set_color(r, g, b));
}

float color_distance(uint r1, uint g1, uint b1, uint r2, uint g2, uint b2)
{
    return sqrt(pow(r2 - r1, 2) + pow(g2 - g1, 2) + pow(b2 - b1, 2));
}
