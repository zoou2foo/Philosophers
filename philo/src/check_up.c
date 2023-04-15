/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/04/15 15:06:10 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*			FOUR FUNCTIONS			*/

//function to check if all my philo have eaten enough
int	is_full(t_philo *philo)
{
	int	full;

	full = 0;
	pthread_mutex_lock(&philo->data->count_full);
	if (philo->data->nb_full_philos == philo->data->nb_philos)
		full = 1;
	pthread_mutex_unlock(&philo->data->count_full);
	return (full);
}

//to check the state of the philo
int	is_dead(t_philo *philo)
{
	int	ret;

	pthread_mutex_lock(&philo->data->state_mutex);
	ret = philo->data->status;
	pthread_mutex_unlock(&philo->data->state_mutex);
	return (ret);
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
		i++;
		if (i == data->nb_philos)
			i = 0;
	}
}

//function to print the died message
void	end_when_dead(t_data *data, int i)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld - Philo %d is dead\n", time_stamp()
		- data->start_time, data->philo_struct[i].id);
	pthread_mutex_unlock(&data->print_mutex);
}
