/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:54:08 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:55:49 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	body_plane_print(t_body *body)
{
	t_plane	plane;

	plane = body->plane;
	ft_printf("plane:\nx: %f\ny: %f\nz: %f\nnormal:\nx: %f\n" \
			"y: %f\nz: %f\ncolor: %X\nid: %d\n\n", plane.point.x, \
			plane.point.y, plane.point.z, \
			plane.normal.x, plane.normal.y, plane.normal.z, \
			body->color, body->id);
}

void	plane_save(t_plane plane, uint color, int fd)
{
	ft_fprintf(fd, "pl %f,%f,%f %f,%f,%f", \
			plane.point.x, plane.point.y, plane.point.z, \
			plane.normal.x, plane.normal.y, plane.normal.z);
	color_print(color, fd);
}

void	get_color_checker_plane(double u, double v, t_pixel *pixel)
{
	if (((int)floor(u) + (int)floor(v)) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

void	get_color_texture_plane(double u, double v, t_texture *texture, \
		t_pixel *pixel)
{
	int	tex_u;
	int	tex_v;

	tex_u = (int)(u * (texture->width - 1)) % texture->width;
	tex_v = (int)((1 - v) * (texture->height - 1)) % texture->height;
	if (tex_u < 0)
		tex_u += texture->width;
	if (tex_v < 0)
		tex_v += texture->height;
	*pixel->color = texture->pixel[tex_v * texture->width + tex_u];
}
