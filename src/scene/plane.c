/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:27:02 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 16:25:12 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	move_plane(int keycode, t_plane *plane)
{
	if (keycode == XK_Up || keycode == XK_Down)
		plane->normal = rot_x(plane->normal, keycode - XK_Up - 1);
	else if (keycode == XK_Left || keycode == XK_Right)
		plane->normal = rot_y(plane->normal, keycode - XK_Left - 1);
	else if (keycode == XK_1 || keycode == XK_3)
		plane->normal = rot_z(plane->normal, keycode - XK_1 - 1);
	else if (!key_move_focused(keycode, &plane->point))
		return (false);
	plane->inverse_normal = scale_vector(plane->normal, -1);
	return (true);
}

void	body_plane_print(t_body *body)
{
	t_plane	plane;

	plane = body->plane;
	ft_printf("plane:\nx: %f\ny: %f\nz: %f\nnormal:\nx: %f\n" \
			"y: %f\nz: %f\ncolor: %X\nid: %d\n\n", plane.point.x, \
			plane.point.y, plane.point.z, \
			plane.normal.x, plane.normal.y, plane.normal.z, \
			body->color, body->id);
}

bool	parse_plane(char *entry, uint line_count, \
					t_body *body, uint body_count)
{
	char	*params[9];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	while (ft_isspace(*entry))
		entry++;
	if (!collect_param_comma(&entry, params, 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[3], 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[6], 2, COLLECT_MODE_END))
		return (err("plane", line_count), false);
	body->type = BODY_PLANE;
	body->plane = (t_plane){
		.point.x = ft_atod(params[0]), .point.y = ft_atod(params[1]), \
		.point.z = ft_atod(params[2]), .normal.x = ft_atod(params[3]), \
		.normal.y = ft_atod(params[4]), .normal.z = ft_atod(params[5])};
	body->color = parse_body_color(params + 6, &error);
	body->id = id_set(ID_GROUP_PLANE, body_count);
	if (error || !check_normal_vector(&body->plane.normal))
		return (err("plane", line_count), false);
	normalize_vector(&body->plane.normal);
	body->plane.inverse_normal = scale_vector(body->plane.normal, -1);
	return (true);
}

void	plane_save(t_plane plane, uint color, int fd)
{
	ft_fprintf(fd, "pl %f,%f,%f %f,%f,%f", \
			plane.point.x, plane.point.y, plane.point.z, \
			plane.normal.x, plane.normal.y, plane.normal.z);
	color_print(color, fd);
}

void	trace_plane(t_pixel *pixel, t_vector ray, \
						t_body *body, t_scene *scene)
{
	t_hit_point	hit;
	double		dist;
	t_plane		plane;
	int			invert;

	plane = body->plane;
	invert = false;
	dist = plane_hit_distance(plane, scene->camera.position, ray, &invert);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		set_info_to_pixel(pixel, body, dist);
		hit.p = add_vector(scene->camera.position, scale_vector(ray, dist));
		if (invert)
			plane.normal = plane.inverse_normal;
		calc_hit_point_vectors(&hit, ray, plane.normal);
		get_color_plane(body, &hit, pixel);
		if (body->reflect && scene->depth < MAX_DEPTH)
			trace_reflection(pixel, hit, *scene);
		else
			trace_lights(scene, pixel, hit);
	}
}
