/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:28:03 by victor            #+#    #+#             */
/*   Updated: 2024/11/26 18:52:48 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	data_init_threads(t_data *data)
{
	if (pthread_barrier_init(&data->barrier, NULL, THREAD_COUNT + 1) != 0)
	{
		ft_fprintf(STDERR_FILENO, \
				"[ERROR] Failed to init pthread_barrier\nExiting...");
		lst_memory(NULL, NULL, FAIL);
	}
	if (pthread_rwlock_init(&data->rwlock, NULL) != 0)
	{
		ft_fprintf(STDERR_FILENO, \
				"[ERROR] Failed to init pthread_rwlock\nExiting...");
		lst_memory(NULL, NULL, FAIL);
	}
}

void	threads_init(t_thread thread[], t_data *data)
{
	uint	i;

	data_init_threads(data);
	pthread_rwlock_wrlock(&data->rwlock);
	data->go = true;
	i = 0;
	while (i < THREAD_COUNT)
	{
		thread[i].width = WI;
		thread[i].data = data;
		thread[i].id = i;
		thread[i].starty = i * THREAD_HEIGHT;
		thread[i].pixel = data->pixel;
		thread[i].scene = &data->scene;
		thread[i].rwlock = &data->rwlock;
		if (pthread_create(&thread[i].thread, NULL, \
				thread_rendering_loop, &thread[i]) != 0)
			key_press(XK_Escape, data);
		i++;
		data->thread_count = i;
	}
	rendering_loop(data);
}

void	*thread_rendering_loop(void *thread_ptr)
{
	t_thread	*thread;
	t_scene		scene;

	thread = (t_thread *)thread_ptr;
	while (1)
	{
		pthread_rwlock_rdlock(thread->rwlock);
		if (thread->data->go == false)
			break ;
		pthread_barrier_wait(&thread->data->barrier);
		scene = *thread->scene;
		thread_define_camera_rays(thread, scene.pixel, &scene, &scene.camera);
		pthread_rwlock_unlock(thread->rwlock);
		pthread_barrier_wait(&thread->data->barrier);
	}
	pthread_rwlock_unlock(thread->rwlock);
	return (NULL);
}
