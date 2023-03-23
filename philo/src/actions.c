/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:17:16 by vjean             #+#    #+#             */
/*   Updated: 2023/03/23 18:00:52 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//take the first fork;
void	take_first_fork(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[philo->id - 1]);
		print_message(philo, "has taken a fork"); //send a str; don't forget mutex_lock print_mutex
	}
}

//take second fork
void	take_second_fork(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
		print_message(philo, "has taken a  2nd fork"); //send a str; don't forget mutex_lock print_mutex
	}
}

//then time to eat
void	eat(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		print_message(philo, "is eating");
		philo->nb_meals_enjoyed++;
		pthread_mutex_lock(&philo->data->last_meal_mutex);
		philo->last_meal = time_stamp() - philo->data->start_time;
		pthread_mutex_unlock(&philo->data->last_meal_mutex);
		if (philo->data->time_to_die < philo->data->time_to_eat) // to calculate the time to eat: here it will die before ending his meal
			ms_sleep(philo->data->time_to_die); //then eat until dies
		else //may not need this shit (if... else)
			ms_sleep(philo->data->time_to_eat); //else eat for the time_to_eat determined
		pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
		pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
	}
}

//putting the philo to sleep
void	time_to_sleep(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		print_message(philo, "is sleeping");
		ms_sleep(philo->data->time_to_sleep);
	}
}

//time to think
void	think(t_philo *philo)
{
	if (is_dead(philo) == false)
		print_message(philo, "is thinking");
}