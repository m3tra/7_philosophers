/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 18:36:01 by fporto            #+#    #+#             */
/*   Updated: 2022/04/04 04:19:37 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	exterminate(t_philo *philos)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = philos[0].prop->n_philo;
	while (i < size)
		free(philos[i++].id);
	free(philos);
}

int	check_alive(t_philo *philo)
{
	t_prop	*prop;

	prop = philo->prop;
	if ((get_runtime_ms(prop) - philo->last_meal_ms) > prop->time_alive_ms)
		return (0);
	return (1);
}

int	eat(t_philo *philo)
{
	size_t	time_ms;

	if (!flip_forks(philo, 0))
		return (0);
	time_ms = get_runtime_ms(philo->prop);
	log_event(philo, "fork");
	log_event(philo, "eat");
	usleep(philo->prop->time_eat_ms * 1000);
	if (philo->prop->n_times_eat > 0)
		philo->prop->n_times_eat--;
	if (!flip_forks(philo, 1))
		return (0);
	return (1);
}

void	*life(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = arg;
	philo->last_meal_ms = 0;
	i = 0;
	while (1)
	{
		log_event(philo, "think");
		if (philo->prop->n_philo > 1 && !eat(philo))
			break ;
		if (philo->prop->n_times_eat != -1)
			i++;
		if (philo->prop->n_times_eat == i)
			break ;
		philo->last_meal_ms = get_runtime_ms(philo->prop);
		if (!philo->prop->n_times_eat)
			break ;
		if (!check_alive(philo))
			break ;
		log_event(philo, "sleep");
		usleep(philo->prop->time_sleep_ms * 1000);
	}
	return ((void *)philo);
}
