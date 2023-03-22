/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/03/22 09:35:50 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	check_if_philo_dead(t_philo *philo) //unlock mutex when I break
{
	if (((time_stamp() - philo->data->start_time) - philo->last_meal) >= philo->data->time_to_die)
	{
		return (false); //philo died
	}
	else if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(&(philo->data->forks_mutex[philo->id - 1]));
		pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 1); //printf("- Philo %d has taken a fork\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		ms_sleep(philo->data->time_to_die);
		return (false); //One philo will die after time_to_die as it doesn't have another fork
	}
	return (true); //not dead
}

bool	check_if_philo_full(t_philo *philo)
{
	if (philo->state == FULL)
	{
		return (true);
	}
	return (false);
}
