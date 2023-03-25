/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/25 14:38:00 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//function to print_messages
void	print_message(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print_mutex); //lock mutex to print
	printf("%ld - Philo %d %s\n", time_stamp() - philo->data->start_time, philo->id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0) //pour eviter deadlock
		usleep(100);
	while (1) //to avoid data_race: infinite loop and an if condition is_dead. Lock mutex before if and unlock after
	{
		//pthread_mutex_lock(&philo->data->dead_body);
		//pthread_mutex_lock(&philo->data->print_mutex);
		if (is_dead(philo) == true) //we need to add if they are all full
		{
			//pthread_mutex_unlock(&philo->data->dead_body);
			//pthread_mutex_unlock(&philo->data->print_mutex);
			break ;
		}
		//pthread_mutex_unlock(&philo->data->dead_body);
		//pthread_mutex_unlock(&philo->data->print_mutex);
		take_first_fork(philo); //in the function; check again if alive or dead ->mutex in to lock fork; send to print_message (mutex pour print)
		take_second_fork(philo); //in the function; check again if alive or dead
		eat(philo); //in the function; check again if alive or dead ->mutex eat
		time_to_sleep(philo); //in the function; check again if alive or dead
		think(philo); //in the function; check again if alive or dead ->fin tuer les philos.
	}
	//maybe another function to check_if_really dead
	stop_simulation(philo); //en faisant thread_watch, pas besoin
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
			printf("%s\n", ERR_THREAD); //error management
			return ;
		}
		i++;
	}
	//stop_simulation(data); maybe not there to call //FUNCTION to STOP simulation
	//exit_simulation(data); //FUNCTION to destroy all threads: unlock all forks, destroy all forks_mutex and destroy all the remaining mutex
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)