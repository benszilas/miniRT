/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:13:10 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/06 22:02:46 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	move_cylinder(int keycode, t_cylinder *cyl)
{
	if (keycode == XK_Up || keycode == XK_Down)
		cyl->normal = rot_x(cyl->normal, keycode - XK_Up - 1);
	else if (keycode == XK_Left || keycode == XK_Right)
		cyl->normal = rot_y(cyl->normal, keycode - XK_Left - 1);
	else if (keycode == XK_1 || keycode == XK_3)
		cyl->normal = rot_z(cyl->normal, keycode - XK_1 - 1);
	else if (!key_move_focused(keycode, &cyl->center))
		return (false);
	calc_cyl_data(cyl);
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

void	cylinder_save(t_cylinder cylinder, uint color, int fd)
{
	ft_fprintf(fd, "cy %f,%f,%f %f,%f,%f %f %f", \
			cylinder.center.x, cylinder.center.y, cylinder.center.z, \
			cylinder.normal.x, cylinder.normal.y, cylinder.normal.z, \
			cylinder.radius, cylinder.height);
	color_print(color, fd);
}

void	calc_cyl_data(t_cylinder *cy)
{
	t_vector	axle;

	normalize_vector(&cy->normal);
	axle = scale_vector(cy->normal, cy->height / 2);
	cy->bottom = vector_subtract(cy->center, axle);
	cy->top = add_vector(cy->center, axle);
}

void	cyl_equation_coefficients(t_cylinder *cy, \
									t_vector ray, \
									t_vector cam_delta)
{
	cy->coeffs.x = ray_distance_from_point_squared(cy->normal, ray);
	cy->coeffs.y = 2 * (dot_product(cam_delta, ray) - \
	dot_product(cam_delta, cy->normal) * dot_product(ray, cy->normal));
	cy->coeffs.z = ray_distance_from_point_squared(cy->normal, cam_delta) - \
	pow(cy->radius, 2);
}

bool	parse_cylinder(char *entry, uint line_c, t_body *body, uint body_cursor)
{
	char	*params[11];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	if (!collect_param_comma(&entry, params, 2) \
		|| !collect_param_comma(&entry, &params[3], 2) \
		|| !collect_param_single(&entry, &params[6]) \
		|| !collect_param_single(&entry, &params[7]) \
		|| !collect_param_comma(&entry, &params[8], 2))
		return (err("cylinder", line_c), lst_memory(NULL, NULL, FAIL), false);
	body->type = BODY_CYLINDER;
	body->cylinder = (t_cylinder){\
		.center = set_vector(ft_atod(params[0]), ft_atod(params[1]), \
		ft_atod(params[2])), .normal = set_vector(ft_atod(params[3]), \
		ft_atod(params[4]), ft_atod(params[5])), \
		.radius = ft_atod(params[6]) / 2, .height = ft_atod(params[7])};
	body->color = parse_body_color(params + 8, &error);
	body->id = id_set(ID_GROUP_CYLINDER, body_cursor);
	if (error || body->cylinder.radius <= 0 || body->cylinder.height <= 0)
		return (err("cone", line_c), lst_memory(NULL, NULL, FAIL), false);
	calc_cyl_data(&body->cylinder);
	return (true);
}

double	solve_cyl_equation(t_cylinder *cy, t_vector ray, \
							t_vector cam_delta, int *flip)
{
	double	discriminant;
	double	a2;
	double	t1;
	double	t2;

	cyl_equation_coefficients(cy, ray, cam_delta);
	discriminant = pow(cy->coeffs.y, 2) - 4 * cy->coeffs.x * cy->coeffs.z;
	if (discriminant < 0)
		return (-1);
	discriminant = sqrt(discriminant);
	a2 = 1 / (2 * cy->coeffs.x);
	t2 = (-(cy->coeffs.y) - discriminant) * a2;
	t1 = (-(cy->coeffs.y) + discriminant) * a2;
	if (t1 > 0 && t2 < 0)
		*flip = -1;
	return (smaller_non_negative(t1, t2));
}

double	cyl_hit_distance(t_cylinder *cy, t_vector ray, t_vector cam, int *flip)
{
	double		t;
	t_vector	p;

	t = solve_cyl_equation(cy, ray, vector_subtract(cam, cy->bottom), flip);
	if (t > 0)
	{
		p = add_vector(cam, scale_vector(ray, t));
		cy->hit_h = dot_product(vector_subtract(p, cy->bottom), cy->normal);
		if (cy->hit_h <= cy->height && cy->hit_h >= 0)
			return (t);
	}
	return (-1);
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

void	get_color_checker_cylinder(double u, double v, t_pixel *pixel)
{
	int checker_u;
	int checker_v;

	checker_u = (int)(u * 16);
	checker_v = (int)(v * 8);
	if ((checker_u + checker_v) % 2 == 0)
		*pixel->color = 0x000000;
	else
		*pixel->color = 0xffffff;
}

void	get_color_texture_cylinder(double u, double v, t_texture *texture, t_pixel *pixel);

t_vector	get_local(t_cylinder *cylinder, t_vector intersect)
{
    t_vector u_axis;
	t_vector v_axis;
    t_vector delta;
	t_vector local;

    u_axis = (fabs(cylinder->normal.x) > 0.0001 || fabs(cylinder->normal.z) > 0.0001) ? (t_vector){0, 1, 0} : (t_vector){1, 0, 0};
	u_axis = cross_product(u_axis, cylinder->normal);
	normalize_vector(&u_axis);
    v_axis = cross_product(cylinder->normal, u_axis);

    // Transform intersection point to local coordinates
    delta = vector_subtract(intersect, cylinder->center);
    local.x = dot_product(delta, u_axis);
    local.y = dot_product(delta, cylinder->normal);
    local.z = dot_product(delta, v_axis);
	return (local);
}

void get_color_cylinder(t_body *body, t_vector intersect, t_pixel *pixel)
{
    double	u;
	double	v;
	t_vector	local;
	double theta;

	if (body->reflect || (!body->textured && !body->checker_board))
    {
        *pixel->color = body->color;
        return;
    }
	local = get_local(&body->cylinder, intersect);
    theta = atan2(local.z, local.x);
    u = (theta + M_PI) / (2 * M_PI);
    v = (local.y - (body->cylinder.height / 2)) / body->cylinder.height;

    v = fmod(v, 1.0);
    if (v < 0)
        v += 1.0;
    if (body->checker_board)
        get_color_checker_cylinder(u, v, pixel);
    /*else if (body->textured)*/
    /*    get_color_texture_cylinder(u, v, body->texture, pixel);*/
}

void	trace_cyl_caps(t_pixel *px, t_vector ray, t_body *cyl, t_scene *sc)
{
	t_body	cap;

	cap.id = cyl->id;
	cap.color = cyl->color;
	cap.checker_board = cyl->checker_board;
	cap.textured = cyl->textured;
	cap.texture = cyl->texture;
	cap.reflect = cyl->reflect;
	cap.disk = (t_disk){.point = cyl->cylinder.top, \
		.normal = cyl->cylinder.normal, .radius = cyl->cylinder.radius, \
			.inverse_normal = scale_vector(cyl->cylinder.normal, -1)};
	trace_disk(px, ray, &cap, sc);
	cap.disk.point = cyl->cylinder.bottom;
	trace_disk(px, ray, &cap, sc);
}

t_vector	cyl_normal(t_cylinder cy, t_vector p, int flip)
{
	return (scale_vector(get_normal(p, add_vector(cy.bottom, \
						scale_vector(cy.normal, cy.hit_h))), flip));
}

void	trace_cyl(t_pixel *pixel, t_vector ray, t_body *body, t_scene *sc)
{
	t_hit_point	hit;
	double		dist;
	t_cylinder	cy;
	int			flip;

	cy = body->cylinder;
	calc_cyl_data(&cy);
	trace_cyl_caps(pixel, ray, body, sc);
	flip = 1;
	dist = cyl_hit_distance(&cy, ray, sc->camera.position, &flip);
	if (dist > SHADOW_BIAS && (dist < pixel->dist || pixel->dist < 0))
	{
		pixel->id = body->id;
		pixel->dist = dist;
		hit.p = add_vector(sc->camera.position, scale_vector(ray, dist));
		calc_hit_point_vectors(&hit, ray, cyl_normal(cy, hit.p, flip));
		get_color_cylinder(body, hit.p, pixel);
		if (body->reflect && sc->depth < MAX_DEPTH)
			trace_reflection(pixel, hit, *sc);
		else
			trace_lights(sc, pixel, hit);
	}
}
