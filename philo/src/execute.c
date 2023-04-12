/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/04/12 13:52:13 by vjean            ###   ########.fr       */
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

bool	check_last_meal(t_philo *philo)
{
	if (time_stamp() - philo->last_meal >= philo->data->time_to_die)
		return (true);
	return (false);
}

//thread function
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;

	if (philo->id % 2 == 0)
		usleep(50);
	pthread_mutex_lock(&philo->data->status_mutex);
	while (philo->data->status == 1)
	{
		// if (check_last_meal(philo) == true)
		// {
		// 	pthread_mutex_lock(&philo->data->state_mutex);
		// 	philo->state = DEAD;
		// 	pthread_mutex_unlock(&philo->data->state_mutex);
		// 	break ;
		// }
		if (is_dead(philo) == false)
		{
			pthread_mutex_unlock(&philo->data->status_mutex);
			// if (time_or_no_time(philo))
			// {
			take_first_fork(philo);
			take_second_fork(philo);
			eat(philo);
			// }
			time_to_sleep(philo);
			// if (philo->data->time_to_eat + philo->data->time_to_sleep >= philo->data->time_to_die)
			// {
			// 	//printf("resultat du calcul pour ms_sleep pour philo %d: %ld\n", philo->id, ((time_stamp() + philo->data->time_to_die) - time_stamp()) - (time_stamp() - philo->data->start_time));
			// 	ms_sleep(((time_stamp() + philo->data->time_to_die) - time_stamp()) - (time_stamp() - philo->data->start_time));
			// 	pthread_mutex_lock(&philo->data->state_mutex);
			// 	philo->state = DEAD;
			// 	pthread_mutex_unlock(&philo->data->state_mutex);
			// }
			// else
			print_message(philo, "is thinking");
			
		}
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
		i = 0;
		pthread_mutex_lock(&data->count_full);
		pthread_mutex_lock(&data->someone_is_dead_mutex);
	}
	if (data->nb_full_philos == data->nb_philos && data->someone_is_dead != 1)
		end_when_full(data);
	// else if (data->someone_is_dead == 1)
	// 	end_when_dead(data, i);
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
	wait_for_threads(data); //COMMENT rename to something more specific
}

//after lunch trying new strategy

/*
pthread_detach():
The pthread_detach() function marks the thread identified by thread as detached.
When a detached thread terminates, its resources are automatically released back
to the system without the need for another thread to join with the terminated
thread. Attempting to detach an already detached thread results in unspecified
behavior.

*/