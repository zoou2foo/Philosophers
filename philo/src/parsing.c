/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriejean <valeriejean@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:17:01 by valeriejean       #+#    #+#             */
/*   Updated: 2023/03/21 16:20:11 by valeriejean      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_args(char **str)
{
	int	i;
	int	j;

	i = 1;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (!(str[i][j] >= 48 && str[i][j] <= 57))
			{
				printf("%s\n", ERR_TYPE_ARGS);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	check_len(char **str)
{
	int i;

	i = 1;
	while (str[i])
	{
		if (ft_strlen(str[i]) > 6)
		{
			printf("%s\n", ERR_INT);
			return (1);
		}
		i++;
	}
	return (0);
}

void	setup_datastruct(t_data *data, char **str)
{
	data->nb_philos = ft_atoi(str[1]);
	data->time_to_die = ft_atoi(str[2]);
	data->time_to_eat = ft_atoi(str[3]);
	data->time_to_sleep = ft_atoi(str[4]);
	data->someone_is_dead = 0;
	data->nb_full_philos = 0;
	data->nb_to_eat = 0;
	if (str[5] != NULL)
		data->nb_to_eat = ft_atoi(str[5]);
}

void	parsing(char **str, t_data *data) //time to check args and do atoi with args
{
	if (check_args(str) == 0)
	{
		if (check_len(str) == 0)
		{
			setup_datastruct(data, str);
			if (data->nb_philos < 1 || data->nb_philos > 200)
			{
				printf("%s\n", ERR_PHILO);
				return ;
			}
		}
	}
}
