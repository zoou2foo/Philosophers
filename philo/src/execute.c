/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/23 16:53:53 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while(is_dead(philo) == false) //!dead_or_not
	{
		take_first_fork(philo); //in the function; check again if alive or dead ->mutex in to lock fork; send to print_message (mutex pour print)
		take_second_fork(); //in the function; check again if alive or dead
		eat(); //in the function; check again if alive or dead ->mutex eat
		time_to_sleep(); //in the function; check again if alive or dead
		think(); //in the function; check again if alive or dead ->fin tuer les philos.
	}
	return (NULL);
}

void	execute(t_data *data)
{
	int		i;

	data->start_time = time_stamp();//get the start time of the simulation
	init_singles_mutex(data);
	init_philo_mutex(data);
	i = 0;
	while (i < data->nb_philos) //loop to create the threads
	{
		if (pthread_create(&(data->philo_struct[i].philo_th), NULL, &routine, &(data->philo_struct[i])) != 0) //sending the thread, NULL, thread function and the struct for each philo
		{
			printf("%s\n", ERR_THREAD);
			return ;
		}
		//usleep(100); //to give time for each philo to take a fork
		i++;
	}
	if (data->nb_to_eat)
		wait_for_full(data);
	wait_for_threads(data); //pthread_join: ils ne seront pas join tant qu'ils n'ont pas fini leur routine() (thread function qui est leur job)
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)