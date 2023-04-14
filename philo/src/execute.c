/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/04/14 15:37:18 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FIVE FUNCTIONS			*/

//function to print_messages
void	print_message(t_philo *philo, char *str)
{
	if (is_dead(philo) != 1)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld - Philo %d %s\n", time_stamp()
			- philo->data->start_time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
}

int	is_full(t_philo *philo)
{
	int full;

	full = 0;
	pthread_mutex_lock(&philo->data->count_full);
	if (philo->data->nb_full_philos == philo->data->nb_philos)
		full = 1;
	pthread_mutex_unlock(&philo->data->count_full);
	return (full);
}

//thread function
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(15000);
	//pthread_mutex_lock(&philo->data->state_mutex);
	while (is_dead(philo) != 1 && is_full(philo) != 1)
	{
		//pthread_mutex_unlock(&philo->data->state_mutex);
		// if (is_dead(philo) != 1)
		// {
		take_first_fork(philo);
		take_second_fork(philo);
		eat(philo);
		time_to_sleep(philo);
		print_message(philo, "is thinking");
		// }
		//pthread_mutex_lock(&philo->data->state_mutex);
	}
	//pthread_mutex_unlock(&philo->data->state_mutex);
	return (NULL);
}

void	lock_n_change(t_data *data, int i)
{
	(void)i;
	pthread_mutex_lock(&data->state_mutex);
	// data->philo_struct[i].state = DEAD;
	data->status = DEAD;
	pthread_mutex_unlock(&data->state_mutex);
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
			>= data->time_to_die)
		{
			pthread_mutex_unlock(&data->last_meal_mutex);
			lock_n_change(data, i);
			end_when_dead(data, i);
			return ;
		}
		pthread_mutex_unlock(&data->last_meal_mutex);
		if (is_full(&data->philo_struct[i]) == 1)
			return ;
		// if (data->nb_full_philos == data->nb_philos)
		// {
		// 	end_when_full(data);
		// 	return ;
		// }
		i++;
		if (i == data->nb_philos)
			i = 0;
	}
}

void	wait_thread(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
			return ;
		i++;
	}
}

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
	// pthread_mutex_destroy(&data->full_mutex);
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
		// pthread_detach(data->philo_struct[i].philo_th);
		i++;
	}
	// i = 0;
	// while (i < data->nb_philos)
	// {
	// 	pthread_detach(data->philo_struct[i].philo_th);
	// 	i++;
	// }
	check_health(data);
	wait_thread(data);
	kill_mutex(data);
}

/*
pthread_detach():
The pthread_detach() function marks the thread identified by thread as detached.
When a detached thread terminates, its resources are automatically released back
to the system without the need for another thread to join with the terminated
thread. Attempting to detach an already detached thread results in unspecified
behavior.

*/