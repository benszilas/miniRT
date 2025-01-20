/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_calc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:29:23 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 18:12:40 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

double	plane_hit_distance(t_plane pl, t_vector cam, \
							t_vector camera_ray, int *invert)
{
	t_vector	body_delta;
	double		projection;

	projection = dot_product(pl.normal, camera_ray);
	body_delta = vector_subtract(pl.point, cam);
	if (projection > 0)
		*invert = true;
	if (projection != 0)
		return (dot_product(body_delta, pl.normal) / projection);
	return (-1);
}

void	get_color_texture_plane(double u, double v, \
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

void	get_color_checker_plane(double u, double v, t_pixel *pixel)
{
	if (((int)floor(u) + (int)floor(v)) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

void	calc_up_down_plane(t_body *body, t_vector *right, t_vector *up)
{
	if (fabs(body->plane.normal.y) > 0.9)
	{
		*right = (t_vector){1, 0, 0};
		*up = (t_vector){0, 0, 1};
	}
	else
	{
		*right = cross_product(body->plane.normal, (t_vector){0, 1, 0});
		*up = cross_product(*right, body->plane.normal);
	}
	normalize_vector(right);
	normalize_vector(up);
}

void	get_color_plane(t_body *body, t_hit_point *hit, t_pixel *pixel)
{
	double		u;
	double		v;
	t_vector	right;
	t_vector	up;
	t_vector	intersect;

	*pixel->color = body->color;
	if (body->reflect || (!body->textured \
				&& !body->checker_board && !body->bump))
		return ;
	intersect = vector_subtract(hit->p, body->plane.point);
	calc_up_down_plane(body, &right, &up);
	u = dot_product(intersect, right) * .5;
	v = dot_product(intersect, up) * .5;
	if (body->textured)
		get_color_texture_plane(u - floor(u), \
				v - floor(v), body->texture, pixel);
	else if (body->checker_board)
		get_color_checker_plane(u, v, pixel);
	if (body->bump)
		perturb_normal(&hit->n, body->map, u - floor(u), v - floor(v));
}
