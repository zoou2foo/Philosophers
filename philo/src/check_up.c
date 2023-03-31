/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/03/31 13:51:24 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*			THREE FUNCTIONS			*/

//to check if philo dies OR there is a dead body
//if condition: to make sure that only ONE philo gets in and change the var
//else ->if condition: to check if there is another philo died
bool	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_body);
	if (philo->state == DEAD && philo->data->someone_is_dead == 0)
	{
		pthread_mutex_lock(&philo->data->really_dead);
		philo->data->someone_is_dead = 1;
		pthread_mutex_unlock(&philo->data->really_dead);
		pthread_mutex_unlock(&philo->data->dead_body);
		stop_simulation(philo);
		return (true);
	}
	else
	{
		pthread_mutex_lock(&philo->data->really_dead);
		if (philo->data->someone_is_dead == 1)
		{
			pthread_mutex_unlock(&philo->data->really_dead);
			pthread_mutex_unlock(&philo->data->dead_body);
			return (true);
		}
		pthread_mutex_unlock(&philo->data->really_dead);
		pthread_mutex_unlock(&philo->data->dead_body);
	}
	pthread_mutex_unlock(&philo->data->dead_body);
	return (false);
}

//function to unlock any forks_mutex left and destroy any mutex
//before quitting the program
void	exit_simulation(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_unlock(&data->full_mutex);
	while (i < data->nb_philos)
	{
		pthread_mutex_unlock(&data->forks_mutex[i]);
		i++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->dead_body);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

//function to start the process to stop the simulation
void	stop_simulation(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld - Philo %d is dead\n", time_stamp()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	exit_simulation(philo->data);
}
