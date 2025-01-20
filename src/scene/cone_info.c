/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:16:11 by bszilas           #+#    #+#             */
/*   Updated: 2024/12/16 16:21:09 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	cone_save(t_cone cone, uint color, int fd)
{
	ft_fprintf(fd, "cn %f,%f,%f %f,%f,%f %f %f", \
		cone.vertex.x, cone.vertex.y, cone.vertex.z, \
		cone.normal.x, cone.normal.y, cone.normal.z, \
		cone.radius * 2, cone.height);
	color_print(color, fd);
}

bool	move_cone(int keycode, t_cone *cone)
{
	if (keycode == XK_Up || keycode == XK_Down)
		cone->normal = rot_x(cone->normal, keycode - XK_Up - 1);
	else if (keycode == XK_Left || keycode == XK_Right)
		cone->normal = rot_y(cone->normal, keycode - XK_Left - 1);
	else if (keycode == XK_1 || keycode == XK_3)
		cone->normal = rot_z(cone->normal, keycode - XK_1 - 1);
	else if (!key_move_focused(keycode, &cone->vertex))
		return (false);
	calc_cone_data(cone);
	return (true);
}

bool	parse_cone(char *entry, uint line_count, t_body *body, uint body_count)
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
		return (err("cone", line_count), false);
	body->type = BODY_CONE;
	body->cone = (t_cone){.vertex = set_vector(ft_atod(params[0]), \
	ft_atod(params[1]), ft_atod(params[2])), .normal = set_vector(\
	ft_atod(params[3]), ft_atod(params[4]), ft_atod(params[5])), \
	.radius = ft_atod(params[6]) / 2, .height = ft_atod(params[7])};
	body->color = parse_body_color(params + 8, &error);
	body->id = id_set(ID_GROUP_CONE, body_count);
	if (error || body->cone.radius <= 0 || body->cone.height <= 0 || \
		!check_normal_vector(&body->cone.normal))
		return (err("cone", line_count), false);
	calc_cone_data(&body->cone);
	return (true);
}
