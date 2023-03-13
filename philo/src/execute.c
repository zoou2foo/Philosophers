/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/13 16:36:50 by vjean            ###   ########.fr       */
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
		printf("id: %d \n", philo->id); //debug
		//pthread_mutex_lock(&(philo->data->forks[philo->id]));
		printf("Philo %d has taken a fork\n", philo->id);
		//pthread_mutex_lock(&(philo->data->forks[(philo->id + 1) % philo->data->nb_philos]));
		printf("Philo %d has taken a fork\n", philo->id);
		printf("Philo %d is eating\n", philo->id);
		//usleep(philo->data->time_to_eat); //put in a function to modify the state too
		//pthread_mutex_unlock(&(philo->data->forks[philo->id]));
		//pthread_mutex_unlock(&(philo->data->forks[(philo->id + 1) % philo->data->nb_philos]));
		printf("Philo %d is sleeping\n", philo->id);
		//usleep(philo->data->time_to_sleep);
		//if (philo->data->time_to_eat < philo->data->elapsed_time_ms) //si les philos ont déjà mangé au moins nb_to_eat; temps actuel - l'heure qui a mange(la derniere fois) = res a comparer au time_to_eat
		//	break ;
	}
	return (NULL);
}

void	execute(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		//time_stamp(data);
		//printf("thread no: %d \n", i);
		data->philo[i].id = i;
		//printf("ID est : %d\n", data->philo[i].id);
		if (pthread_create(&(data->philo[i].philos), NULL, routine, &(data->philo[i])) != 0)
		{
			printf("%s\n", ERR_THREAD);
		}
		//usleep(1000000); //need to redo another function; "smartsleep"
		//printf("looking for the segfault: WHYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY!!!!!!!!!!!!!\n"); //debug
		i++;
	}
	if (pthread_join(data->philo[i].philos, NULL) != 0)
		return ; //function to return
}
