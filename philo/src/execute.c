/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/24 09:56:10 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//function to print_messages
void	print_message(t_philo *philo, char *str)
{
	if (is_dead(philo) == false) //still needs to check if anybody is dead
	{
		pthread_mutex_lock(&philo->data->print_mutex); //lock mutex to print
		printf("%ld - Philo %d %s\n", time_stamp() - philo->data->start_time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print_mutex); //unlock mutex to print
	}
	else if (is_dead(philo) == true) //to print the dead message
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld - Philo %d %s\n", time_stamp() - philo->data->start_time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (is_dead(philo) == false && (philo->data->someone_is_dead == 0) && (philo->data->flag_dead == 0)) //!dead_or_not
	{
		take_first_fork(philo); //in the function; check again if alive or dead ->mutex in to lock fork; send to print_message (mutex pour print)
		take_second_fork(philo); //in the function; check again if alive or dead
		eat(philo); //in the function; check again if alive or dead ->mutex eat
		time_to_sleep(philo); //in the function; check again if alive or dead
		think(philo); //in the function; check again if alive or dead ->fin tuer les philos.
	}
	if (philo->data->flag_dead == 1) //flag to stop them from all printing that they are dead
		return (NULL);
	stop_simulation(philo);
	return (NULL);
}

//function to join back threads
void	wait_for_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)//loop to join(wait) each thread
	{
		if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
			return ; //function to return
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
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
	//6th arg to check LATER
	// if (data->nb_to_eat)
	// 	wait_for_full(data);
	wait_for_threads(data); //pthread_join: ils ne seront pas join tant qu'ils n'ont pas fini leur routine() (thread function qui est leur job)
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)