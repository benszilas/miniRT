/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:50:30 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/05 11:58:06 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	scene_save_helper(t_scene *scene, char *path)
{
	uint	i;
	int		fd;

	i = 0;
	ft_open(&fd, path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	light_save(scene->light, fd);
	camera_save(&scene->camera, fd);
	while (i < scene->body_cursor)
	{
		if (scene->body[i].type == BODY_SPHERE)
			sphere_save(scene->body[i].sphere, scene->body[i].color, fd);
		else if (scene->body[i].type == BODY_PLANE)
			plane_save(scene->body[i].plane, scene->body[i].color, fd);
		else if (scene->body[i].type == BODY_CYLINDER)
			cylinder_save(scene->body[i].cylinder, scene->body[i].color, fd);
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
	while (i < PATH_LENGTH - 1)
	{
		ft_read(1, &c, 1);
		if (c == '\n')
			break ;
		path[i] = c;
		i++;
	}
	if (i == 0)
		ft_strlcpy(path, "untitled.rt", sizeof(path));
	scene_save_helper(scene, path);
}

bool	body_determine(char *entry, uint line_cursor, t_scene *scene)
{
	char			*tmp;
	bool			success;

	while (*entry && ft_isspace((*entry)))
		entry++;
	tmp = entry;
	while (*tmp && !ft_isspace(*tmp))
		tmp++;
	if (!*tmp)
		return (ft_fprintf(STDERR_FILENO, "Empty or Invalid config\n"), false);
	*tmp++ = 0;
	success = false;
	if (ft_strncmp(entry, "L", 2) == 0)
		success = parse_light(tmp, line_cursor, &scene->light);
	else if (ft_strncmp(entry, "C", 2) == 0)
		success = parse_camera(tmp, line_cursor, &scene->camera);
	else if (ft_strncmp(entry, "A", 2) == 0)
		success = parse_ambient(tmp, line_cursor, &scene->ambient);
	else
		success = body_distribute(entry, tmp, line_cursor, scene);
	return (success);
}

void	scene_create_loop(t_scene *scene, t_line line, int fd)
{
	bool	file_end_reached;

	file_end_reached = false;
	while (!(file_end_reached == true && line.length == 0))
	{
		buffer_read_chunk(&line, fd, &file_end_reached);
		if (!body_determine(line.buffer, line.count, scene))
			lst_memory(NULL, NULL, FAIL);
		ft_memmove(line.buffer, line.buffer + line.length + 1, \
				READ_BUFFER_SIZE - line.length);
		line.length = ft_strlen(line.buffer);
		ft_bzero(line.buffer + line.length, READ_BUFFER_SIZE - line.length - \
				(line.length < READ_BUFFER_SIZE));
	}
}

void	scene_create(const char *filepath, t_scene *scene)
{
	int		fd;
	t_line	line;

	ft_bzero(scene, sizeof(*scene));
	ft_bzero(&line, sizeof(line));
	scene->current_body_max = MAX_BODY_INIT;
	scene->texture = ft_calloc(16, sizeof(t_texture));
	lst_memory(scene->texture, free, ADD);
	scene->body = ft_calloc(MAX_BODY_INIT, sizeof(*scene->body));
	lst_memory(scene->body, free, ADD);
	scene->texture[0] = ppm_image_read("./assets/earth.ppm");
	ft_open(&fd, filepath, O_CREAT | O_RDWR, 0644);
	scene_create_loop(scene, line, fd);
	ft_close(fd);
	scene->resolution_x = SCENE_START_RESOLUTION_X;
	scene->resolution_y = SCENE_START_RESOLUTION_Y;
}
