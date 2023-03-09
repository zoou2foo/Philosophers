/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:31:45 by valeriejean       #+#    #+#             */
/*   Updated: 2023/03/09 15:33:33 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_atoi(char *str) //change args in int
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			sign *= -1;
			i++;
		}
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	return (result * sign);
}

void	time_stamp(t_data *data)
{
	struct timeval	zero_time;
	struct timeval	current_time;
	int				i;

	i = 0;
	gettimeofday(&zero_time, NULL);
	while (i <= data->time_to_eat)
	{
		gettimeofday(&current_time, NULL);
		data->elapsed_time_ms = (current_time.tv_sec - zero_time.tv_sec) * 1000 + (current_time.tv_usec - zero_time.tv_usec) / 1000;
		printf("%ld - ", data->elapsed_time_ms);
		usleep(500000);
	}
}

/*
void	smart_sleep()
{

}

*/