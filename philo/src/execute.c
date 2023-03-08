/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/08 15:30:47 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//If one philo needs to be execute separately as it will have only one fork

void	*routine(void *arg)
{
	t_data	*data;
	int		i;

	i = 0;
	data = (t_data *)arg;
	while(i < data->nb_philos)
	{
		pthread_mutex_lock(&data->forks);
		printf("philo %d eat\n", i);
		usleep(500000);
		pthread_mutex_unlock(&data->forks);
		i++;
	}
	return (NULL);
}

void	execute(t_data *data)
{
	int		i;
	t_philo	philo;

	i = 1;
	pthread_mutex_init(&data->forks, NULL);
	while (i <= data->nb_philos)
	{
		if (pthread_create(&philo.philos[i], NULL, &routine, (void *)data) != 0)
		{
			printf("%s\n", ERR_THREAD);
		}
		usleep(1000000); //need to redo another function; "smartsleep"
		if (pthread_join(philo.philos[i], NULL) != 0)
			exit (1); //function to return
		i++;
	}
}
