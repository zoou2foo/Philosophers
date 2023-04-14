/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/04/14 11:10:19 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*			FOUR FUNCTIONS			*/

//to check if philo dies
//remplacer if condition par juste philo->data->status tout court, anyway c'est
//l'externe qui s'occupe de le faire mourir. Alors, pourquoi pas réutiliser
//cette variable. Puis, ça enlève des mutexes.
int	is_dead(t_philo *philo)
{
	int ret;

	pthread_mutex_lock(&philo->data->state_mutex);
	ret = philo->state;
	pthread_mutex_unlock(&philo->data->state_mutex);
	return (ret);
	// pthread_mutex_lock(&philo->data->state_mutex);
	// if (philo->state == DEAD) //FIXME datarace; with 4 310 200 100 and 5 510 250 250; sometimes when only one too; once in a while
	// {
	// 	pthread_mutex_unlock(&philo->data->state_mutex);
	// 	// pthread_mutex_lock(&philo->data->someone_is_dead_mutex);
	// 	// philo->data->someone_is_dead = 1; //FIXED datarace
	// 	// pthread_mutex_unlock(&philo->data->someone_is_dead_mutex);
	// 	return (true);
	// }
	// pthread_mutex_unlock(&philo->data->state_mutex);
	// return (false);
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
	// pthread_mutex_destroy(&data->status_mutex);
	pthread_mutex_destroy(&data->count_full);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

void	end_when_dead(t_data *data, int i)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld - Philo %d is dead\n", time_stamp()
		- data->start_time, data->philo_struct[i].id);
	usleep(500);
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->someone_is_dead_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	// pthread_mutex_destroy(&data->status_mutex);
	pthread_mutex_destroy(&data->count_full);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}
