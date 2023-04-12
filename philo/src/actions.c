/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:17:16 by vjean             #+#    #+#             */
/*   Updated: 2023/04/12 16:27:11 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FOUR FUNCTIONS			*/

//take the first fork
//philo 1 takes fork_mutex 0 and so on...
void	take_first_fork(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[philo->id - 1]);
		print_message(philo, "has taken a fork");
	}
}

//take second fork
//2nd if: in case of one philo and can't take another fork
//print took the 2nd fork and eating right away. To reduce drift and time
//between when philo can eat and the message printed.
void	take_second_fork(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		if (philo->id - 1 == (philo->id) % philo->data->nb_philos)
		{
			ms_sleep(philo->data->time_to_die);
			pthread_mutex_lock(&philo->data->someone_is_dead_mutex);
			philo->data->someone_is_dead = 1; //FIXME datarace when only one philo
			pthread_mutex_unlock(&philo->data->someone_is_dead_mutex);
			pthread_mutex_lock(&philo->data->state_mutex);
			philo->state = DEAD; //FIXME datarace when only one philo
			pthread_mutex_unlock(&philo->data->state_mutex);
			return ;
		}
		pthread_mutex_lock(&philo->data->forks_mutex[(philo->id)
			% philo->data->nb_philos]);
		print_message(philo, "has taken a 2nd fork");
		print_message(philo, "is eating");
		pthread_mutex_lock(&philo->data->last_meal_mutex);
		philo->last_meal = time_stamp() - philo->data->start_time; //FIXED datarace
		pthread_mutex_unlock(&philo->data->last_meal_mutex);
		ms_sleep(philo->data->time_to_eat);
	}
}

//then time to eat takes care of setting up the time to eat
//keep track of last_meal and counts the number of time that they have eaten
void	eat(t_philo *philo)
{
	pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
	pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id)
			% philo->data->nb_philos]));
	philo->nb_meals_enjoyed++;
	if (philo->nb_meals_enjoyed == philo->nb_to_eat)
	{
		pthread_mutex_lock(&philo->data->count_full);
		philo->data->nb_full_philos += 1;
		pthread_mutex_unlock(&philo->data->count_full);
	}
}

//putting the philo to sleep
//need to check if philo will die during his sleep
void	time_to_sleep(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		print_message(philo, "is sleeping");
		ms_sleep(philo->data->time_to_sleep);
	}
}
