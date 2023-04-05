/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/04/05 12:08:51 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*			FOUR FUNCTIONS			*/

//to check if philo dies
bool	is_dead(t_philo *philo)
{
	time_or_no_time(philo);
	pthread_mutex_lock(&philo->data->state_mutex); //mutex superflu
	if (philo->state == DEAD)
	{
		pthread_mutex_lock(&philo->data->someone_is_dead_mutex);
		philo->data->someone_is_dead = 1;
		pthread_mutex_unlock(&philo->data->someone_is_dead_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->data->state_mutex); //mutex superflu
	return (false);
}

void	end_when_full(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->print_mutex);
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->someone_is_dead_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->status_mutex);
	pthread_mutex_destroy(&data->count_full);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

void	end_when_dead(t_data *data, int i)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld - Philo %d is dead\n", time_stamp()
		- data->start_time, data->philo_struct[i].id);
	pthread_mutex_unlock(&data->print_mutex);
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->someone_is_dead_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->status_mutex);
	pthread_mutex_destroy(&data->count_full);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

void	loop_check_state(t_data *data, int i)
{
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->state_mutex);
		if (data->philo_struct[i].state == DEAD)
		{
			pthread_mutex_unlock(&data->state_mutex);
			pthread_mutex_lock(&data->status_mutex);
			data->status = 0;
			pthread_mutex_unlock(&data->status_mutex);
			pthread_mutex_lock(&data->print_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->state_mutex);
		i++;
	}
}
