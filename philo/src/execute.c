/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/09 16:57:30 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//If one philo needs to be execute separately as it will have only one fork

void	*routine(void *arg)
{
	t_data	*philo;

	philo = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->forks);
		printf(" eat\n");
		usleep(200000);
		pthread_mutex_unlock(&data->forks);
		if (data->time_to_eat < data->elapsed_time_ms)
			break ;
	}
	return (NULL);
}

void	execute(t_data *data)
{
	int		i;
	t_philo	philo;

	i = 0;
	philo = data->philo;
	pthread_mutex_init(&data->forks, NULL);
	while (i <= data->nb_philos)
	{
		philo.id = i;
		time_stamp(data);
		printf("thread no: %d ", i);
		if (pthread_create(&philo.philos[i], NULL, &routine, (void *)philo) != 0)
		{
			printf("%s\n", ERR_THREAD);
		}
		usleep(1000000); //need to redo another function; "smartsleep"
		if (pthread_join(philo.philos[i], NULL) != 0)
			exit (1); //function to return
		i++;
	}
}
