/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explorer_load_file.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:47:52 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 14:39:45 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

static bool	is_format(char *file, char *format)
{
	char	*tmp;

	tmp = ft_strrchr(file, '.');
	if (!tmp)
		return (false);
	if (ft_strncmp(tmp, format, ft_strlen(format)) == 0)
		return (true);
	else
	{
		ft_fprintf(STDERR_FILENO, \
			"[Explorer] Invalid file format, expected: %s\n", format);
		return (false);
	}
}

static void	bump_map_load(t_item *item, t_data *data)
{
	uint	i;

	i = 0;
	while (i < data->scene.bump_map_count)
	{
		if (ft_strncmp(data->scene.bump_map[i].path, \
					item->dirent.full_path, 255) == 0)
			return (*(t_bump_map **)item->param.param \
					= &data->scene.bump_map[i], (void)0);
		i++;
	}
	ft_printf("[Explorer] Loading file: %s\n", item->dirent.full_path);
	bump_map_read(item->dirent.full_path, \
		&data->scene.bump_map[data->scene.bump_map_count]);
	*(t_bump_map **)item->param.param = \
				&data->scene.bump_map[data->scene.bump_map_count];
	data->scene.bump_map_count = (data->scene.bump_map_count + 1);
	if (data->scene.bump_map_count == MAPS_MAX)
		data->scene.bump_map_count = 0;
	ft_printf("[Explorer] File Loaded!\n");
}

static void	texture_load(t_item *item, t_data *data)
{
	uint	i;

	i = 0;
	while (i < data->scene.texture_count)
	{
		if (ft_strncmp(data->scene.texture[i].path, \
					item->dirent.full_path, 255) == 0)
			return (*(t_texture **)item->param.param \
					= &data->scene.texture[i], (void)0);
		i++;
	}
	ft_printf("[Explorer] Loading file: %s\n", item->dirent.full_path);
	ppm_image_read(item->dirent.full_path, \
			&data->scene.texture[data->scene.texture_count]);
	*(t_texture **)item->param.param = \
				&data->scene.texture[data->scene.texture_count];
	if (data->scene.texture_count == MAPS_MAX)
		data->scene.texture_count = 0;
	ft_printf("[Explorer] File Loaded!\n");
}

void	file_load(t_item *item, t_data *data)
{
	if (item->param.type == PARAM_TEXTURE \
			&& is_format(item->dirent.full_path, ".ppm"))
		texture_load(item, data);
	else if (item->param.type == PARAM_BUMP \
			&& is_format(item->dirent.full_path, ".bump"))
		bump_map_load(item, data);
	else if (item->param.type == PARAM_SCENE \
			&& is_format(item->dirent.full_path, ".rt"))
	{
		ft_printf("[Explorer] Loading file: %s\n", item->dirent.full_path);
		scene_replace(item->param.param, item->dirent.full_path);
		ft_printf("[Explorer] File Loaded!\n");
	}
}
