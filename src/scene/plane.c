/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:27:02 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/21 21:37:49 by bszilas          ###   ########.fr       */
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

bool	parse_plane(char *entry, uint line_count, t_body *body, uint body_count)
{
	char	*params[9];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	while (ft_isspace(*entry))
		entry++;
	if (!collect_param_comma(&entry, params, 2) \
		|| !collect_param_comma(&entry, &params[3], 2) \
		|| !collect_param_comma(&entry, &params[6], 2))
		return (err("plane", line_count), lst_memory(NULL, NULL, FAIL), false);
	body->type = BODY_PLANE;
	body->plane = (t_plane){
		.point.x = ft_atod(params[0]), .point.y = ft_atod(params[1]), \
		.point.z = ft_atod(params[2]), .normal.x = ft_atod(params[3]), \
		.normal.y = ft_atod(params[4]),	.normal.z = ft_atod(params[5])};
	body->color = parse_body_color(params + 6, &error);
	body->id = id_set(ID_GROUP_PLANE, body_count);
	if (error)
		return (err("plane", line_count), lst_memory(NULL, NULL, FAIL), false);
	body->plane.inverse_normal = scale_vector(body->plane.normal, -1);
	normalize_vector(&body->plane.inverse_normal);
	normalize_vector(&body->plane.normal);
	return (true);
}

void	plane_save(t_plane plane, uint color, int fd)
{
	ft_fprintf(fd, "pl %f,%f,%f %f,%f,%f", \
			plane.point.x, plane.point.y, plane.point.z, \
			plane.normal.x, plane.normal.y, plane.normal.z);
	color_print(color, fd);
}

double	plane_hit_distance(t_plane pl, t_vector cam, \
							t_vector camera_ray, int *flip)
{
	t_vector	body_delta;
	double		projection;

	projection = dot_product(pl.normal, camera_ray);
	body_delta = vector_subtract(pl.point, cam);
	if (projection > 0)
		*flip = true;
	if (projection != 0)
		return (dot_product(body_delta, pl.normal) / projection);
	return (-1);
}

void	get_color_texture_plane(double u, double v, t_texture *texture, t_pixel *pixel)
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

void	get_color_checker_plane(double u, double v, t_pixel *pixel)
{
    if (((int)floor(u) + (int)floor(v)) % 2 == 0)
        *pixel->color = 0x000000;
    else
        *pixel->color = 0xffffff;
}

void	get_color_plane(t_body *body, t_vector intersect, t_pixel *pixel)
{
	double		u;
	double 		v;
	t_vector	right;
	t_vector	up;

	if (!body->textured && !body->checker_board)
	{
		*pixel->color = body->color;
		return ;
	}
    intersect = vector_subtract(intersect, body->plane.point);
    if (fabs(body->plane.normal.y) > 0.9)
    {
        right = (t_vector){1, 0, 0};
        up = (t_vector){0, 0, 1};
    }
    else
    {
        right = cross_product(body->plane.normal, (t_vector){0, 1, 0});
        up = cross_product(right, body->plane.normal);
    }
	normalize_vector(&right);
    normalize_vector(&up);
    u = dot_product(intersect, right) * .5;
    v = dot_product(intersect, up) * .5;
	if (body->textured)
		get_color_texture_plane(u - floor(u), v - floor(v), body->texture, pixel);
	else if (body->checker_board)
		get_color_checker_plane(u, v, pixel);
}

void	pixel_plane_set(t_pixel *pixel, t_vector camera_ray, \
						t_body *body, t_scene *scene)
{
	t_vector	p;
	double		dist;
	t_plane		plane;
	double		attenuation;
	int			flip;

	plane = body->plane;
	flip = false;
	dist = plane_hit_distance(plane, scene->camera.position, \
			camera_ray, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		*pixel->color = body->color;
		pixel->id = body->id;
		p = add_vector(scene->camera.position, scale_vector(camera_ray, dist));
		if (flip)
			plane.normal = plane.inverse_normal;
		get_color_plane(body, p, pixel);
		attenuation = get_color_attenuation(p, plane.normal, \
				scene->light, scene);
		if (attenuation <= .01)
			pixel->id = 0;
		set_hit_pixel(scene, pixel, attenuation, dist);
	}
}
