/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:24:15 by fporto            #+#    #+#             */
/*   Updated: 2022/05/23 15:43:20 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philo	*init_philos(t_prop *prop)
{
	t_philo	*philos;
	size_t	i;

	philos = malloc(sizeof(t_philo) * prop->n_philo);
	if (!philos)
		err_exit("Failed malloc\n", prop);
	i = 0;
	prop->philos = philos;
	while (i < prop->n_philo)
	{
		philos[i].id = malloc(sizeof(size_t));
		if (!philos[i].id)
			err_exit("Failed malloc\n", prop);
		*(philos[i].id) = i;
		philos[i].prop = prop;
		philos[i++].philos = philos;
	}
	return (philos);
}

static int	create_threads(t_prop *prop, pthread_t *th)
{
	t_philo	*philos;
	size_t	i;

	i = 0;
	philos = init_philos(prop);
	while (i < prop->n_philo)
	{
		if (pthread_create(&th[i], NULL, &life, &philos[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int	join_threads(t_prop *prop)
{
	t_philo	*philo;
	size_t	i;
	size_t	dead;

	i = 0;
	dead = 0;
	while (i < prop->n_philo && dead < prop->n_philo)
	{
		if (pthread_join(prop->th[i], (void **)&philo))
			printf("Error joining thread\n");
		log_event(philo, "death");
		dead++;
		if (prop->n_times_eat == -1)
		{
			pthread_mutex_lock(&prop->end_mutex);
			*prop->suicide = 1;
			pthread_mutex_unlock(&prop->end_mutex);
		}
		i++;
	}
	return (1);
}

static int	overwatch(t_prop *prop)
{
	prop->th = malloc(sizeof(pthread_t) * prop->n_philo);
	if (!prop->th)
		err_exit("Failed to malloc thread array\n", prop);
	if (!create_threads(prop, prop->th))
		return (0);
	usleep(10000);
	return (1);
}

int	main(int argc, char **argv)
{
	t_prop	*prop;
	size_t	i;

	if (argc < 5 || argc > 6)
		err_exit("Invalid number of arguments", NULL);
	i = 0;
	prop = malloc(sizeof(t_prop));
	if (!prop)
		err_exit("Failed malloc", NULL);
	prop->n_times_eat = -1;
	if (argc == 6)
		prop->n_times_eat = ft_atoi(argv[5]);
	if (!set_args(prop, argc, argv) || !init_mutex(prop) || !init_forks(prop)
		|| !overwatch(prop))
	{
		free_all(prop);
		return (1);
	}
	join_threads(prop);
	free_all(prop);
	return (0);
}
