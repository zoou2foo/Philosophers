/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/03/24 16:49:08 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//to check if philo dies OR is there a dead body
bool	is_dead(t_philo *philo)
{
	//pthread_mutex_lock(&philo->data->dead_body); //not sure if the right place to do
	if (((time_stamp() - philo->data->start_time) - philo->last_meal) > philo->data->time_to_die)
	{
		//philo->data->someone_is_dead = 1; // not sure if the right place to do
		//pthread_mutex_unlock(&philo->data->dead_body);
		//should I stop everything here and print dead??
		//pthread_mutex_lock(&philo->data->dead_body);
		philo->data->someone_is_dead = 1;
		//pthread_mutex_unlock(&philo->data->dead_body);
		return (true);
	}
	//might need to add pthread_mutex_unlock(&philo->data->dead_body)
	return(false);
}

void	exit_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_join(data->philo_struct[i].philo_th, NULL);
		i++;
	}
	i = 0;
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

//watcher_th lock tout les mutex pour empecher les autres; BLOQUER celui de MESSAGE en priorite
void	stop_simulation(t_philo *philo) //to be called in the routine to stop simulation
{
	// pthread_mutex_lock(&philo->data->dead_body);
	// philo->data->someone_is_dead = 1; //to say there is a dead body
	// pthread_mutex_unlock(&philo->data->dead_body);
	philo->state = DEAD; //change state; superflu
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	exit_simulation(philo->data);
}