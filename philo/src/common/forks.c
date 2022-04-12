/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 01:07:15 by fporto            #+#    #+#             */
/*   Updated: 2022/04/12 16:54:26 by fporto           ###   ########.fr       */
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

static int	place_forks(t_philo *philo)
{
	int		ret;
	t_prop	*prop;
	size_t	id;
	int		*forks;

	ret = 0;
	prop = philo->prop;
	id = *philo->id;
	forks = prop->forks;
	pthread_mutex_lock(&prop->forks_mutex[id]);
	pthread_mutex_lock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
	if (!forks[id] && !(forks[(id + 1) % prop->n_philo]))
	{
		forks[id] = 1;
		forks[(id + 1) % prop->n_philo] = 1;
		ret = 1;
	}
	pthread_mutex_unlock(&prop->forks_mutex[id]);
	pthread_mutex_unlock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
	return (ret);
}

static int	take_forks(t_philo *philo)
{
	int		ret;
	t_prop	*prop;
	size_t	id;
	int		*forks;

	ret = 0;
	prop = philo->prop;
	id = *philo->id;
	forks = prop->forks;
	pthread_mutex_lock(&prop->forks_mutex[id]);
	pthread_mutex_lock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
	if (forks[id] && (forks[(id + 1) % prop->n_philo]))
	{
		forks[id] = 0;
		forks[(id + 1) % prop->n_philo] = 0;
		ret = 1;
	}
	pthread_mutex_unlock(&prop->forks_mutex[id]);
	pthread_mutex_unlock(&prop->forks_mutex[(id + 1) % prop->n_philo]);
	return (ret);
}

int	flip_forks(t_philo *philo, int place)
{
	while (1)
	{
		if (!check_alive(philo))
			return (0);
		if ((place && place_forks(philo)) || (!place && take_forks(philo)))
			break ;
	}
	return (1);
}
