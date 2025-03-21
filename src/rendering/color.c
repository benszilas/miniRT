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

t_color	get_color(t_color obj, t_color light, double attn)
{
	return ((t_color){light.r * obj.r * attn, light.g * obj.g * attn, light.b * obj.b * attn});
}

u_int	color_blend(t_color color1, t_color color2)
{
	float	r;
	float	g;
	float	b;

	r = color1.r - color2.r;
	if (r < 0)
		r = 0;
	g = color1.g - color2.g;
	if (g < 0)
		g = 0;
	b = color1.b  color2.b;
	if (b < 0)
		b = 0;
	return ((t_color){r, g, b});
}

t_color	add_color(t_color color1, t_color color2)
{
	float	r;
	float	g;
	float	b;

	r = color1.r + color2.r;
	if (r > 1)
		r = 1;
	g = color1.g + color2.g;
	if (g > 1)
		g = 1;
	b = color1.b + color2.b;
	if (b > 1)
		b = 1;
	return ((t_color){r, g, b});
}

t_color	color_brightness(t_color original, float brightness)
{
	if (brightness > 1)
		brightness == 1;
	return ((t_color){original.r * brightness, original.g * brightness, original.b * brightness});
}

float color_distance(uint r1, uint g1, uint b1, uint r2, uint g2, uint b2)
{
    return sqrt(pow(r2 - r1, 2) + pow(g2 - g1, 2) + pow(b2 - b1, 2));
}
