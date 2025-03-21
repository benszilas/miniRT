/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 17:52:32 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/11 16:48:08 by bszilas          ###   ########.fr       */
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

void	sphere_save(t_sphere sphere, t_color color, int fd)
{
	ft_fprintf(fd, "sp %f,%f,%f %f", \
			sphere.center.x, sphere.center.y, sphere.center.z, \
			sphere.radius * 2);
	color_print(color, fd);
}

bool	parse_sphere(char *entry, uint line_count, \
						t_body *body, uint body_count)
{
	char	*params[7];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	if (!collect_param_comma(&entry, params, 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_single(&entry, &params[3], COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry, &params[4], 2, COLLECT_MODE_END))
		return (err("sphere", line_count), false);
	body->type = BODY_SPHERE;
	body->sphere = (t_sphere){.center.x = ft_atod(params[0]), \
		.center.y = ft_atod(params[1]), .center.z = ft_atod(params[2]), \
		.radius = ft_atod(params[3]) / 2};
	body->color = set_color(set_signed_int(params[4], 0, 255, &error), \
			set_signed_int(params[5], 0, 255, &error), \
			set_signed_int(params[6], 0, 255, &error));
	if (error || body->sphere.radius <= 0)
		return (err("sphere", line_count), false);
	body->id = id_set(ID_GROUP_SPHERE, body_count);
	return (true);
}

double	sphere_hit_distance(t_vector ray, t_vector dlt_centr, \
							t_sphere sphere, int *invert)
{
	double	projection;
	double	in_section;
	double	ray_dist;
	double	t0;
	double	t1;

	ray_dist = dot_product(dlt_centr, ray);
	ray_dist = dot_product(dlt_centr, dlt_centr) - ray_dist * ray_dist;
	if (ray_dist > sphere.radius * sphere.radius)
		return (-1);
	else if (ray_dist < 0)
		ray_dist = 0;
	ray_dist = sqrt(ray_dist);
	projection = dot_product(dlt_centr, ray);
	if (ray_dist == sphere.radius)
		return (smaller_non_negative(-1, projection));
	in_section = sqrt(pow(sphere.radius, 2) - pow(ray_dist, 2));
	t1 = projection + in_section;
	t0 = projection - in_section;
	t0 -= SHADOW_BIAS;
	if (t0 < 0 && t1 > 0)
		*invert = -1;
	return (smaller_non_negative(t0, t1));
}

void	pixel_sphere_set(t_pixel *pixel, t_vector ray, \
							t_body *body, t_scene *scene)
{
	double		dist;
	t_hit_point	hit;
	int			invert;

	dist = -1;
	invert = 1;
	dist = sphere_hit_distance(ray, vector_subtract(body->sphere.center, \
								scene->camera.position), body->sphere, &invert);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		set_info_to_pixel(pixel, body, dist);
		hit.p = add_vector(scene->camera.position, scale_vector(ray, dist));
		calc_hit_point_vectors(&hit, ray, \
			scale_vector(get_normal(hit.p, body->sphere.center), invert));
		apply_shadow_bias(&hit.p, hit.n, pow(dist, 1.3) \
							* 1 / body->sphere.radius);
		get_color_sphere(body, &hit, pixel);
		if (body->reflect && scene->depth < MAX_DEPTH && invert == 1)
			trace_reflection(pixel, hit, *scene);
		else
			trace_lights(scene, pixel, hit);
	}
}
