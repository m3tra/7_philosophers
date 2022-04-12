/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 18:38:33 by fporto            #+#    #+#             */
/*   Updated: 2022/04/09 19:51:09 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	free_all(t_prop *prop, int err)
{
	size_t	i;

	i = (size_t) -1;
	if (prop)
	{
		if (prop->pids)
			free(prop->pids);
		if (prop->forks != SEM_FAILED)
			sem_close(prop->forks);
		if (prop->done != SEM_FAILED)
			sem_close(prop->done);
		if (prop->died != SEM_FAILED)
			sem_close(prop->died);
		if (prop->rip_thread)
			free(prop->rip_thread);
		if (prop->done_thread)
			free(prop->done_thread);
		free(prop);
	}
	if (err)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

size_t	get_runtime_ms(t_prop *prop)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		free_all(prop, 1);
	return (time.tv_sec * 1000 + time.tv_usec / 1000 - prop->start_time_ms);
}

void	*ripper(void *arg)
{
	t_prop	*prop;
	size_t	i;

	prop = arg;
	sem_wait(prop->died);
	sem_wait(prop->died);
	if (prop->thread_switch)
		return (NULL);
	prop->thread_switch = 1;
	i = 0;
	while (i < prop->n_philo)
	{
		sem_post(prop->done);
		kill(prop->pids[i++], SIGTERM);
	}
	return (NULL);
}

void	*completionist(void *arg)
{
	t_prop	*prop;
	size_t	i;

	prop = arg;
	i = 0;
	sem_wait(prop->done);
	while (i++ < prop->n_philo)
		sem_wait(prop->done);
	if (prop->thread_switch)
		return (NULL);
	prop->thread_switch = 1;
	i = 0;
	while (i < prop->n_philo)
		kill(prop->pids[i++], SIGKILL);
	sem_post(prop->died);
	return (NULL);
}
