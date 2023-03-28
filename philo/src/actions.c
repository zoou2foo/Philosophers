/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:17:16 by vjean             #+#    #+#             */
/*   Updated: 2023/03/28 11:57:23 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FIVE FUNCTIONS			*/

//take the first fork;
void	take_first_fork(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		// if (philo->id - 1 < 1) //to get the first philo NO NEED
		// 	philo->id = philo->data->nb_philos; //to refer to his neighbor NO NEED
		pthread_mutex_lock(&philo->data->forks_mutex[philo->id - 1]); //philo 1 takes fork_mutex 0 and so on...
		print_message(philo, "has taken a fork"); //send a str; don't forget mutex_lock print_mutex
	}
	// else
	// 	pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]); //utile encore ou pas?? Regle pas le drift
}

//take second fork
void	take_second_fork(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		if (philo->id - 1 == (philo->id) % philo->data->nb_philos) {
			ms_sleep(philo->data->time_to_die);
			philo->data->someone_is_dead = 1;
			stop_simulation(philo);
			return;
		}
		pthread_mutex_lock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
		print_message(philo, "has taken a 2nd fork"); //send a str; don't forget mutex_lock print_mutex
		print_message(philo, "is eating");
	}
	// else
	// {
	// 	pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]);
	// 	pthread_mutex_unlock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
	// }
}

//then time to eat
void	eat(t_philo *philo)
{
	philo->state = EATING; //superflu, parce qu'on ne sait pas lequel; il faudrait un tableau meme chose pour tous les state
//	print_message(philo, "is eating");
	pthread_mutex_lock(&philo->data->last_meal_mutex); //seulement un philo a la fois va lire la variable; juste lui qui va lire sa propre variable
	philo->last_meal = time_stamp() - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->last_meal_mutex);
	
	if (philo->data->time_to_die < philo->data->time_to_eat) // to calculate the time to eat: here it will die before ending his meal
		ms_sleep(philo->data->time_to_die); //then eat until dies
	else //may not need this shit (if... else)
		ms_sleep(philo->data->time_to_eat); //else eat for the time_to_eat determined
	//ms_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
	pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
	pthread_mutex_lock(&philo->data->full_mutex);
	philo->nb_meals_enjoyed++;
	pthread_mutex_unlock(&philo->data->full_mutex);
}

//putting the philo to sleep
void	time_to_sleep(t_philo *philo)
{
	if (is_dead(philo) == false)
	{
		philo->state = SLEEPING;
		print_message(philo, "is sleeping");
		if ((philo->data->time_to_eat + philo->data->time_to_sleep) > philo->data->time_to_die)
		{
			ms_sleep(philo->data->time_to_die - philo->data->time_to_eat);
			philo->state = DEAD;
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
		//ms_sleep(philo->data->time_to_eat / 2);
	}
}