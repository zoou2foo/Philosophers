/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/28 11:30:36 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FOUR FUNCTIONS			*/
//function to print_messages
void	print_message(t_philo *philo, char *str)
{
	if (is_dead(philo) == false) //still needs to check if anybody is dead
	{
		pthread_mutex_lock(&philo->data->print_mutex); //lock mutex to print
		printf("%ld - Philo %d %s\n", time_stamp() - philo->data->start_time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print_mutex); //unlock mutex to print
	}
	// else if (is_dead(philo) == true && philo->data->someone_is_dead == 1) //to print the dead message
	// {
	// 	pthread_mutex_lock(&philo->data->print_mutex);
	// 	printf("%ld - Philo %d %s\n", time_stamp() - philo->data->start_time, philo->id, str);
	// 	pthread_mutex_unlock(&philo->data->print_mutex);
	// }
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		//think(philo); // peut-etre problematique a cause de ce que je peux recevoir en parametres
		usleep(15000);
	while (1) //possible data_race -> Solution: boucle infini, if (is_dead == false), dead_body mutex before if and unlock after
	{
		//pthread_mutex_lock(&philo->data->dead_body); //if I have the mutex here; only one thread can do the routine
		if (is_dead(philo) == false)
		{
			take_first_fork(philo); //in the function; check again if alive or dead ->mutex in to lock fork; send to print_message (mutex pour print)
			take_second_fork(philo); //in the function; check again if alive or dead
			eat(philo); //in the function; check again if alive or dead ->mutex eat
			time_to_sleep(philo); //in the function; check again if alive or dead
			print_message(philo, "is thinking");
			//think(philo); //in the function; check again if alive or dead ->fin tuer les philos.
		}
		else
		{
			//pthread_mutex_lock(&philo->data->print_mutex);
			break ;
		}
		//pthread_mutex_unlock(&philo->data->dead_body);
	}
	//stop_simulation(philo);
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

	init_singles_mutex(data);
	init_philo_mutex(data);
	i = 0;
	data->start_time = time_stamp();//get the start time of the simulation
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
	wait_for_threads(data); //pthread_join: ils ne seront pas join tant qu'ils n'ont pas fini leur routine() (thread function qui est leur job)
	// if (data->someone_is_dead == 1) //mutex avant et apres pour eviter data race; possible
	// 	exit_simulation(data);
	//6th arg to check LATER
	// if (data->nb_to_eat)
	// 	wait_for_full(data);
	// pthread_mutex_destroy(&data->print_mutex);
	// pthread_mutex_destroy(&data->full_mutex);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)