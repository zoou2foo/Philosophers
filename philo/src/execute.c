/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/04/05 15:50:20 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FOUR FUNCTIONS			*/

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
	pthread_mutex_lock(&philo->data->last_meal_mutex);
	philo->last_meal = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->last_meal_mutex);

	if (philo->id % 2 == 0)
		usleep(50);
	pthread_mutex_lock(&philo->data->status_mutex);
	while (philo->data->status == 1)
	{
		pthread_mutex_unlock(&philo->data->status_mutex);
		// if (time_or_no_time(philo))
		// {
		take_first_fork(philo);
		take_second_fork(philo);
		eat(philo);
		// }
		time_to_sleep(philo);
		print_message(philo, "is thinking");
	}
	pthread_mutex_unlock(&philo->data->status_mutex);
	return (NULL);
}

//function to join back threads when they are done their job(thread function)
//need to relock before going back to the condition in the while
void	wait_for_threads(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->someone_is_dead_mutex);
	pthread_mutex_lock(&data->count_full);
	while (data->someone_is_dead != 1 && (data->nb_full_philos
			!= data->nb_philos))
	{
		pthread_mutex_unlock(&data->someone_is_dead_mutex);
		pthread_mutex_unlock(&data->count_full);
		loop_check_state(data, i);
		usleep(50);
		//i = 0;
		pthread_mutex_lock(&data->count_full);
		pthread_mutex_lock(&data->someone_is_dead_mutex);
	}
	if (data->nb_full_philos == data->nb_philos && data->someone_is_dead != 1)
		end_when_full(data);
	else if (data->someone_is_dead == 1)
		end_when_dead(data, i);
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
	wait_for_threads(data);
}
