/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 18:36:01 by fporto            #+#    #+#             */
/*   Updated: 2022/05/23 15:44:34 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_event(t_philo *philo, const char *event)
{
	size_t	id;
	size_t	time_ms;

	id = *philo->id + 1;
	time_ms = get_runtime_ms(philo->prop);
	if (!ft_strcmp(event, "death"))
		printf("%ld %ld died\n", time_ms, id);
	else if (!ft_strcmp(event, "think"))
		printf("%ld %ld is thinking\n", time_ms, id);
	else if (!ft_strcmp(event, "fork"))
		printf("%ld %ld has taken a fork\n", time_ms, id);
	else if (!ft_strcmp(event, "eat"))
		printf("%ld %ld is eating\n", time_ms, id);
	else if (!ft_strcmp(event, "sleep"))
		printf("%ld %ld is sleeping\n", time_ms, id);
}

int	check_alive(t_philo *philo)
{
	t_prop	*prop;

	prop = philo->prop;
	pthread_mutex_lock(&prop->end_mutex);
	if ((get_runtime_ms(prop) - philo->last_meal_ms) > prop->time_alive_ms
		|| *prop->suicide)
	{
		if (prop->n_times_eat == -1)
			*prop->suicide = 1;
		pthread_mutex_unlock(&prop->end_mutex);
		return (0);
	}
	pthread_mutex_unlock(&prop->end_mutex);
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
	philo->meals_eaten++;
	usleep(prop->time_eat_ms * 1000);
	if (!place_forks(philo))
		return (0);
	philo->last_meal_ms = get_runtime_ms(prop);
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
		if (!check_alive(philo))
			break ;
		if (philo->prop->n_philo <= 1 || !eat(philo) || !check_alive(philo))
			break ;
		if (philo->meals_eaten == philo->prop->n_times_eat
			|| !check_alive(philo))
			break ;
		log_event(philo, "sleep");
		usleep(philo->prop->time_sleep_ms * 1000);
	}
	return ((void *)philo);
}
