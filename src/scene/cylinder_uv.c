/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_uv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:05:49 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/26 19:06:21 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	get_color_checker_cylinder(double u, double v, t_pixel *pixel)
{
	int	checker_u;
	int	checker_v;

	checker_u = (int)(u * 16);
	checker_v = (int)(v * 8);
	if ((checker_u + checker_v) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

t_vector	get_local(t_cylinder *cylinder, t_vector intersect)
{
	t_vector	u_axis;
	t_vector	v_axis;
	t_vector	delta;
	t_vector	local;

	if (fabs(cylinder->normal.x) > 0.0001 || fabs(cylinder->normal.z) > 0.0001)
		u_axis = (t_vector){0, 1, 0};
	else
		u_axis = (t_vector){1, 0, 0};
	u_axis = cross_product(u_axis, cylinder->normal);
	normalize_vector(&u_axis);
	v_axis = cross_product(cylinder->normal, u_axis);
	delta = vector_subtract(intersect, cylinder->center);
	local.x = dot_product(delta, u_axis);
	local.y = dot_product(delta, cylinder->normal);
	local.z = dot_product(delta, v_axis);
	return (local);
}

void	get_color_cylinder(t_body *body, t_vector intersect, t_pixel *pixel)
{
	double		u;
	double		v;
	double		theta;
	t_vector	local;

	if (body->reflect || (!body->textured && !body->checker_board))
	{
		*pixel->color = body->color;
		return ;
	}
	local = get_local(&body->cylinder, intersect);
	theta = atan2(local.z, local.x);
	u = (theta + M_PI) / (2 * M_PI);
	v = (local.y - (body->cylinder.height / 2)) / body->cylinder.height;
	v = fmod(v, 1.0);
	if (v < 0)
		v += 1.0;
	if (body->checker_board)
		get_color_checker_cylinder(u, v, pixel);
}
