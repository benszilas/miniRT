/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_info.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:09:17 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/17 12:07:42 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	finite_cylinder_hit(t_cylinder *cy, t_vector hit)
{
	cy->hit_h = dot_product(vector_subtract(hit, cy->bottom), cy->normal);
	return (cy->hit_h <= cy->height && cy->hit_h >= 0);
}

bool	parse_cylinder(char *entry, uint line_c, t_body *body, uint body_cursor)
{
	char	*params[11];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	if (!collect_param_comma(&entry, params, 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[3], 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_single(&entry, &params[6], COLLECT_MODE_NORMAL) \
		|| !collect_param_single(&entry, &params[7], COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[8], 2, COLLECT_MODE_END))
		return (err("cylinder", line_c), false);
	body->type = BODY_CYLINDER;
	body->cylinder = (t_cylinder){\
		.center = set_vector(ft_atod(params[0]), ft_atod(params[1]), \
		ft_atod(params[2])), .normal = set_vector(ft_atod(params[3]), \
		ft_atod(params[4]), ft_atod(params[5])), \
		.radius = ft_atod(params[6]) / 2, .height = ft_atod(params[7])};
	body->color = parse_body_color(params + 8, &error);
	body->id = id_set(ID_GROUP_CYLINDER, body_cursor);
	if (error || body->cylinder.radius <= 0 || body->cylinder.height <= 0 || \
		!check_normal_vector(&body->cylinder.normal))
		return (err("cylinder", line_c), false);
	calc_cyl_data(&body->cylinder);
	return (true);
}

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

void	cylinder_save(t_cylinder cylinder, t_color color, int fd)
{
	ft_fprintf(fd, "cy %f,%f,%f %f,%f,%f %f %f", \
			cylinder.center.x, cylinder.center.y, cylinder.center.z, \
			cylinder.normal.x, cylinder.normal.y, cylinder.normal.z, \
			cylinder.radius * 2, cylinder.height);
	color_print(color, fd);
}
