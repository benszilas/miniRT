/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:26:27 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:40:56 by bszilas          ###   ########.fr       */
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

bool	parse_disk(char *entry, uint line_count, t_body *body, uint body_count)
{
	char	*params[10];
	int		error;

	error = false;
	ft_bzero(params, sizeof(params));
	while (ft_isspace(*entry))
		entry++;
	if (!collect_param_comma(&entry, params, 2) \
		|| !collect_param_comma(&entry, &params[3], 2) \
		|| !collect_param_single(&entry, &params[6]) \
		|| !collect_param_comma(&entry, &params[7], 2))
		return (err("disk", line_count), lst_memory(NULL, NULL, FAIL), false);
	body->type = BODY_DISK;
	body->disk = (t_disk){
		.point.x = ft_atod(params[0]), .point.y = ft_atod(params[1]), \
		.point.z = ft_atod(params[2]), .normal.x = ft_atod(params[3]), \
		.normal.y = ft_atod(params[4]), .normal.z = ft_atod(params[5]), \
		.radius = ft_atod(params[6]) / 2};
	body->color = parse_body_color(params + 7, &error);
	body->id = id_set(ID_GROUP_DISC, body_count);
	if (error)
		return (err("disk", line_count), lst_memory(NULL, NULL, FAIL), false);
	normalize_vector(&body->disk.normal);
	body->disk.inverse_normal = scale_vector(body->disk.normal, -1);
	return (true);
}

double	disk_hit_distance(t_disk disk, t_vector ray, t_vector cam, int *flip)
{
	t_vector	delta;
	double		projection;
	double		dist;
	t_vector	p;

	projection = dot_product(disk.normal, ray);
	if (fabs(projection) < 1e-6)
		return (-1);
	delta = vector_subtract(disk.point, cam);
	dist = dot_product(delta, disk.normal) / projection;
	if (projection > 0)
		*flip = true;
	if (dist > SHADOW_BIAS)
	{
		p = add_vector(cam, scale_vector(ray, dist));
		delta = vector_subtract(p, disk.point);
		if (dot_product(delta, delta) <= pow(disk.radius, 2))
			return (dist);
	}
	return (-1);
}

void	trace_disk(t_pixel *pixel, t_vector ray, t_body *body, t_scene *scene)
{
	t_vector	p;
	double		attn;
	double		dist;
	t_disk		disk;
	int			flip;

	disk = body->disk;
	flip = false;
	dist = disk_hit_distance(disk, ray, scene->camera.position, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		pixel->id = body->id;
		if (flip)
			disk.normal = disk.inverse_normal;
		p = add_vector(scene->camera.position, scale_vector(ray, dist));
		get_color_disk(body, p, pixel);
		attn = get_color_attenuation(p, disk.normal, scene->light, scene);
		set_hit_pixel(scene, pixel, attn, dist);
	}
}
