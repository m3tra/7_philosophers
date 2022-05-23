/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 18:38:33 by fporto            #+#    #+#             */
/*   Updated: 2022/05/23 15:44:06 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	err_exit(char *err, t_prop *prop)
{
	free_all(prop);
	printf("Error\n%s\n", err);
	exit(EXIT_FAILURE);
}

void	free_all(t_prop *prop)
{
	size_t	i;

	i = 0;
	if (prop)
	{
		while (i < prop->n_philo)
			pthread_mutex_destroy(&prop->forks_mutex[i++]);
		ft_free(prop->forks_mutex);
		pthread_mutex_destroy(&prop->end_mutex);
		ft_free(prop->suicide);
		ft_free(prop->th);
		ft_free(prop->forks);
		i = 0;
		while (i < prop->n_philo)
			ft_free(prop->philos[i++].id);
		ft_free(prop->philos);
		ft_free(prop);
	}
}

size_t	get_runtime_ms(t_prop *prop)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		err_exit("gettimeofday failed\n", prop);
	return (time.tv_sec * 1000 + time.tv_usec / 1000 - prop->start_time_ms);
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
	pthread_mutex_init(&prop->end_mutex, NULL);
	prop->forks_mutex = forks_mutex;
	return (1);
}

int	set_args(t_prop *prop, int argc, char **argv)
{
	struct timeval	time;

	if (!ft_isnumber(argv[1]) || !ft_isnumber(argv[2]) || !ft_isnumber(argv[3])
		|| !ft_isnumber(argv[4]) || (argc == 6 && !ft_isnumber(argv[5])))
		return (0);
	prop->suicide = malloc(sizeof(int));
	if (!prop->suicide)
		err_exit("Failed malloc", prop);
	*prop->suicide = 0;
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
