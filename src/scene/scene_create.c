/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:34:29 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 16:40:04 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	scene_save_helper(t_scene *scene, char *path)
{
	uint	i;
	int		fd;

	ft_open(&fd, path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	camera_save(&scene->camera, fd);
	save_all_lights(scene, fd);
	i = 0;
	while (i < scene->body_cursor)
	{
		if (scene->body[i].type == BODY_SPHERE)
			sphere_save(scene->body[i].sphere, scene->body[i].color, fd);
		else if (scene->body[i].type == BODY_PLANE)
			plane_save(scene->body[i].plane, scene->body[i].color, fd);
		else if (scene->body[i].type == BODY_CYLINDER)
			cylinder_save(scene->body[i].cylinder, scene->body[i].color, fd);
		else if (scene->body[i].type == BODY_DISK)
			disk_save(scene->body[i].disk, scene->body[i].color, fd);
		else if (scene->body[i].type == BODY_CONE)
			cone_save(scene->body[i].cone, scene->body[i].color, fd);
		i++;
	}
	ft_printf("File %s succesfully saved\n", path);
	ft_close(fd);
}

void	scene_save(t_scene *scene)
{
	char	path[PATH_LENGTH];
	char	c;
	uint	i;

	c = 0;
	i = 0;
	ft_bzero(path, sizeof(path));
	ft_putstr_fd("\n\nPlease Enter filename to save to: ", 1);
	while (i < sizeof(path) - 4) //4 for the ".rt" file extension
	{
		ft_read(1, &c, 1);
		if (c == '\n')
			break ;
		path[i] = c;
		i++;
	}
	if (i == 0)
		ft_strlcpy(path, "untitled.rt", sizeof(path));
	else
		ft_strlcat(path, ".rt", sizeof(path));
	scene_save_helper(scene, path);
}

bool	body_determine(char *entry, uint line_cursor, t_scene *scene)
{
	char	*tmp;
	bool	success;

	while (*entry && ft_isspace((*entry)))
		entry++;
	tmp = entry;
	while (*tmp && !ft_isspace(*tmp))
		tmp++;
	if (!*tmp)
		return (ft_fprintf(STDERR_FILENO, "Empty or Invalid config\n"), false);
	*tmp++ = 0;
	success = false;
	if (!check_line(tmp))
		return (err("line content", line_cursor), false);
	if (ft_strncmp(entry, "L", 2) == 0)
		return (success = parse_light(tmp, line_cursor, \
					scene->light + scene->light_count++, scene->light_count));
	else if (ft_strncmp(entry, "C", 2) == 0)
		return (success = parse_camera(tmp, line_cursor, &scene->camera, \
					&scene->ambient_was_parsed));
	else if (ft_strncmp(entry, "A", 2) == 0)
		return (success = parse_ambient(tmp, line_cursor, &scene->ambient, \
					&scene->camera_was_parsed));
	else
		return (success = body_distribute(entry, tmp, line_cursor, scene));
}

void	scene_create_loop(t_scene *scene, t_line line, int fd)
{
	bool	file_end_reached;

	file_end_reached = false;
	while (!(file_end_reached == true && line.length == 0))
	{
		if (!buffer_read_chunk(&line, fd, &file_end_reached) \
			|| !body_determine(line.buffer, line.count, scene))
		{
			close(fd);
			lst_memory(NULL, NULL, FAIL);
		}
		ft_memmove(line.buffer, line.buffer + line.length + 1, \
				READ_BUFFER_SIZE - line.length);
		line.length = ft_strlen(line.buffer);
		ft_bzero(line.buffer + line.length, READ_BUFFER_SIZE - line.length - \
				(line.length < READ_BUFFER_SIZE));
	}
	if (scene->camera_was_parsed == false || scene->ambient_was_parsed == false)
	{
		ft_fprintf(STDERR_FILENO, "[MiniRT] Missing Ambient " \
				"light or Camera!\n");
		lst_memory(NULL, NULL, FAIL);
	}
}

void	scene_create(const char *filepath, t_scene *scene)
{
	int		fd;
	t_line	line;

	ft_bzero(scene, sizeof(*scene));
	ft_bzero(&line, sizeof(line));
	scene->texture = ft_calloc(MAPS_MAX, sizeof(t_texture));
	lst_memory(scene->texture, free, ADD);
	scene->bump_map = ft_calloc(MAPS_MAX, sizeof(t_bump_map));
	lst_memory(scene->bump_map, free, ADD);
	ppm_image_read(TEXTURE_PATH, &scene->texture[0]);
	scene->texture_count = 1;
	ppm_image_read(SKYBOX_PATH, &scene->sky);
	scene->sky_color = set_color(SKY_COLOR >> 16 & 0xff, SKY_COLOR >> 8 & 0xff, SKY_COLOR & 0xff);
	bump_map_read(BUMP_MAP_PATH, &scene->bump_map[0]);
	scene->bump_map_count = 1;
	scene->body = ft_calloc(MAX_BODY_INIT + 1, sizeof(*scene->body));
	lst_memory(scene->body, free, ADD);
	scene->current_body_max = MAX_BODY_INIT;
	scene->body[scene->current_body_max].type = BODY_END;
	ft_open(&fd, filepath, O_RDONLY, 0644);
	scene_create_loop(scene, line, fd);
	ft_close(fd);
	scene->resolution_x = SCENE_START_RESOLUTION_X;
	scene->resolution_y = SCENE_START_RESOLUTION_Y;
	srand(time(NULL));
}
