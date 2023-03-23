/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/03/23 18:00:26 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//to check if philo dies OR is there a dead body
bool	is_dead(t_philo *philo)
{
	//might need to add pthread_mutex_lock(&philo->data->dead_body)
	if (((time_stamp() - philo->data->start_time) - philo->last_meal) >= philo->data->time_to_die || (philo->data->someone_is_dead == 1))
	{
		//might need to add pthread_mutex_unlock(&philo->data->dead_body)
		//should I stop everything here and print dead??
		return (true);
	}
	//might need to add pthread_mutex_unlock(&philo->data->dead_body)
	return(false);
}
