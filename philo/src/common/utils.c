/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 18:38:33 by fporto            #+#    #+#             */
/*   Updated: 2022/04/12 16:55:06 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_runtime_ms(t_prop *prop)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		err_exit("gettimeofday failed\n", prop);
	return (time.tv_sec * 1000 + time.tv_usec / 1000 - prop->start_time_ms);
}

void	free_all(t_prop *prop)
{
	size_t	i;

	i = 0;
	while (i < prop->n_philo)
		pthread_mutex_destroy(&prop->forks_mutex[i++]);
	if (prop->forks_mutex)
		free(prop->forks_mutex);
	if (prop->forks)
		free(prop->forks);
	free(prop);
}

void	log_event(t_philo *philo, const char *event)
{
	size_t	n_philo;
	size_t	time_ms;

	n_philo = *philo->id + 1;
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

int	init_mutex(t_prop *prop)
{
	pthread_mutex_t	*forks_mutex;
	size_t			i;

	forks_mutex = malloc(sizeof(pthread_mutex_t) * prop->n_philo);
	if (!forks_mutex)
		return (0);
	i = 0;
	while (i < prop->n_philo)
		pthread_mutex_init(&forks_mutex[i++], NULL);
	prop->forks_mutex = forks_mutex;
	return (1);
}

int	set_args(t_prop *prop, int argc, char **argv)
{
	struct timeval	time;

	if (!ft_isnumber(argv[1]) || !ft_isnumber(argv[2]) || !ft_isnumber(argv[3])
		|| !ft_isnumber(argv[4]) || (argc == 6 && !ft_isnumber(argv[5])))
		return (0);
	prop->n_times_eat = -1;
	if (argc == 6)
		prop->n_times_eat = ft_atoi(argv[5]);
	else if (argc != 5)
		err_exit("Wrong number of arguments\n", prop);
	prop->n_philo = ft_atoi(argv[1]);
	prop->time_alive_ms = ft_atoi(argv[2]);
	prop->time_eat_ms = ft_atoi(argv[3]);
	prop->time_sleep_ms = ft_atoi(argv[4]);
	if (prop->n_philo <= 0 || prop->time_alive_ms <= 0 || prop->time_eat_ms <= 0
		|| prop->time_sleep_ms <= 0)
	{
		printf("Error\nArgument too low\n", NULL);
		return (0);
	}
	if (gettimeofday(&time, NULL) == -1)
		return (1);
	prop->start_time_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (1);
}
