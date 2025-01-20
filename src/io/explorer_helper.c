/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explorer_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:51:57 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/11 12:47:28 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	ft_opendir(DIR **dir, char *path)
{
	*dir = opendir(path);
	if (!*dir)
	{
		ft_fprintf(STDERR_FILENO, "Failed to open directory: %s\n", path);
		return (false);
	}
	return (true);
}

void	scene_replace(t_scene *scene, char *new_path)
{
	int		fd;
	t_line	line;

	ft_bzero(&line, sizeof(line));
	ft_open(&fd, new_path, O_CREAT | O_RDWR, 0644);
	scene->body_cursor = 0;
	scene->light_count = 0;
	scene->reload = true;
	scene->camera_was_parsed = false;
	scene->ambient_was_parsed = false;
	ft_bzero(scene->light, sizeof(*scene->light) * scene->light_count);
	ft_bzero(&scene->ambient, sizeof(*scene->light));
	ft_bzero(scene->body, sizeof(t_body) * scene->current_body_max);
	scene->body_focus = NULL;
	scene_create_loop(scene, line, fd);
	ft_close(fd);
}

void	container_item_swap(t_item *itema, t_item *itemb)
{
	t_item	tmp;

	ft_strlcpy(tmp.description, itema->description, 1024);
	tmp.dirent = itema->dirent;
	tmp.param = itema->param;
	ft_strlcpy(itema->description, itemb->description, 1024);
	itema->dirent = itemb->dirent;
	itema->param = itemb->param;
	ft_strlcpy(itemb->description, tmp.description, 1024);
	itemb->dirent = tmp.dirent;
	itemb->param = tmp.param;
}

void	container_item_desc_sort(t_container *cont)
{
	uint	j;
	bool	swapped;

	while (1)
	{
		swapped = false;
		j = 0;
		while (j + 1 < cont->item_count)
		{
			if (ft_strncmp(cont->item[j].description, \
						cont->item[j + 1].description, \
						ft_strlen(cont->item[j].description)) > 0)
			{
				container_item_swap(&cont->item[j], &cont->item[j + 1]);
				swapped = true;
			}
			j++;
		}
		if (!swapped)
			break ;
	}
}
