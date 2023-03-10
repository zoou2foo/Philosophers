/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/10 14:23:05 by vjean            ###   ########.fr       */
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
		pthread_mutex_lock(&philo->forks[philo->id]);
		pthread_mutex_lock(&philo->forks[philo->id + 1]);
		printf("philo %d took forks\n", philo->id);
		usleep(200000); //put in a function to modify the state too
		pthread_mutex_unlock(&philo->forks[philo->id]);
		pthread_mutex_unlock(&philo->forks[philo->id + 1]);
		//if (philo->data->time_to_eat < philo->data->elapsed_time_ms) //si les philos ont déjà mangé au moins nb_to_eat
		//	break ;
	}
	return (NULL);
}

void	execute(t_data *data)
{
	int		i;

	i = 0;
	pthread_mutex_init(&data->philo->forks, NULL);
	while (i <= data->nb_philos)
	{
		data->philo->id = i;
		time_stamp(data);
		printf("thread no: %d ", i);
		if (pthread_create(&data->philo->philos[i], NULL, &routine, (void *)data->philo) != 0)
		{
			printf("%s\n", ERR_THREAD);
		}
		usleep(1000000); //need to redo another function; "smartsleep"
		if (pthread_join(data->philo->philos[i], NULL) != 0)
			exit (1); //function to return
		i++;
	}
}
