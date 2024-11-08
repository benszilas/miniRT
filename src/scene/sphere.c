/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 17:52:32 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/06 21:45:14 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	body_sphere_print(t_body *body)
{
	t_sphere	sphere;

	sphere = body->sphere;
	ft_printf("sphere:\nx: %f\ny: %f\nz: %f\nrad: %f\ncolor: %X\nid: %d\n\n", \
			sphere.center.x, sphere.center.y, sphere.center.z, \
			sphere.radius, body->color, body->id);
}

void	sphere_save(t_sphere sphere, uint color, int fd)
{
	ft_fprintf(fd, "sp %f,%f,%f %f", \
			sphere.center.x, sphere.center.y, sphere.center.z, \
			sphere.radius);
	color_print(color, fd);
}

bool	parse_sphere(char *entry, uint line_count, \
						t_body *body, uint body_count)
{
	char	*params[7];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	if (!collect_param_comma(&entry, params, 2) \
		|| !collect_param_single(&entry, &params[3]) \
		|| !collect_param_comma(&entry, &params[4], 2))
		return (err("sphere", line_count), lst_memory(NULL, NULL, FAIL), 0);
	body->type = BODY_SPHERE;
	body->sphere = (t_sphere){.center.x = ft_atod(params[0]), \
		.center.y = ft_atod(params[1]), .center.z = ft_atod(params[2]), \
		.radius = ft_atod(params[3]) / 2};
	body->color = set_color(set_signed_int(params[4], 0, 255, &error), \
			set_signed_int(params[5], 0, 255, &error), \
			set_signed_int(params[6], 0, 255, &error));
	if (error || body->sphere.radius <= 0)
		return (err("sphere", line_count), lst_memory(NULL, NULL, FAIL), 0);
	body->id = id_set(ID_GROUP_SPHERE, body_count);
	return (true);
}

double	sphere_hit_distance(t_vector ray, t_vector dlt_centr, \
							t_sphere sphere, int *flip)
{
	double		projection;
	double		in_section;
	double		ray_dist;
	double		t0;
	double		t1;

	ray_dist = dot_product(dlt_centr, ray);
	ray_dist = (dot_product(dlt_centr, dlt_centr) + SHADOW_BIAS) \
		- ray_dist * ray_dist;
	if (ray_dist > sphere.radius * sphere.radius)
		return (-1);
	ray_dist = sqrt(ray_dist);
	projection = dot_product(dlt_centr, ray);
	if (ray_dist == sphere.radius)
		return (smaller_non_negative(-1, projection));
	in_section = sqrt(pow(sphere.radius, 2) - pow(ray_dist, 2));
	t1 = projection + in_section;
	t0 = projection - in_section;
	if (t0 < 0 && t1 > 0)
		*flip = -1;
	return (smaller_non_negative(t0, t1));
}

void	get_color_checker_sphere(double u, double v, t_pixel *pixel)
{

	int checker_u;
	int checker_v;

	checker_u = round(u * CHECKER_GRID_SIZE);
	checker_v = round(v * CHECKER_GRID_SIZE);
	if ((checker_u + checker_v) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

void	get_color_texture_sphere(double u, double v, \
								t_texture *texture, t_pixel *pixel)
{
	int tex_u;
	int tex_v;

	tex_u = (int)(u * (texture->width - 1)) % texture->width;
	tex_v= (int)(v * (texture->height - 1)) % texture->height;
	if (tex_u < 0)
		tex_u += texture->width;
	if (tex_v < 0)
		tex_v += texture->height;
	*pixel->color = texture->pixel[tex_v * texture->width + tex_u]; 
}

void	get_color_sphere(	t_body *body, \
							t_vector intersect, \
							t_pixel *pixel)
{
    double theta;
    double phi;
    double u;
    double v;

	if (body->reflect || (!body->textured && !body->checker_board))
	{
		*pixel->color = body->color;
		return ;
	}
    intersect = vector_subtract(intersect, body->sphere.center);
	theta = atan2(intersect.x, intersect.z); 
	phi = acos(intersect.y / body->sphere.radius);
	v = phi / M_PI;
	u = (theta + M_PI) / (2 * M_PI);;
	u = 1 - (u + 0.5);
	if (body->checker_board)
		get_color_checker_sphere(u, v, pixel);
	else if (body->texture && body->textured == true)
		get_color_texture_sphere(u, v, body->texture, pixel);
}

void	pixel_sphere_set(t_pixel *pixel, t_vector ray, \
							t_body *body, t_scene *scene)
{
	double		dist;
	t_hit_point	hit;
	int			flip;

	dist = -1;
	flip = 1;
	dist = sphere_hit_distance(ray, vector_subtract(body->sphere.center, \
								scene->camera.position), body->sphere, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		pixel->dist = dist;
		pixel->id = body->id;
		hit.p = add_vector(scene->camera.position, scale_vector(ray, dist));
		calc_hit_point_vectors(&hit, ray, \
		scale_vector(get_normal(hit.p, body->sphere.center), flip));
		apply_shadow_bias(&hit.p, hit.n, 20 * (2 / body->sphere.radius));
		get_color_sphere(body, hit.p, pixel);
		if (body->reflect && scene->depth < MAX_DEPTH)
			trace_reflection(pixel, hit, *scene);
		else
			trace_lights(scene, pixel, hit);
	}
}
