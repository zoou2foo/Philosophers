/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/04/12 16:01:34 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FIVE FUNCTIONS			*/

//function to print_messages
void	print_message(t_philo *philo, char *str)
{
	if (is_dead(philo) == false)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld - Philo %d %s\n", time_stamp()
			- philo->data->start_time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
}

//thread function
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(15000);
	pthread_mutex_lock(&philo->data->status_mutex);
	while (philo->data->status == 1) //FIXME datarace
	{
		pthread_mutex_unlock(&philo->data->status_mutex);
		if (is_dead(philo) == false)
		{
			take_first_fork(philo);
			take_second_fork(philo);
			eat(philo);
			time_to_sleep(philo);
			print_message(philo, "is thinking");
		}
		pthread_mutex_lock(&philo->data->status_mutex);
	}
	pthread_mutex_unlock(&philo->data->status_mutex);
	return (NULL);
}

void	lock_n_change(t_data *data, int i)
{
	pthread_mutex_lock(&data->state_mutex);
	data->philo_struct[i].state = DEAD; //FIXME datarace
	pthread_mutex_unlock(&data->state_mutex);
	pthread_mutex_lock(&data->someone_is_dead_mutex);
	data->someone_is_dead = 1; //FIXME datarace
	pthread_mutex_unlock(&data->someone_is_dead_mutex);
	pthread_mutex_lock(&data->status_mutex);
	data->status = 0; //FIXME datarace
	pthread_mutex_unlock(&data->status_mutex);
}

//function to join back threads when they are done their job(thread function)
//need to relock before going back to the condition in the while
void	check_health(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->last_meal_mutex);
		if ((time_stamp() - data->start_time) - data->philo_struct[i].last_meal
			>= data->time_to_die) //FIXED datarace, maybe...
		{
			pthread_mutex_unlock(&data->last_meal_mutex);
			lock_n_change(data, i);
			end_when_dead(data, i);
			return ;
		}
		pthread_mutex_unlock(&data->last_meal_mutex);
		if (data->nb_full_philos == data->nb_philos)
		{
			end_when_full(data);
			return ;
		}
		i++;
		if (i == data->nb_philos)
			i = 0;
	}
}

//starting the simulation
void	execute(char **av, t_data *data)
{
	int	i;

	init_singles_mutex(data);
	init_philo_mutex(av, data);
	i = 0;
	data->start_time = time_stamp();
	while (i < data->nb_philos)
	{
		if (pthread_create(&(data->philo_struct[i].philo_th), NULL,
				&routine, &(data->philo_struct[i])) != 0)
		{
			printf("%s\n", ERR_THREAD);
			return ;
		}
		pthread_detach(data->philo_struct[i].philo_th);
		i++;
	}
	check_health(data);
}

/*
pthread_detach():
The pthread_detach() function marks the thread identified by thread as detached.
When a detached thread terminates, its resources are automatically released back
to the system without the need for another thread to join with the terminated
thread. Attempting to detach an already detached thread results in unspecified
behavior.

*/