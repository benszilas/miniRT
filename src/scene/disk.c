/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:26:27 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/06 22:02:39 by bszilas          ###   ########.fr       */
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

void get_color_texture_disk(double u, double v, t_texture *texture, t_pixel *pixel)
{
	int tex_u;
	int tex_v;

	tex_u = (int)(u * (texture->width - 1)) % texture->width;
	tex_v= (int)((1 - v) * (texture->height - 1)) % texture->height;
	if (tex_u < 0)
		tex_u += texture->width;
	if (tex_v < 0)
		tex_v += texture->height;
	*pixel->color = texture->pixel[tex_v * texture->width + tex_u]; 
}

void get_color_checker_disk(double u, double v, t_pixel *pixel)
{
    if (((int)floor(u) + (int)floor(v)) % 2 == 0)
        *pixel->color = 0x000000;
    else
        *pixel->color = 0xffffff;
}

void get_color_disk(t_body *body, t_vector intersect, t_pixel *pixel)
{
	double u;
	double v;
	t_vector right;
	t_vector up;

	if (body->reflect || (!body->textured && !body->checker_board))
	{
		*pixel->color = body->color;
		return ;
	}
    intersect = vector_subtract(intersect, body->disk.point);
    if (fabs(body->disk.normal.y) > 0.9)
    {
        right = (t_vector){1, 0, 0};
        up = (t_vector){0, 0, 1};
    }
    else
    {
        right = cross_product(body->disk.normal, (t_vector){0, 1, 0});
        up = cross_product(right, body->disk.normal);
    }
	normalize_vector(&right);
    normalize_vector(&up);
    u = dot_product(intersect, right) * .5;
    v = dot_product(intersect, up) * .5;
	if (body->textured)
		get_color_texture_disk(u - floor(u), v - floor(v), body->texture, pixel);
	else if (body->checker_board)
		get_color_checker_disk(u, v, pixel);
}

void	trace_disk(t_pixel *pixel, t_vector ray, t_body *body, t_scene *scene)
{
	t_hit_point	hit;
	double		dist;
	t_disk		disk;
	int			flip;

	disk = body->disk;
	flip = false;
	dist = disk_hit_distance(disk, ray, scene->camera.position, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		hit.p = add_vector(scene->camera.position, scale_vector(ray, dist));
		pixel->id = body->id;
		pixel->dist = dist;
		if (flip)
			disk.normal = disk.inverse_normal;
		calc_hit_point_vectors(&hit, ray, disk.normal);
		get_color_disk(body, hit.p, pixel);
		if (body->reflect && scene->depth < MAX_DEPTH)
			trace_reflection(pixel, hit, *scene);
		else
			trace_lights(scene, pixel, hit);	
	}
}
