/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:33:29 by victor            #+#    #+#             */
/*   Updated: 2024/12/05 22:35:36 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
#include <mlx.h>

void	data_destroy_func(void *data_ptr)
{
	t_data	*data;

	data = data_ptr;
	ft_free(&data->image.image);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	ft_free(&data->mlx);
	data->go = false;
	pthread_rwlock_unlock(&data->rwlock);
	while (data->thread_count--)
		pthread_join(data->threads[data->thread_count].thread, NULL);
	pthread_barrier_destroy(&data->barrier);
	pthread_rwlock_destroy(&data->rwlock);
}

t_container	*menu_init(t_data *data)
{
	t_container	*menu;
	t_item		item;

	menu = ft_calloc(16, sizeof(*menu));
	lst_memory(menu, free, ADD);
	menu[ID_GROUP_MENU_MAIN - 1] = container_create("Main menu", \
			NULL, CONTAINER_GRID);
	item = container_item_button_create("add sphere", \
			NULL, scene_add_sphere_func);
	container_item_add(&menu[ID_GROUP_MENU_MAIN - 1], &item);
	item = container_item_button_create("add plane", NULL, \
			scene_add_plane_func);
	container_item_add(&menu[ID_GROUP_MENU_MAIN - 1], &item);
	item = container_item_button_create("add cylinder", NULL, \
			scene_add_cylinder);
	container_item_add(&menu[ID_GROUP_MENU_MAIN - 1], &item);
	item = container_item_button_create("add disk", NULL, \
			scene_add_disk);
	container_item_add(&menu[ID_GROUP_MENU_MAIN - 1], &item);
	item = container_item_button_create("show_help", NULL, help_menu_draw);
	container_item_add(&menu[ID_GROUP_MENU_MAIN - 1], &item);
	item = container_item_button_create("Load Scene", NULL, explorer_read_dir);
	item.param = (t_param){&data->scene, NULL, PARAM_SCENE};
	container_item_add(&menu[ID_GROUP_MENU_MAIN - 1], &item);
	return (menu);
}

t_container	*menus_create(t_data *data)
{
	t_container	*menu;
	uint		i;

	menu = menu_init(data);
	sphere_menu_create(&menu[ID_GROUP_SPHERE]);
	plane_menu_create(&menu[ID_GROUP_PLANE]);
	cone_menu_create(&menu[ID_GROUP_CONE]);
	disc_menu_create(&menu[ID_GROUP_DISC]);
	cylinder_menu_create(&menu[ID_GROUP_CYLINDER]);
	i = 0;
	while (i < 16)
		menu[i++].data = data;
	return (menu);
}

void	initialize_data(t_data *data, char *path)
{
	ft_bzero(data, sizeof(*data));
	scene_create(path, &data->scene);
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		ft_putendl_fd("Failed to initialize mlx", 2);
		exit (1);
	}
	data->win = mlx_new_window(data->mlx, WI, HI, "MiniRT");
	if (!data->win)
	{
		ft_putendl_fd("Failed to initialize window", 2);
		mlx_destroy_display(data->mlx);
		ft_free(data->mlx);
		exit (1);
	}
	data->pixel = pixel_plane_create();
	data->func_ptr = help_menu_draw;
	data->mouse.data = data;
	data->scene.pixel = data->pixel;
	data->image = image_create(data->mlx, WI, HI);
	data->menu = menus_create(data);
	pixels_image_syncronize(&data->image, data->pixel);
}

char* 	validate_file_extension(int argc, char **argv)
{
	if (argc == 2 && ft_strlen(argv[1]) > 3 && ft_memcmp(&argv[1][ft_strlen(argv[1]) - 3], ".rt\0", 4) == 0)
		return argv[1];
	ft_fprintf(STDERR_FILENO, "Invalid Argument to Program!\nExiting...\n");
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv)
{
	t_data		data;
	char		*path;
	t_thread	thread[THREAD_COUNT];

	if (argc == 1)
		path = "scenes/multilight.rt";
	else
		path = validate_file_extension(argc, argv);
	initialize_data(&data, path);
	lst_memory(&data, data_destroy_func, ADD);
	threads_init(thread, &data);
	data.threads = thread;
	mlx_hook(data.win, KeyPress, KeyPressMask, key_press, &data);
	mlx_mouse_hook(data.win, mouse_press, &data);
	mlx_hook(data.win, MotionNotify, PointerMotionMask, mouse_move, &data);
	mlx_hook(data.win, ButtonRelease, ButtonReleaseMask, mouse_release, &data);
	mlx_hook(data.win, DestroyNotify, ButtonPressMask, &close_window, &data);
	mlx_loop(data.mlx);
	return (0);
}
