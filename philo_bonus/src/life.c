/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 18:36:01 by fporto            #+#    #+#             */
/*   Updated: 2022/04/09 20:12:22 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	log_event(t_philo *philo, const char *event)
{
	size_t	n_philo;
	size_t	time_ms;

	n_philo = philo->id + 1;
	time_ms = get_runtime_ms(philo->prop);
	if (!ft_strcmp(event, "death"))
		printf("%ld %ld died\n", time_ms, n_philo);
	else if (!ft_strcmp(event, "think"))
		printf("%ld %ld is thinking\n", time_ms, n_philo);
	else if (!ft_strcmp(event, "fork"))
		printf("%ld %ld has taken a fork\n", time_ms, n_philo);
	else if (!ft_strcmp(event, "eat"))
		printf("%ld %ld is eating\n", time_ms, n_philo);
	else if (!ft_strcmp(event, "sleep"))
		printf("%ld %ld is sleeping\n", time_ms, n_philo);
}

static void	is_alive(t_philo *philo)
{
	t_prop	*prop;
	size_t	i;

	prop = philo->prop;
	if ((get_runtime_ms(prop) - philo->last_meal_ms) > prop->time_alive_ms)
	{
		i = (size_t) - 1;
		while (++i < philo->forks_in_hand)
			sem_post(philo->prop->forks);
		log_event(philo, "death");
		sem_post(prop->died);
		while (1)
		{
		}
	}
}

static void	place_forks(t_philo *philo, int place)
{
	is_alive(philo);
	if (place)
	{
		sem_post(philo->prop->forks);
		sem_post(philo->prop->forks);
		philo->forks_in_hand = 0;
	}
	else
	{
		sem_wait(philo->prop->forks);
		philo->forks_in_hand = 1;
		log_event(philo, "fork");
		is_alive(philo);
		sem_wait(philo->prop->forks);
		philo->forks_in_hand = 2;
		log_event(philo, "fork");
		is_alive(philo);
	}
}

static void	eat(t_philo *philo)
{
	place_forks(philo, 0);
	log_event(philo, "eat");
	usleep(philo->prop->time_eat_ms * 1000);
	philo->meals_eaten++;
	place_forks(philo, 1);
	if (philo->meals_eaten == philo->prop->n_times_eat)
	{
		sem_post(philo->prop->done);
		while (philo->prop->n_times_eat != ((size_t) - 1))
		{
		}
	}
}

void	life(t_philo philo)
{
	philo.last_meal_ms = 0;
	philo.meals_eaten = 0;
	philo.forks_in_hand = 0;
	while (1)
	{
		is_alive(&philo);
		log_event(&philo, "think");
		philo.last_meal_ms = get_runtime_ms(philo.prop);
		if (philo.prop->n_philo == 1)
			break ;
		eat(&philo);
		is_alive(&philo);
		log_event(&philo, "sleep");
		usleep(philo.prop->time_sleep_ms * 1000);
	}
	sem_post(philo.prop->died);
	log_event(&philo, "death");
	while (1)
	{
	}
}
