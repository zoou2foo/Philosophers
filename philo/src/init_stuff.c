/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:30:49 by vjean             #+#    #+#             */
/*   Updated: 2023/04/15 15:21:13 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FIVE FUNCTIONS		*/

//to initialize the data struct
void	setup_datastruct(t_data *data, char **str)
{
	data->nb_philos = ft_atoi(str[1]);
	data->time_to_die = ft_atoi(str[2]);
	data->time_to_eat = ft_atoi(str[3]);
	data->time_to_sleep = ft_atoi(str[4]);
	data->status = ALIVE;
	data->nb_full_philos = 0;
}

//initialize each philo struct
void	init_philo(char **av, t_data *data, int i)
{
	if (av[5] == NULL)
		data->philo_struct[i].nb_to_eat = -1;
	else
		data->philo_struct[i].nb_to_eat = ft_atoi(av[5]);
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
	pthread_mutex_init(&(data->count_full), NULL);
	pthread_mutex_init(&data->state_mutex, NULL);
	pthread_mutex_init(&data->last_meal_mutex, NULL);
}

//function with the loop to initialize all the philo struct
void	init_philo_mutex(char **av, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		init_philo(av, data, i);
		i++;
	}
}

//small function to kill them all (destroy mutexes)
void	kill_mutex(t_data *data)
{
	int	i;

	usleep(500);
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->count_full);
	pthread_mutex_destroy(&data->last_meal_mutex);
}
