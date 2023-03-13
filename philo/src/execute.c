/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/13 10:42:19 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//If one philo needs to be execute separately as it will have only one fork

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		//only to eat
		pthread_mutex_lock(&(philo->forks[philo->id % philo->data->nb_philos]));
		printf("Philo %d has taken a fork\n", philo->id);
		pthread_mutex_lock(&(philo->forks[(philo->id + 1) % philo->data->nb_philos]));
		printf("Philo %d has taken a fork\n", philo->id);
		printf("Philo %d is eating\n", philo->id);
		usleep(philo->data->time_to_eat); //put in a function to modify the state too
		pthread_mutex_unlock(&(philo->forks[philo->id % philo->data->nb_philos]));
		pthread_mutex_unlock(&(philo->forks[(philo->id + 1) % philo->data->nb_philos]));
		printf("Philo %d is sleeping\n", philo->id);
		usleep(philo->data->time_to_sleep);
		//if (philo->data->time_to_eat < philo->data->elapsed_time_ms) //si les philos ont déjà mangé au moins nb_to_eat; temps actuel - l'heure qui a mange(la derniere fois) = res a comparer au time_to_eat
		//	break ;
	}
	return (NULL);
}

void	execute(t_data *data)
{
	int		i;

	i = 0;
	while (i <= data->nb_philos)
	{
		pthread_mutex_init(&data->philo->forks[i], NULL);
		data->philo->id = i;
		//time_stamp(data);
		printf("thread no: %d \n", i);
		if (pthread_create(&data->philo[i].philos, NULL, &routine, (void *)data->philo) != 0)
		{
			printf("%s\n", ERR_THREAD);
		}
		//usleep(1000000); //need to redo another function; "smartsleep"
		printf("looking for the segfault: WHYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY!!!!!!!!!!!!!\n");
		if (pthread_join(data->philo[i].philos, NULL) != 0)
			exit (1); //function to return
		i++;
	}
}
