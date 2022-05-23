/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 03:59:44 by fporto            #+#    #+#             */
/*   Updated: 2022/05/23 15:56:54 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdlib.h>
# include <stdio.h>
# include <stddef.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>

# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/time.h>
# include <time.h>
# include <signal.h>
# include <pthread.h>
# include <semaphore.h>

typedef struct s_prop
{
	int			*pids;
	sem_t		*forks;
	sem_t		*done;
	sem_t		*died;
	size_t		n_philo;
	size_t		start_time_ms;
	size_t		time_alive_ms;
	size_t		time_eat_ms;
	size_t		time_sleep_ms;
	size_t		n_times_eat;
	pthread_t	*rip_thread;
	pthread_t	*done_thread;
	int			thread_switch;
}	t_prop;

typedef struct s_philo
{
	t_prop	*prop;
	size_t	id;
	size_t	last_meal_ms;
	size_t	meals_eaten;
	size_t	forks_in_hand;
}	t_philo;

void	free_all(t_prop *prop, int err);
void	err_exit(char *err, t_prop *prop);
size_t	get_runtime_ms(t_prop *prop);
void	*ripper(void *arg);
void	*completionist(void *arg);

void	ft_free(void *ptr);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_isnumber(const char *str);
int		ft_atoi(const char *str);

void	life(t_philo philo);

#endif
