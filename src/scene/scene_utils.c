/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:58:16 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 11:58:21 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	scene_body_add(t_body body, t_scene *scene)
{
	t_body	*body_store;

	if (scene->body_cursor == scene->current_body_max)
	{
		scene->current_body_max *= 2;
		body_store = scene->body;
		scene->body = ft_calloc(scene->current_body_max, sizeof(t_body));
		ft_memcpy(scene->body, body_store, sizeof(t_body) * scene->body_cursor);
		lst_memory(body_store, NULL, FREE);
		lst_memory(scene->body, free, ADD);
	}
	scene->body[scene->body_cursor++] = body;
}

t_body	*body_get_by_id(int id, t_scene *scene)
{
	uint	i;

	i = 0;
	if (id < 0)
		return (0);
	printf("id: %d\n", id);
	while (i < scene->body_cursor)
	{
		if (scene->body[i].id == id)
			return (&scene->body[i]);
		i++;
	}
	return (0);
}

void	scene_print(t_scene *scene)
{
	uint	i;

	i = 0;
	body_light_print(scene->light);
	body_light_print(scene->ambient);
	body_camera_print(scene->camera);
	while (scene->body[i].type != BODY_END && i < scene->current_body_max)
	{
		if (scene->body[i].type == BODY_SPHERE)
			body_sphere_print(&scene->body[i]);
		else if (scene->body[i].type == BODY_PLANE)
			body_plane_print(&scene->body[i]);
		else if (scene->body[i].type == BODY_CYLINDER)
			body_cylinder_print(&scene->body[i]);
		else if (scene->body[i].type == BODY_DISK)
			print_disk(scene->body + i);
		i++;
	}
}

bool	body_distribute(char *entry, char *tmp, \
						uint line_cursor, t_scene *scene)
{
	bool	success;
	t_body	body_new;

	ft_bzero(&body_new, sizeof(body_new));
	success = false;
	if (ft_strncmp(entry, "sp", 2) == 0)
		success = parse_sphere(tmp, line_cursor, \
				&body_new, scene->body_cursor);
	else if (ft_strncmp(entry, "pl", 2) == 0)
		success = parse_plane(tmp, line_cursor, \
				&body_new, scene->body_cursor);
	else if (ft_strncmp(entry, "cy", 2) == 0)
		success = parse_cylinder(tmp, line_cursor, \
				&body_new, scene->body_cursor);
	else if (ft_strncmp(entry, "di", 2) == 0)
		success = parse_disk(tmp, line_cursor, \
				&body_new, scene->body_cursor);
	else if (ft_strncmp(entry, "cn", 2) == 0)
		success = parse_cone(tmp, line_cursor, \
				&body_new, scene->body_cursor);
	body_new.texture = &scene->texture[0];
	scene_body_add(body_new, scene);
	return (success);
}

char	*buffer_read_chunk(t_line *line, uint fd, bool *file_end_reached)
{
	long	bytes_read;
	char	*tmp;

	if (!*file_end_reached)
	{
		bytes_read = ft_read(fd, line->buffer + line->length, \
				READ_BUFFER_SIZE - line->length);
		if (bytes_read < 0)
			lst_memory(NULL, NULL, FAIL);
		else if (bytes_read == 0)
			*file_end_reached = true;
	}
	tmp = ft_strchr(line->buffer, '\n');
	if (!tmp)
	{
		tmp = ft_strchr(line->buffer, '\0');
		if (!tmp)
			return (ft_printf("The length of line %d is too long for " \
							"the current implementation\nexiting...\n", \
							line->count), NULL);
	}
	*tmp = 0;
	line->count++;
	line->length = ft_strlen(line->buffer);
	return (tmp);
}
