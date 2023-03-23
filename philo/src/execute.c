/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/23 14:18:18 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*	FIVE functions... */

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (check_dead(philo->data) == 0)
	{
		if ((check_dead(philo->data) == 0) && (check_if_philo_dead(philo) == false))
		{
			philo->state = DEAD;
			pthread_mutex_lock(&philo->data->print_mutex);
			printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
			pthread_mutex_unlock(&philo->data->print_mutex);
			pthread_mutex_lock(&philo->data->dead_body);
			philo->data->someone_is_dead = 1;
			pthread_mutex_unlock(&philo->data->dead_body);
			return (NULL); //to terminate thread
		}
		eat(philo);
		go_to_sleep(philo);
	}
	return (NULL);
}

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

void	wait_for_full(t_data *data)
{
	while (check_dead(data) != 1)
	{
		pthread_mutex_lock(&data->full_mutex);
		if (data->nb_full_philos == data->nb_philos)
		{
			pthread_mutex_unlock(&data->full_mutex);
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld - All philosophers have eaten enough\n", time_stamp() - data->start_time);
			pthread_mutex_unlock(&data->print_mutex);
			pthread_mutex_lock(&data->dead_body);
			data->someone_is_dead = 1;
			pthread_mutex_unlock(&data->dead_body);
			break ;
		}
		pthread_mutex_unlock(&data->full_mutex);
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
	if (data->nb_to_eat)
		wait_for_full(data);
	wait_for_threads(data); //pthread_join: ils ne seront pas join tant qu'ils n'ont pas fini leur routine() (thread function qui est leur job)
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->full_mutex);
	pthread_mutex_destroy(&data->dead_body);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)