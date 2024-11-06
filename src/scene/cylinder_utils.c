/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:21:40 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 14:01:04 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	body_cylinder_print(t_body *body)
{
	t_cylinder	cylinder;

	cylinder = body->cylinder;
	ft_printf("cylinder:\nx: %f\ny: %f\nz: %f\nnormal:\nx: %f\ny: %f\nz: " \
			"%f\nrad: %f\nheight: %f\ncolor: %X\nid: %d\n" \
			"top: x: %f\ny: %f\nz: %f\nbottom:x: %f\ny: %f\nz: %f\n\n", \
			cylinder.center.x, cylinder.center.y, cylinder.center.z, \
			cylinder.normal.x, cylinder.normal.y, cylinder.normal.z, \
			cylinder.radius, cylinder.height, body->color, body->id, \
			cylinder.top.x, cylinder.top.y, cylinder.top.z, \
			cylinder.bottom.x, cylinder.bottom.y, cylinder.bottom.z);
}

void	cylinder_save(t_cylinder cylinder, uint color, int fd)
{
	ft_fprintf(fd, "cy %f,%f,%f %f,%f,%f %f %f", \
			cylinder.center.x, cylinder.center.y, cylinder.center.z, \
			cylinder.normal.x, cylinder.normal.y, cylinder.normal.z, \
			cylinder.radius, cylinder.height);
	color_print(color, fd);
}

void	trace_cyl_caps(t_pixel *px, t_vector ray, t_body *cyl, t_scene *sc)
{
	t_body	cap;

	cap.id = cyl->id;
	cap.color = cyl->color;
	cap.checker_board = cyl->checker_board;
	cap.textured = cyl->textured;
	cap.texture = cyl->texture;
	cap.disk = (t_disk){.point = cyl->cylinder.top, \
		.normal = cyl->cylinder.normal, .radius = cyl->cylinder.radius, \
			.inverse_normal = scale_vector(cyl->cylinder.normal, -1)};
	trace_disk(px, ray, &cap, sc);
	cap.disk.point = cyl->cylinder.bottom;
	trace_disk(px, ray, &cap, sc);
}

double	cyl_components_shadow(t_cylinder cy, t_vector ray, t_vector p)
{
	int		misc;
	double	dist;

	misc = 0;
	dist = cyl_hit_distance(&cy, ray, p, &misc);
	dist = smaller_non_negative(dist, disk_hit_distance((t_disk){\
				.point = cy.top, .normal = cy.normal, .radius = cy.radius, \
				.inverse_normal = scale_vector(cy.normal, -1)}, \
				ray, p, &misc));
	return (smaller_non_negative(dist, disk_hit_distance((t_disk){\
					.point = cy.bottom, .normal = cy.normal, \
					.radius = cy.radius, \
					.inverse_normal = scale_vector(cy.normal, -1)}, \
					ray, p, &misc)));
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
