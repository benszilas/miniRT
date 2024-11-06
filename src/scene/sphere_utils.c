/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:03:30 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 12:03:47 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	body_sphere_print(t_body *body)
{
	t_sphere	sphere;

	sphere = body->sphere;
	ft_printf("sphere:\nx: %f\ny: %f\nz: %f\nrad: %f\ncolor: %X\nid: %d\n\n", \
			sphere.center.x, sphere.center.y, sphere.center.z, \
			sphere.radius, body->color, body->id);
}

void	sphere_save(t_sphere sphere, uint color, int fd)
{
	ft_fprintf(fd, "sp %f,%f,%f %f", \
			sphere.center.x, sphere.center.y, sphere.center.z, \
			sphere.radius);
	color_print(color, fd);
}

void	get_color_checker_sphere(double u, double v, t_pixel *pixel)
{
	int	checker_u;
	int	checker_v;

	checker_u = round(u * CHECKER_GRID_SIZE);
	checker_v = round(v * CHECKER_GRID_SIZE);
	if ((checker_u + checker_v) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

void	get_color_texture_sphere(double u, double v, \
								t_texture *texture, t_pixel *pixel)
{
	int	tex_u;
	int	tex_v;

	tex_u = (int)(u * (texture->width - 1)) % texture->width;
	tex_v = (int)(v * (texture->height - 1)) % texture->height;
	if (tex_u < 0)
		tex_u += texture->width;
	if (tex_v < 0)
		tex_v += texture->height;
	*pixel->color = texture->pixel[tex_v * texture->width + tex_u];
}
