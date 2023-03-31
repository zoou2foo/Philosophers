/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 09:35:28 by vjean             #+#    #+#             */
/*   Updated: 2023/03/31 08:57:14 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*			FOUR FUNCTIONS			*/


//to check if philo dies OR is there a dead body
bool	is_dead(t_philo *philo)
{
	//mutex_lock before and after if; doesn't help
	//if ((philo->data->someone_is_dead == 0 && ((time_stamp() - philo->data->start_time) - philo->last_meal) > philo->data->time_to_die) || philo->state == DEAD) //formula to check if philo is dead
	pthread_mutex_lock(&philo->data->dead_body);
	if (philo->state == DEAD && philo->data->someone_is_dead == 0)//pour s'assure que juste ONE philo rentre là et s'affiche.
	{
		//philo->state = DEAD; //change state; superfluous maybe
		//pthread_mutex_lock(&philo->data->dead_body); //lock to change someone_is_dead (in data)
		pthread_mutex_lock(&philo->data->really_dead);
		philo->data->someone_is_dead = 1;
		pthread_mutex_unlock(&philo->data->really_dead);
		pthread_mutex_unlock(&philo->data->dead_body);
		stop_simulation(philo);
		return (true);
	}
	else
	{
		//pthread_mutex_lock(&philo->data->dead_body); //lock to change someone_is_dead (in data)
		//philo->state = DEAD; //change state; superfluous maybe
		pthread_mutex_lock(&philo->data->really_dead);
		if (philo->data->someone_is_dead == 1) //check si un autre philo est mort
		{
			pthread_mutex_unlock(&philo->data->really_dead);
			pthread_mutex_unlock(&philo->data->dead_body);
			return (true);
		}
		pthread_mutex_unlock(&philo->data->really_dead);
		pthread_mutex_unlock(&philo->data->dead_body);
	}
	pthread_mutex_unlock(&philo->data->dead_body);
	return(false);
}

void	exit_simulation(t_data *data)
{
	int	i;

	i = 0;
	//pthread_mutex_unlock(&data->last_meal_mutex);
	pthread_mutex_unlock(&data->full_mutex);
	while (i < data->nb_philos)
	{
		pthread_mutex_unlock(&data->forks_mutex[i]); //to unlock all forks so nobody is holding on a fork
		i++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]); //destroy all the mutex created for the forks
		//pthread_join(data->philo_struct[i].philo_th, NULL); //threads join when they are done their thread function (their job); needs to be after destroy forks mutex otherwise; you lost the threads
		i++;
	}
	// i = 0;
	// while (i < data->nb_philos)
	// {
	// 	i++;
	// }
	pthread_mutex_destroy(&data->print_mutex); //destroy each single mutex
	pthread_mutex_destroy(&data->dead_body);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

void	stop_simulation(t_philo *philo) //to be called in the routine to stop simulation
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	//print_message(philo, "is dead");
	exit_simulation(philo->data);
}