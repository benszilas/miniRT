/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explorer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 11:27:50 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 14:56:19 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	container_item_dirent_create(t_item *item, char *title, \
									int type, void (*func)(void *, void *))
{
	item->type = ITEM_DIRENT;
	if (title && ft_strlen(title) < CONTAINER_TITLE_LEN)
		ft_strlcpy(item->description, title, ft_strlen(title) + 1);
	item->dirent.type = type;
	item->dirent.func = func;
}

void	explorer_create(t_container *explorer, t_data *data)
{
	ft_bzero(explorer, sizeof(*explorer));
	*explorer = container_create("Explorer", NULL, CONTAINER_LIST);
	explorer->data = data;
}

void	explorer_read_dir_loop(DIR *dir, void *param, \
								char *cwd, t_container *explorer)
{
	struct dirent	*entry;
	t_item			item;

	ft_strlcpy(explorer->title, cwd, CONTAINER_TITLE_LEN);
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		container_item_dirent_create(&item, entry->d_name, \
				entry->d_type, explorer_entry_func);
		ft_strlcpy(item.dirent.full_path, cwd, 1024);
		ft_strlcat(item.dirent.full_path, "/", 1024);
		ft_strlcat(item.dirent.full_path, entry->d_name, 1024);
		item.param = *(t_param *)param;
		container_item_add(explorer, &item);
	}
}

void	explorer_read_dir(void *data, void *param)
{
	DIR					*dir;
	static t_container	explorer = {0};
	char				cwd[1024];

	if (explorer.title[0] == 0)
		explorer_create(&explorer, data);
	if (!getcwd(cwd, 1024))
		return ;
	if (!ft_opendir(&dir, cwd))
		return ;
	ft_bzero(&explorer.item, sizeof(explorer.item));
	explorer.item_count = 0;
	explorer_read_dir_loop(dir, param, cwd, &explorer);
	closedir(dir);
	container_item_desc_sort(&explorer);
	((t_data *)data)->func_ptr = container_draw;
	((t_data *)data)->param = &explorer;
	rendering_loop(data);
	return ;
}

void	explorer_entry_func(void *item_ptr, void *data)
{
	t_item	*item;
	t_param	param;

	item = item_ptr;
	if (item->dirent.type == DT_DIR)
	{
		param = item->param;
		if (chdir(item->description) == 0)
			explorer_read_dir(data, &param);
	}
	else if (item->dirent.type == DT_REG)
		file_load(item, data);
	else
		ft_fprintf(STDERR_FILENO, "[Explorer] Invalid File Type: %s\n", \
				item->description);
}
