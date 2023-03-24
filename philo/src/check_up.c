/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/03/24 12:16:46 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//to check if philo dies OR is there a dead body
bool	is_dead(t_philo *philo)
{
	//pthread_mutex_lock(&philo->data->dead_body); //not sure if the right place to do
	if (((time_stamp() - philo->data->start_time) - philo->last_meal) >= philo->data->time_to_die || (philo->data->someone_is_dead == 1))
	{
		//philo->data->someone_is_dead = 1; // not sure if the right place to do
		//pthread_mutex_unlock(&philo->data->dead_body);
		//should I stop everything here and print dead??
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

void	stop_simulation(t_philo *philo) //to be called in the routine to stop simulation
{
	if (philo->data->flag_dead == 0)
	{
		philo->state = DEAD; //change state
		print_message(philo, "is dead");
		pthread_mutex_lock(&philo->data->dead_body);
		philo->data->flag_dead = 1; //flag to tell others to not print dead message and just stop simulation
		philo->data->someone_is_dead = 1; //to say there is a dead body
		pthread_mutex_unlock(&philo->data->dead_body);
		exit_simulation(philo->data);
	}
}