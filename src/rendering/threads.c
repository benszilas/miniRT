/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:28:03 by victor            #+#    #+#             */
/*   Updated: 2024/11/07 17:05:28 by bszilas          ###   ########.fr       */
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
		/*block until main thread has done writing*/
		pthread_rwlock_rdlock(thread->rwlock);
		
		/*if Esc is pressed this condition breaks the loop*/
		if (thread->data->go == false)
			break ;
			
		/*tell main thread that everyone locked read*/
		pthread_barrier_wait(&thread->data->barrier);

		/*this replaces the thread_scene_update function*/
		scene = *thread->scene;
		
		thread_define_camera_rays(thread, scene.pixel, &scene, &scene.camera);
		
		/*tell main thread that its finished*/
		pthread_rwlock_unlock(thread->rwlock);
		
		/*synchronize until main thread acquires write lock*/
		pthread_barrier_wait(&thread->data->barrier);
	}
	pthread_rwlock_unlock(thread->rwlock);
	return (NULL);
}
