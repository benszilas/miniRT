/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:24:05 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/27 23:00:21 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

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

void	get_color_sphere(	t_body *body, \
							t_hit_point	*hit, \
							t_pixel *pixel)
{
	double		theta;
	double		phi;
	double		u;
	double		v;
	t_vector	intersect;

	*pixel->color = body->color;
	if (body->reflect || (!body->textured && !body->checker_board \
				&& body->bump == false))
		return ;
	intersect = vector_subtract(hit->p, body->sphere.center);
	theta = atan2(intersect.x, intersect.z);
	phi = acos(intersect.y / body->sphere.radius);
	v = (phi / M_PI);
	u = (theta + M_PI) / (2 * M_PI);
	u = (1 - (u + 0.5));
	if (body->checker_board)
		get_color_checker_sphere(u, v, pixel);
	else if (body->texture && body->textured == true)
		get_color_texture_sphere(u, v, body->texture, pixel);
	if (body->bump)
		perturb_normal(&hit->n, body->map, u, v);
}
