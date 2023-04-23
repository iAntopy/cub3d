/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 03:31:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/23 14:14:31 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
/*
typedef struct s_draw_threads
{
	pthread_mutex_t	start_locks[NB_DRAW_THREADS];
	pthread_mutex_t	idle_locks[NB_DRAW_THREADS];
	int			thread_ids[NB_DRAW_THREADS];
	int			start_locks_isinit[NB_DRAW_THREADS];
	int			idle_locks_isinit[NB_DRAW_THREADS];
	int			isidle[NB_DRAW_THREADS];
	int			stop_request[NB_DRAW_THREADS];
	t_thprof		thread_profils[NB_DRAW_THREADS];
}	t_thdraw;

typedef int (t_draw_func *)(t_cub *cub);

typedef struct s_draw_thread_profil
{
	t_cub		*cub;
	pthread_mutex_t	start_lock;
	pthread_mutex_t	end_lock;
	t_draw_func	draw_func;
	int			start_lock_isinit;
	int			end_lock_isinit;
	int			id;
	int			isidle;
	int			stop_request;
}	t_thdraw;
*/

void	*__draw_thread_routine(t_thdraw *th)
{
	if (!th)
		return ((void *)1);
	printf("Draw routine started : thread id : %zd\n", th->id);
	th->isidle = 1;
	while (!pthread_mutex_lock(&th->end_lock) && !pthread_mutex_unlock(&th->end_lock))
	{
		printf("thread : waiting for start_lock\n");
		pthread_mutex_lock(&th->start_lock);
		printf("thread : start_lock acquired\n");
		th->isidle = 0;
		if (th->stop_request)
			break ;
		th->draw_func(th->cub, th->cub->hero.rcast.rdata);
		printf("thread unlocking start_lock\n");
		pthread_mutex_unlock(&th->start_lock);
		printf("thread unlocking start_lock DONE\n");
		th->isidle = 1;
	}
	pthread_mutex_unlock(&th->start_lock);
	return (NULL);
}

void	stop_draw_threads(t_thdraw *threads)
{
	int	i;

	printf("Stopping draw threads \n");
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
	printf("Draw threads STOPPED\n");
}

int	report_threads_err(t_thdraw *threads, char *err, int print_strerr)
{
	stop_draw_threads(threads);
	if (print_strerr)
		fperror("Error\n\t - %s", err);
	else
		report_err(err);
	return (-1);
}

int	order_draw_call(t_thdraw *threads)
{
	int	i;

	i = -1;
	while (++i < NB_DRAW_THREADS)
	{
		pthread_mutex_lock(&threads[i].end_lock);
		pthread_mutex_unlock(&threads[i].start_lock);
	}
//	printf("WOWOW :: start_lock unlocked ! Drawing begins !\n");
	usleep(10);
	i = -1;
	printf("Start spinnin'. thread 0 is idle %d\n", threads[i].isidle);
	while (++i < NB_DRAW_THREADS)
		if (threads[i].isidle)
			i = -1;
	printf("Stop spinnin'\n");
//	printf("WOWOW :: try lock start_lock\n");
	i = -1;
	while (++i < NB_DRAW_THREADS)
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
		if (pthread_create(&threads[i].id, NULL, (void *(*)(void *))__draw_thread_routine,
			threads + i) < 0)
			return (report_threads_err(threads, "Thread creation failed.", 1));
	}
	return (0);
}

int	init_draw_threads(t_cub *cub, t_thdraw *threads)
{
	int	i;

	printf("init draw threads : threads ptr : %p \n", threads);
	printf("threads ptr : %p\n", threads);
	if (!cub || !threads || threads[0].id != 0)
		return (report_err("missing threads struct info or already initialized."));
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
	/// Associate each thread to their draw_func. Add more as needed and 
	/// raise NB_DRAW_THREADS to the amount necessary.
	threads[0].draw_func = render_walls;
	/// ...
	start_draw_threads(threads);
	return (0);
}
