/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:17:16 by vjean             #+#    #+#             */
/*   Updated: 2023/03/22 13:47:19 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* FOUR functions... */

void	print_eating(t_philo *philo)
{
	philo->state = EATING;
	printf("%ld - Philo %d is eating\n", time_stamp() - philo->data->start_time, philo->id);
	philo->nb_meals_enjoyed++;
	if (philo->nb_meals_enjoyed == philo->data->nb_to_eat)
	{
		philo->state = FULL;
		philo->data->nb_full_philos++; // increment the number of full philosophers. DATA RACE
	}
}

int	print_message(t_philo *philo, int flag) //fait trop de choses separes pour function qui s'occupe du state, une autre pour print
{
	//pthread_mutex_lock(&philo->data->dead_body);
	if (philo->data->someone_is_dead == 1) //data race
	{
		//pthread_mutex_unlock(&philo->data->dead_body);
		return (1);
	}
	if (flag == 1)
		printf("%ld - Philo %d has taken a fork\n", time_stamp() - philo->data->start_time, philo->id); //get time(current time - start time) APRES mutex
	else if (flag == 2)
		print_eating(philo);
	else if (flag == 3)
	{
		philo->state = SLEEPING;
		printf("%ld - Philo %d is sleeping\n", time_stamp() - philo->data->start_time, philo->id);
	}
	else if (flag == 4)
	{
		philo->state = THINKING;
		printf("%ld - Philo %d is thinking\n", time_stamp() - philo->data->start_time, philo->id);
	}
	else if (flag == 5)
	{
		philo->state = DEAD;
		printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
		//pthread_mutex_lock(&philo->data->dead_body);
		philo->data->someone_is_dead = 1;
		//pthread_mutex_unlock(&philo->data->dead_body);
	}
	return (0);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->forks_mutex[philo->id - 1]));
	pthread_mutex_lock(&philo->data->print_mutex);
	if (print_message(philo, 1) == 1) //printf("- Philo %d has taken a fork\n", philo->id);
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_lock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
	pthread_mutex_lock(&philo->data->print_mutex);
	if (print_message(philo, 1) == 1)//printf("- Philo %d has taken a fork\n", philo->id celle de son voisin);
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]);
		pthread_mutex_unlock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (print_message(philo, 2) == 1)//printf(" - Philo %d is eating\n", philo->id);
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]);
		pthread_mutex_unlock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	philo->last_meal = time_stamp() - philo->data->start_time;
	if (philo->data->time_to_die < philo->data->time_to_eat)
		ms_sleep(philo->data->time_to_die);
	else
		ms_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
	pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
}

void	go_to_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	print_message(philo, 3);//printf(" - Philo %d is sleeping\n", philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	if (philo->data->time_to_die < philo->data->time_to_eat + philo->data->time_to_sleep) //->sleep time to die
		ms_sleep(philo->data->time_to_die - philo->data->time_to_eat);
	else
	{
		ms_sleep(philo->data->time_to_sleep);
		pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 4); //think message
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
}

