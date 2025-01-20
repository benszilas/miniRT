/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk_calc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:15:11 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/04 11:29:48 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

double	disk_hit_distance(t_disk disk, t_vector ray, t_vector cam, int *invert)
{
	t_vector	delta;
	double		projection;
	double		dist;
	t_vector	p;

	projection = dot_product(disk.normal, ray);
	if (fabs(projection) < SHADOW_BIAS)
		return (-1);
	delta = vector_subtract(disk.point, cam);
	dist = dot_product(delta, disk.normal) / projection;
	if (projection > 0)
		*invert = true;
	if (dist > SHADOW_BIAS)
	{
		p = add_vector(cam, scale_vector(ray, dist));
		delta = vector_subtract(p, disk.point);
		if (dot_product(delta, delta) <= pow(disk.radius, 2))
			return (dist);
	}
	return (-1);
}

void	get_color_texture_disk(double u, double v, \
							t_texture *texture, t_pixel *pixel)
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

void	calc_up_and_down(t_body *body, t_vector *right, t_vector *up)
{
	if (fabs(body->disk.normal.y) > 0.9)
	{
		*right = (t_vector){1, 0, 0};
		*up = (t_vector){0, 0, 1};
	}
	else
	{
		*right = cross_product(body->disk.normal, (t_vector){0, 1, 0});
		*up = cross_product(*right, body->disk.normal);
	}
	normalize_vector(right);
	normalize_vector(up);
}

void	get_color_disk(t_body *body, t_vector intersect, t_pixel *pixel)
{
	double		u;
	double		v;
	t_vector	right;
	t_vector	up;

	if (body->reflect || (!body->textured && !body->checker_board))
	{
		*pixel->color = body->color;
		return ;
	}
	calc_up_and_down(body, &right, &up);
	intersect = vector_subtract(intersect, body->disk.point);
	u = dot_product(intersect, right) * .5;
	v = dot_product(intersect, up) * .5;
	if (body->textured)
		get_color_texture_disk(u - floor(u), v - floor(v), \
				body->texture, pixel);
	else if (body->checker_board)
		get_color_checker_disk(u, v, pixel);
}
