/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:05:49 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:05:54 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	trace_cone_bottom(t_pixel *px, t_vector ray, t_body *cone, t_scene *sc)
{
	t_body	bottom;

	ft_bzero(&bottom, sizeof (bottom));
	bottom.id = cone->id;
	bottom.color = cone->color;
	bottom.disk = cone->cone.bottom_cap;
	trace_disk(px, ray, &bottom, sc);
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
