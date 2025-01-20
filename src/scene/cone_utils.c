/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:18:57 by bszilas           #+#    #+#             */
/*   Updated: 2024/12/04 16:48:53 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	finite_cone_hit(double cone_height, double h)
{
	return (h >= 0 && h <= cone_height);
}

bool	cam_inside_cone(t_cone *cn, t_vector cam, double cam_h)
{
	t_vector	p;
	double		r;

	p = add_vector(cn->vertex, scale_vector(cn->normal, cam_h));
	r = cn->radius * (cam_h / cn->height);
	return (vector_length(vector_subtract(p, cam)) < r);
}

t_vector	cone_surface_normal(t_cone cn, t_vector p, int invert)
{
	return (scale_vector(get_normal(p, add_vector(cn.vertex, \
	scale_vector(cn.normal, cn.hit_h * cn.tan_a2p1))), invert));
}

void	calc_cone_data(t_cone *cn)
{
	t_disk	*bottom;

	bottom = &cn->bottom_cap;
	normalize_vector(&cn->normal);
	cn->tan_a2p1 = pow(cn->radius / cn->height, 2) + 1;
	bottom->point = add_vector(cn->vertex, \
	scale_vector(cn->normal, cn->height));
	bottom->normal = cn->normal;
	bottom->inverse_normal = scale_vector(cn->normal, -1);
	bottom->radius = cn->radius;
}

double	cone_components_shadow(t_cone cn, t_vector ray, t_vector p)
{
	int		misc;
	double	dist;

	misc = 0;
	dist = cone_hit_distance(&cn, ray, p, &misc);
	return (smaller_non_negative(dist, disk_hit_distance(cn.bottom_cap, \
				ray, p, &misc)));
}
