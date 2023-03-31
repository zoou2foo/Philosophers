/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:17:16 by vjean             #+#    #+#             */
/*   Updated: 2023/03/31 11:43:22 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FIVE FUNCTIONS			*/

//take the first fork; need mutex to lock fork, just before the print_mess
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
//2nd if: to deal with one philo and can't take a second fork
//lock 2nd fork and print both taken a 2nd fork and eating. To reduce time
//before printing the eating message
void	take_second_fork(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		if (philo->id - 1 == (philo->id) % philo->data->nb_philos)
		{
			ms_sleep(philo->data->time_to_die);
			pthread_mutex_lock(&philo->data->really_dead);
			philo->data->someone_is_dead = 1;
			pthread_mutex_unlock(&philo->data->really_dead);
			stop_simulation(philo);
			return;
		}
		pthread_mutex_lock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
		print_message(philo, "has taken a 2nd fork");
		print_message(philo, "is eating");
	}
}

//then time to eat: setup the time to eat; to keep track of last_meal
//keep track of number of times that they have eaten
void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->last_meal_mutex);
	philo->last_meal = time_stamp() - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->last_meal_mutex);
	ms_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
	pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
	pthread_mutex_lock(&philo->data->full_mutex);
	philo->nb_meals_enjoyed++;
	pthread_mutex_unlock(&philo->data->full_mutex);
	if (philo->nb_meals_enjoyed == philo->data->nb_to_eat)
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
		philo->state = SLEEPING;
		print_message(philo, "is sleeping");
		if ((philo->data->time_to_eat + philo->data->time_to_sleep) > philo->data->time_to_die)
		{
			ms_sleep(philo->data->time_to_die - philo->data->time_to_eat);
			pthread_mutex_lock(&philo->data->really_dead);
			philo->state = DEAD;
			pthread_mutex_unlock(&philo->data->really_dead);
		}
		else
			ms_sleep(philo->data->time_to_sleep);
	}
}

//time to think
void	think(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		philo->state = THINKING;
		print_message(philo, "is thinking");
	}
}