/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriejean <valeriejean@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/14 14:54:04 by valeriejean      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//If one philo needs to be execute separately as it will have only one fork

void	*routine(void *arg)
{
	t_philo	*philo;
	unsigned long long	start_time;

	philo = (t_philo *)arg;
	start_time = time_stamp();
	while (1)
	{
		//only to eat
		//printf("id: %d \n", philo->id); //debug
		//time_stamp(philo->data);
		pthread_mutex_lock(&(philo->data->forks_mutex[philo->id]));
		printf("%lld - Philo %d has taken a fork\n", time_stamp() - start_time, philo->id);
		pthread_mutex_lock(&(philo->data->forks_mutex[(philo->id + 1) % philo->data->nb_philos]));
		printf("%lld - Philo %d has taken a fork\n", time_stamp() - start_time, philo->id);
		printf("%lld - Philo %d is eating\n", time_stamp() - start_time, philo->id);
		usleep(philo->data->time_to_eat); //put in a function to modify the state too
		pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id]));
		pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id + 1) % philo->data->nb_philos]));
		printf("%lld - Philo %d is sleeping\n", time_stamp() - start_time, philo->id);
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
	while (i++ < data->nb_philos)
	{
		pthread_mutex_init(&data->forks_mutex[i], NULL);
		//time_stamp(data); //probably here to start timer
		data->philo_struct[i].id = i;
		if (pthread_create(&(data->philo_struct[i].philo_th), NULL, routine, &(data->philo_struct[i])) != 0)
		{
			printf("%s\n", ERR_THREAD);
			return ;
		}
		usleep(1000000); //need to redo another function; "smartsleep"
	}
	i = 0;
	while (i++ < data->nb_philos)
	{
		if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
			return ; //function to return
	}
	pthread_mutex_destroy(&data->forks_mutex[i]);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)