/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 01:07:15 by fporto            #+#    #+#             */
/*   Updated: 2022/04/13 03:05:41 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_prop *prop)
{
	int		*forks;
	size_t	i;

	forks = malloc(sizeof(int) * prop->n_philo);
	if (!forks)
		return (0);
	i = 0;
	while (i < prop->n_philo)
		forks[i++] = 1;
	prop->forks = forks;
	return (1);
}

int	place_forks(t_philo *philo)
{
	t_prop	*prop;
	size_t	id;
	int		*forks;

	prop = philo->prop;
	id = *philo->id;
	forks = prop->forks;
	pthread_mutex_lock(&prop->forks_mutex[id]);
	forks[id] = 1;
	pthread_mutex_unlock(&prop->forks_mutex[id]);
	pthread_mutex_lock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
	forks[(id + 1) % prop->n_philo] = 1;
	pthread_mutex_unlock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
	if (!check_alive(philo))
		return (0);
	return (1);
}

int	take_forks(t_philo *philo)
{
	t_prop	*prop;
	size_t	id;

	prop = philo->prop;
	id = *philo->id;
	while (1)
	{
		if (!check_alive(philo))
			return (0);
		pthread_mutex_lock(&prop->forks_mutex[id]);
		pthread_mutex_lock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
		if (prop->forks[id] && (prop->forks[(id + 1) % prop->n_philo]))
		{
			prop->forks[id] = 0;
			pthread_mutex_unlock(&prop->forks_mutex[id]);
			log_event(philo, "fork");
			prop->forks[(id + 1) % prop->n_philo] = 0;
			pthread_mutex_unlock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
			log_event(philo, "fork");
			return (1);
		}
		pthread_mutex_unlock(&prop->forks_mutex[id]);
		pthread_mutex_unlock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
	}
}
