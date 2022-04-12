/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 03:59:44 by fporto            #+#    #+#             */
/*   Updated: 2022/04/12 16:54:37 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <stddef.h>
# include <unistd.h>
# include <string.h>

# include <pthread.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/time.h>
# include <time.h>

typedef struct s_prop
{
	size_t			start_time_ms;
	pthread_mutex_t	*forks_mutex;
	pthread_t		*th;
	size_t			n_philo;
	size_t			time_alive_ms;
	size_t			time_eat_ms;
	size_t			time_sleep_ms;
	long			n_times_eat;
	int				*forks;
}	t_prop;

typedef struct s_philo
{
	struct s_philo	*philos;
	t_prop			*prop;
	size_t			*id;
	size_t			last_meal_ms;
	int				meals_eaten;
}	t_philo;

void	err_exit(char *err, t_prop *prop);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_isnumber(const char *str);
int		ft_atoi(const char *str);

void	exterminate(t_philo *philos);
int		check_alive(t_philo *philo);
void	*life(void *arg);

int		eat(t_philo *philo);
int		init_forks(t_prop *prop);
int		flip_forks(t_philo *philo, int place);

size_t	get_runtime_ms(t_prop *prop);
void	free_all(t_prop *prop);
void	log_event(t_philo *philo, const char *event);
int		init_mutex(t_prop *prop);
int		set_args(t_prop *prop, int argc, char **argv);

#endif
