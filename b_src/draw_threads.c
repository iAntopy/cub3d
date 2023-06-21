/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 03:31:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/20 20:43:26 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	*__draw_thread_routine(t_thdraw *th)
{
	if (!th)
		return ((void *)1);
	th->isidle = 1;
	while (!pthread_mutex_lock(&th->end_lock)
		&& !pthread_mutex_unlock(&th->end_lock))
	{
		pthread_mutex_lock(&th->start_lock);
		th->isidle = 0;
		if (th->stop_request)
			break ;
		if (th->draw_func)
			th->draw_func(th->cub);
		pthread_mutex_unlock(&th->start_lock);
		th->isidle = 1;
	}
	pthread_mutex_unlock(&th->start_lock);
	return (NULL);
}

void	stop_draw_threads(t_thdraw *threads)
{
	int	i;

	i = -1;
	while (++i < NB_DRAW_THREADS && threads[i].start_lock_isinit
		&& threads[i].end_lock_isinit)
	{
		threads[i].stop_request = 1;
		if (threads[i].isidle)
			pthread_mutex_unlock(&threads[i].start_lock);
	}
	i = -1;
	while (++i < NB_DRAW_THREADS)
	{
		if (threads[i].id)
			pthread_join(threads[i].id, NULL);
		if (threads[i].start_lock_isinit)
			pthread_mutex_destroy(&threads[i].start_lock);
		if (threads[i].end_lock_isinit)
			pthread_mutex_destroy(&threads[i].end_lock);
	}
}

// from - to, are ints of the range of threads to order draw with.
// Used to split world draw threads call and portal projection draw threads.
int	order_draw_call(t_thdraw *threads, int from, int to)
{
	int			i;
	int			nb_spins;

	i = from - 1;
	while (++i < to)
	{
		pthread_mutex_lock(&threads[i].end_lock);
		pthread_mutex_unlock(&threads[i].start_lock);
	}
	i = from - 1;
	nb_spins = 10;
	while (++i < to && --nb_spins)
	{
		if (threads[i].isidle)
			i = -1;
		usleep(10);
	}
	i = from - 1;
	while (++i < to)
	{
		pthread_mutex_lock(&threads[i].start_lock);
		pthread_mutex_unlock(&threads[i].end_lock);
	}
	return (0);
}

static int	start_draw_threads(t_thdraw *threads)
{
	int	i;

	i = -1;
	while (++i < NB_DRAW_THREADS)
	{
		pthread_mutex_lock(&threads[i].start_lock);
		if (pthread_create(&threads[i].id, NULL,
				(void *(*)(void *))__draw_thread_routine, threads + i) < 0)
			return (report_threads_err(threads, "Thread creation failed.", 1));
	}
	return (0);
}

// Associate each thread to their draw_func. Add more as needed and 
// raise NB_DRAW_THREADS to the amount necessary.
int	init_draw_threads(t_cub *cub, t_thdraw *threads)
{
	int	i;

	if (!cub || !threads || threads[0].id != 0)
		return (report_err("missing threads struct or already initialized."));
	i = -1;
	while (++i < NB_DRAW_THREADS)
	{
		if (pthread_mutex_init(&threads[i].start_lock, NULL) < 0)
			return (report_threads_err(threads, "A mutex failed to init", 1));
		threads[i].start_lock_isinit = 1;
		if (pthread_mutex_init(&threads[i].end_lock, NULL) < 0)
			return (report_threads_err(threads, "A mutex failed to init", 1));
		threads[i].end_lock_isinit = 1;
		threads[i].cub = cub;
	}
	threads[0].draw_func = render_walls;
	//threads[0].draw_func = NULL;
	threads[1].draw_func = render_floor_sky;
	//threads[1].draw_func = NULL;
	threads[2].draw_func = render_objects;
	//threads[2].draw_func = NULL;
	threads[3].draw_func = __render_proj_objects;
	threads[4].draw_func = __render_proj_walls;
	threads[5].draw_func = __render_proj_floor;
	start_draw_threads(threads);
	return (0);
}
