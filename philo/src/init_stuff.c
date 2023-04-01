/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriejean <valeriejean@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:30:49 by vjean             #+#    #+#             */
/*   Updated: 2023/04/01 19:16:58 by valeriejean      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FOUR FUNCTIONS		*/

//to initialize the data struct
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

//initialize each philo struct
void	init_philo(t_data *data, int i)
{
	data->philo_struct[i].id = i + 1;
	data->philo_struct[i].data = data;
	data->philo_struct[i].last_meal = 0;
	data->philo_struct[i].nb_meals_enjoyed = 0;
	data->philo_struct[i].state = ALIVE;
	pthread_mutex_init(&data->forks_mutex[i], NULL);
}

//initialize all the mutexes
void	init_singles_mutex(t_data *data)
{
	pthread_mutex_init(&(data->print_mutex), NULL);
	pthread_mutex_init(&(data->full_mutex), NULL);
	pthread_mutex_init(&(data->count_full), NULL);
	pthread_mutex_init(&data->dead_body, NULL);
	pthread_mutex_init(&data->really_dead, NULL);
	pthread_mutex_init(&data->last_meal_mutex, NULL);
}

//function with the loop to initialize all the philo struct
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
