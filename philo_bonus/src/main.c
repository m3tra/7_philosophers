/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:24:15 by fporto            #+#    #+#             */
/*   Updated: 2022/04/09 20:10:28 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	check_args(t_prop *prop, int argc, char **argv)
{
	if (!ft_isnumber(argv[1]) || !ft_isnumber(argv[2]) || !ft_isnumber(argv[3])
		|| !ft_isnumber(argv[4]) || (argc == 6 && !ft_isnumber(argv[5])))
		return (0);
	prop->n_philo = ft_atoi(argv[1]);
	prop->time_alive_ms = ft_atoi(argv[2]);
	prop->time_eat_ms = ft_atoi(argv[3]);
	prop->time_sleep_ms = ft_atoi(argv[4]);
	if (prop->n_philo <= 0 || prop->time_alive_ms <= 0 || prop->time_eat_ms <= 0
		|| prop->time_sleep_ms <= 0)
		free_all(prop, 1);
	return (1);
}

static void	set_args(t_prop *prop, int argc, char **argv)
{
	struct timeval	time;
	int				temp;
	pthread_mutex_t	fork_mutex;

	pthread_mutex_init(&fork_mutex, NULL);
	prop->n_times_eat = (size_t) -1;
	if (argc == 6)
	{
		if (!ft_isnumber(argv[5]))
			free_all(prop, 1);
		temp = ft_atoi(argv[5]);
		if (temp <= 0)
			free_all(prop, 1);
		prop->n_times_eat = temp;
	}
	else if (argc != 5)
		free_all(prop, 1);
	if (!check_args(prop, argc, argv))
		free_all(prop, 1);
	if (gettimeofday(&time, NULL) == -1)
		free_all(prop, 1);
	prop->start_time_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
}

static void	init_global(t_prop *prop)
{
	prop->rip_thread = malloc(sizeof(pthread_t));
	prop->done_thread = malloc(sizeof(pthread_t));
	if (!prop->rip_thread || !prop->done_thread)
		free_all(prop, 1);
	prop->pids = malloc(sizeof(int) * prop->n_philo);
	if (!prop->pids)
		free_all(prop, 1);
	sem_unlink("/forks");
	sem_unlink("/done");
	sem_unlink("/died");
	prop->forks = sem_open("/forks", O_CREAT, 660, prop->n_philo);
	prop->done = sem_open("/done", O_CREAT, 660, 0);
	prop->died = sem_open("/died", O_CREAT, 660, 0);
	if (prop->forks == SEM_FAILED || prop->done == SEM_FAILED
		|| prop->died == SEM_FAILED)
		free_all(prop, 1);
	sem_post(prop->died);
	sem_post(prop->done);
	prop->thread_switch = 0;
}

static void	create_child(t_prop *prop)
{
	t_philo	philo;
	int		pid;
	size_t	i;

	philo.prop = prop;
	pthread_create(prop->rip_thread, NULL, ripper, prop);
	pthread_create(prop->done_thread, NULL, completionist, prop);
	i = -1;
	while (++i < prop->n_philo)
	{
		philo.id = i;
		pid = fork();
		if (pid == -1)
			free_all(prop, 1);
		if (!pid)
			life(philo);
		prop->pids[i] = pid;
	}
	pthread_join(*prop->rip_thread, NULL);
	pthread_join(*prop->done_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_prop	*prop;

	prop = malloc(sizeof(t_prop));
	if (!prop)
		free_all(NULL, 1);
	set_args(prop, argc, argv);
	init_global(prop);
	create_child(prop);
	free_all(prop, 0);
	return (0);
}
