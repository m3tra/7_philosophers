/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 18:50:26 by fporto            #+#    #+#             */
/*   Updated: 2022/04/03 23:42:36 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	err_exit(char *err, t_prop *prop)
{
	if (prop)
		free(prop);
	printf("Error\n%s\n", err);
	exit(EXIT_FAILURE);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	if (s1[i] == s2[i])
		return (0);
	else
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_isnumber(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < 48 || str[i] > 57)
			return (0);
		i++;
	}
	return (1);
}

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\f' || c == '\n' || c == '\r' \
	|| c == '\t' || c == '\v');
}

int	ft_atoi(const char *str)
{
	int	neg;
	int	pos;
	int	result;

	neg = 1;
	pos = 0;
	result = 0;
	while (*str && (ft_isspace(*str)))
		str++;
	if (*str && *str == '-')
	{
		neg *= -1;
		str++;
	}
	if (*str && *str == '+')
	{
		pos = 1;
		str++;
	}
	if (neg == -1 && pos == 1)
		return (result);
	while (*str >= 48 && *str <= 57)
		result = result * 10 + (*str++ - '0');
	return (result * neg);
}
