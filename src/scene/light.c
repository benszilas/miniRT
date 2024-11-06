/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:55:30 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/05 11:44:19 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	light_save(t_light light, int fd)
{
	ft_fprintf(fd, "L %f,%f,%f %f", \
			light.position.x, light.position.y, light.position.z, \
			light.intensity);
	color_print(light.color, fd);
}

void	body_light_print(t_light light)
{
	printf("light:\nx: %f\ny: %f\nz: %f\nintensity: %f\ncolor: %X\n\n", \
			light.position.x, light.position.y, light.position.z, \
			light.intensity, light.color);
}

bool	parse_light(char *entry_light, uint line_count, t_light *light)
{
	char	*params[7];
	int		error;

	ft_bzero(params, sizeof(params));
	error = false;
	while (ft_isspace(*entry_light))
		entry_light++;
	if (!collect_param_comma(&entry_light, params, 2) \
		|| !collect_param_single(&entry_light, &params[3]) \
		|| !collect_param_comma(&entry_light, &params[4], 2))
		return (err("light", line_count), lst_memory(NULL, NULL, FAIL), false);
	*light = (t_light){.position.x = ft_atod(params[0]), \
		.position.y = ft_atod(params[1]), .position.z = ft_atod(params[2]), \
		.intensity = ft_atod(params[3]), \
		.color = parse_body_color(params + 4, &error)};
	if (error || light->intensity < 0 || light->intensity > 1.0)
		return (err("light", line_count), lst_memory(NULL, NULL, FAIL), false);
	return (true);
}

bool	parse_ambient(char *entry_light, uint line_count, t_light *light)
{
	char	*params[4];
	int		error;
	bool	was_parsed;

	ft_bzero(params, sizeof(params));
	error = false;
	was_parsed = false;
	while (ft_isspace(*entry_light))
		entry_light++;
	if (!collect_param_single(&entry_light, params) \
		|| !collect_param_comma(&entry_light, params + 1, 2) \
		|| was_parsed == true)
		return (err("light", line_count), lst_memory(NULL, NULL, FAIL), false);
	*light = (t_light){.position = set_vector(0, 0, 0), \
		.intensity = ft_atod(params[0]),
		.color = get_color(parse_body_color(params + 1, &error), \
		0xFFFFFFFF, ft_atod(params[0]))};
	if (error || light->intensity < 0 || light->intensity > 1.0)
		return (err("light", line_count), lst_memory(NULL, NULL, FAIL), false);
	return (was_parsed = true);
}
