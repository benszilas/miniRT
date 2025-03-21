/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:26:27 by bszilas           #+#    #+#             */
/*   Updated: 2024/12/16 16:24:38 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	move_disk(int keycode, t_disk *disk)
{
	if (keycode == XK_Up || keycode == XK_Down)
		disk->normal = rot_x(disk->normal, keycode - XK_Up - 1);
	else if (keycode == XK_Left || keycode == XK_Right)
		disk->normal = rot_y(disk->normal, keycode - XK_Left - 1);
	else if (keycode == XK_1 || keycode == XK_3)
		disk->normal = rot_z(disk->normal, keycode - XK_1 - 1);
	else if (!key_move_focused(keycode, &disk->point))
		return (false);
	disk->inverse_normal = scale_vector(disk->normal, -1);
	return (true);
}

void	print_disk(t_body *body)
{
	t_disk	disk;

	disk = body->disk;
	ft_printf("disk:\nx: %f\ny: %f\nz: %f\nnormal:\nx: %f\ny: %f" \
			"\nz: %f\nrad: %f\ncolor: %X\nid: %d\n\n", disk.point.x, \
			disk.point.y, disk.point.z, disk.normal.x, disk.normal.y, \
			disk.normal.z, disk.radius, body->color, body->id);
}

void	disk_save(t_disk disk, t_color color, int fd)
{
	ft_fprintf(fd, "di %f,%f,%f %f,%f,%f %f", \
		disk.point.x, disk.point.y, disk.point.z, \
		disk.normal.x, disk.normal.y, disk.normal.z, disk.radius * 2);
	color_print(color, fd);
}

bool	parse_disk(char *entry, uint line_count, t_body *body, uint body_count)
{
	char	*params[10];
	int		error;

	error = false;
	ft_bzero(params, sizeof(params));
	while (ft_isspace(*entry))
		entry++;
	if (!collect_param_comma(&entry, params, 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[3], 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_single(&entry, &params[6], COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[7], 2, COLLECT_MODE_END))
		return (err("disk", line_count), false);
	body->type = BODY_DISK;
	body->disk = (t_disk){.point.x = ft_atod(params[0]), \
		.point.y = ft_atod(params[1]), .point.z = ft_atod(params[2]), \
		.normal.x = ft_atod(params[3]), .normal.y = ft_atod(params[4]), \
		.normal.z = ft_atod(params[5]), .radius = ft_atod(params[6]) / 2};
	body->color = parse_body_color(params + 7, &error);
	body->id = id_set(ID_GROUP_DISC, body_count);
	if (error || !check_normal_vector(&body->disk.normal) || \
		body->disk.radius <= 0)
		return (err("disk", line_count), false);
	normalize_vector(&body->disk.normal);
	body->disk.inverse_normal = scale_vector(body->disk.normal, -1);
	return (true);
}

void	trace_disk(t_pixel *pixel, t_vector ray, t_body *body, t_scene *scene)
{
	t_hit_point	hit;
	double		dist;
	t_disk		disk;
	int			invert;

	disk = body->disk;
	invert = false;
	dist = disk_hit_distance(disk, ray, scene->camera.position, &invert);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		hit.p = add_vector(scene->camera.position, scale_vector(ray, dist));
		set_info_to_pixel(pixel, body, dist);
		if (invert)
			disk.normal = disk.inverse_normal;
		calc_hit_point_vectors(&hit, ray, disk.normal);
		get_color_disk(body, hit.p, pixel);
		if (body->reflect && scene->depth < MAX_DEPTH)
			trace_reflection(pixel, hit, *scene);
		else
			trace_lights(scene, pixel, hit);
	}
}
