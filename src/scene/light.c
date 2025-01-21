/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:55:30 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 16:44:33 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	ambient_save(t_light light, int fd)
{
	ft_fprintf(fd, "A %f", light.intensity);
	color_print(color_brightness(light.color, 1 / light.intensity), fd);
}

void	body_light_print(t_light light)
{
	ft_printf("light:\nx: %f\ny: %f\nz: %f\nintensity: %f\ncolor: %X\n\n", \
			light.position.x, light.position.y, light.position.z, \
			light.intensity, light.color);
}

void	save_all_lights(t_scene *scene, int fd)
{
	uint	i;

	i = 0;
	while (i < scene->light_count)
	{
		ft_fprintf(fd, "L %f,%f,%f %f", \
				scene->light[i].position.x, scene->light[i].position.y, \
				scene->light[i].position.z, scene->light[i].intensity);
		color_print(scene->light[i].color, fd);
		i++;
	}
	ambient_save(scene->ambient, fd);
}

void	micro_displace_vector(t_vector *position)
{
	position->x += 0.001f * (rand() & 0xf);
	position->y += 0.001f * (rand() & 0xf);
	position->z += 0.001f * (rand() & 0xf);
}

bool	parse_light(char *entry_light, uint line_count, t_light *light, \
		int count)
{
	char	*params[7];
	int		error;

	if (count > MAX_LIGHTS)
		return (err("light", line_count), false);
	ft_bzero(params, sizeof(params));
	error = false;
	while (ft_isspace(*entry_light))
		entry_light++;
	if (!collect_param_comma(&entry_light, params, 2, COLLECT_MODE_NORMAL) \
		|| !collect_param_single(&entry_light, &params[3], COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry_light, &params[4], 2, COLLECT_MODE_END))
		return (err("light", line_count), false);
	*light = (t_light){.position.x = ft_atod(params[0]), \
		.position.y = ft_atod(params[1]), .position.z = ft_atod(params[2]), \
		.intensity = ft_atod(params[3]), \
		.color = parse_body_color(params + 4, &error)};
	if (error || light->intensity < 0 || light->intensity > 1.0)
		return (err("light", line_count), false);
	micro_displace_vector(&light->position);	//make light-object intersections unlikely
	return (true);
}

bool	parse_ambient(char *entry_light, uint line_count, \
						t_light *light, bool *was_parsed)
{
	char		*params[4];
	int			error;

	error = false;
	while (ft_isspace(*entry_light))
		entry_light++;
	if (!collect_param_single(&entry_light, params, COLLECT_MODE_NORMAL) \
		|| !collect_param_comma(&entry_light, params + 1, 2, COLLECT_MODE_END) \
		|| *was_parsed == true)
		return (err("ambient", line_count), false);
	*light = (t_light){.position = set_vector(0, 0, 0), \
		.intensity = ft_atod(params[0]),
		.color = get_color(parse_body_color(params + 1, &error), \
		0xFFFFFFFF, ft_atod(params[0]))};
	if (error || light->intensity < 0 || light->intensity > 1.0)
		return (err("ambient", line_count), false);
	return (*was_parsed = true);
}
