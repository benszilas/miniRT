/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:28:03 by victor            #+#    #+#             */
/*   Updated: 2024/10/30 17:58:53 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	data_init_threads(t_data *data)
{
	if (pthread_cond_init(&data->cond, NULL) != 0)
	{
		ft_fprintf(STDERR_FILENO, \
				"[ERROR] Failed to init pthread_cond\nExiting...");
		lst_memory(NULL, NULL, FAIL);
	}
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
	{
		ft_fprintf(STDERR_FILENO, \
				"[ERROR] Failed to init pthread_mutex\nExiting...");
		lst_memory(NULL, NULL, FAIL);
	}
	if (pthread_barrier_init(&data->barrier, NULL, THREAD_COUNT + 1) != 0)
	{
		ft_fprintf(STDERR_FILENO, \
				"[ERROR] Failed to init pthread_barrier\nExiting...");
		lst_memory(NULL, NULL, FAIL);
	}
}

void	threads_init(t_thread thread[], t_data *data)
{
	uint		i;

	data_init_threads(data);
	i = 0;
	while (i < THREAD_COUNT)
	{
		thread[i].scene = data->scene;
		thread[i].camera = &data->scene.camera;
		thread[i].width = WI;
		thread[i].data = data;
		thread[i].id = i;
		thread[i].starty = i * THREAD_HEIGHT;
		thread[i].pixel = data->pixel;
		thread[i].mutex = &data.mutex;
		pthread_create(&thread[i].thread, NULL, \
				thread_rendering_loop, &thread[i]);
		i++;
	}
}

void	thread_scene_update(t_data *data)
{
	uint	i;

	i = 0;
	while (i < THREAD_COUNT)
	{
		ft_memcpy(&data->threads[i].scene, &data->scene, sizeof(t_scene));
		i++;
	}
}

void	*thread_rendering_loop(void *thread_ptr)
{
	t_thread	*thread;

	thread = (t_thread *)thread_ptr;
	while (1)
	{
		pthread_mutex_lock(thread->mutex);
		while (thread->data->go == false)
			pthread_cond_wait(&thread->data->cond, thread->mutex);
		pthread_mutex_unlock(thread->mutex);
		thread_define_camera_rays(thread, thread->pixel, \
				&thread->scene, thread->camera);
		pthread_barrier_wait(&thread->data->barrier);
		usleep(1000);
	}
	return (NULL);
}
