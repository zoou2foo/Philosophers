/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:30:49 by vjean             #+#    #+#             */
/*   Updated: 2023/03/31 12:05:04 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FOUR FUNCTIONS		*/

//function to initialize main variables in my data struct
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

//function to init each philo struct
void	init_philo(t_data *data, int i)
{
	data->philo_struct[i].id = i + 1;
	data->philo_struct[i].data = data;
	data->philo_struct[i].last_meal = 0;
	data->philo_struct[i].nb_meals_enjoyed = 0;
	pthread_mutex_init(&data->forks_mutex[i], NULL);
}

//function to initialize each mutex
void	init_singles_mutex(t_data *data)
{
	pthread_mutex_init(&(data->print_mutex), NULL);
	pthread_mutex_init(&(data->full_mutex), NULL);
	pthread_mutex_init(&(data->count_full), NULL);
	pthread_mutex_init(&data->dead_body, NULL);
	pthread_mutex_init(&data->really_dead, NULL);
	//pthread_mutex_init(&data->last_meal_mutex, NULL);
}

//loop function to init each philo struct
void	init_philo_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		init_philo(data, i);
		i++;
	}
}