/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 18:36:01 by fporto            #+#    #+#             */
/*   Updated: 2022/04/13 03:17:44 by fporto           ###   ########.fr       */
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
	t_prop	*prop;
	size_t	id;

	prop = philo->prop;
	id = *philo->id;
	if (!take_forks(philo))
		return (0);
	if (!check_alive(philo))
		if (!prop->forks[id] && !(prop->forks[(id + 1) % prop->n_philo]))
			place_forks(philo);
	log_event(philo, "eat");
	usleep(philo->prop->time_eat_ms * 1000);
	if (philo->prop->n_times_eat != -1)
		philo->meals_eaten++;
	if (!place_forks(philo))
		return (0);
	philo->last_meal_ms = get_runtime_ms(philo->prop);
	return (1);
}

void	*life(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = arg;
	philo->last_meal_ms = 0;
	philo->meals_eaten = 0;
	i = 0;
	while (1)
	{
		if (!check_alive(philo))
			break ;
		log_event(philo, "think");
		if (philo->prop->n_philo <= 1 || !eat(philo) || !check_alive(philo))
			break ;
		if (philo->meals_eaten == philo->prop->n_times_eat || !check_alive(philo))
			break ;
		log_event(philo, "sleep");
		usleep(philo->prop->time_sleep_ms * 1000);
	}
	return ((void *)philo);
}
