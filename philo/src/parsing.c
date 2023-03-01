/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:17:01 by valeriejean       #+#    #+#             */
/*   Updated: 2023/03/01 14:58:55 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


//If only numbers, check if I have at least 2 philos; no negative time and max 250 philos.

int	check_args(char **str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (!(str[i][0] >= 48 && str[i][0] <= 57))
		{
			printf("%s\n", ERR_TYPE_ARGS);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (0);
}


void	setup_datastruct(t_data *data, char **str)
{
	data->nb_philos = ft_atoi(str[1]);
	data->time_to_death = ft_atoi(str[2]);
	data->time_to_eat = ft_atoi(str[3]);
	data->time_to_sleep = ft_atoi(str[4]);
	if (str[5])
		data->nb_to_eat = ft_atoi(str[5]);
	printf("Nb de philos: %d\n", data->nb_philos);
	printf("Temps pour mourir: %d\n", data->time_to_death);
	printf("Temps pour manger: %d\n", data->time_to_eat);
	printf("Temps pour dormir: %d\n", data->time_to_sleep);
	printf("Nb de fois pour manger: %d\n", data->nb_to_eat);
}

void	parsing(char **str) //time to check args and do atoi with args
{
	t_data	*data;

	if (check_args(str) == 0)
	{
		data = malloc(sizeof(t_philo) * 1 + 1); //maybe need to do it somewhere else
		setup_datastruct(data, str);
		if (data->nb_philos < 2)
		{
			printf("%s\n", ERR_PHILO);
			exit(EXIT_FAILURE);
		}
	}
}
