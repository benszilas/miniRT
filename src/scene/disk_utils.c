/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:40:13 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:47:24 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	print_disk(t_body *body)
{
	t_disk	disk;

	disk = body->disk;
	ft_printf("disk:\nx: %f\ny: %f\nz: %f\nnormal:\nx: %f\ny: %f" \
			"\nz: %f\nrad: %f\ncolor: %X\nid: %d\n\n", disk.point.x, \
			disk.point.y, disk.point.z, disk.normal.x, disk.normal.y, \
			disk.normal.z, disk.radius, body->color, body->id);
}

void	get_color_texture_disk(double u, double v, t_texture *texture, \
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

void	get_color_checker_disk(double u, double v, t_pixel *pixel)
{
	if (((int)floor(u) + (int)floor(v)) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

void	get_color_disk(t_body *body, t_vector intersect, t_pixel *pixel)
{
	double		u;
	double		v;
	t_vector	right;
	t_vector	up;

	if (!body->textured && !body->checker_board)
	{
		*pixel->color = body->color;
		return ;
	}
	intersect = vector_subtract(intersect, body->disk.point);
	calc_object_space(body->disk.normal, &right, &up);
	u = dot_product(intersect, right) * .5;
	v = dot_product(intersect, up) * .5;
	if (body->textured)
		get_color_texture_disk(u - floor(u), v - floor(v), body->texture, \
		pixel);
	else if (body->checker_board)
		get_color_checker_disk(u, v, pixel);
}
