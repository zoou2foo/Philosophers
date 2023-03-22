/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/22 09:49:04 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*	FOUR functions... */

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (check_if_philo_dead(philo) == false)
		{
			pthread_mutex_lock(&(philo->data->print_mutex));//NEED IT to avoid data race
			print_message(philo, 5);
			pthread_mutex_unlock(&(philo->data->print_mutex)); //NEED IT to avoid data race
			return (NULL); //to terminate thread
		}
		//!!need to add a check si les forks already lock or not.!! <=
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
	while (1)
	{
		pthread_mutex_lock(&data->full_mutex); //wait for all philo to be full
		if (data->nb_full_philos == data->nb_philos) //data race?? because checked in print_message()
		{
			pthread_mutex_unlock(&data->full_mutex);
			break ;
		}
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
		usleep(100); //to give time for each philo to take a fork
		i++;
	}
	wait_for_full(data);
	wait_for_threads(data);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)

// int	check_forks(t_philo *philo)
// {
// 	int	fork1;
// 	int	fork2;
// 	int	first_fork;
// 	int	second_fork;

// 	fork1 = philo->id % philo->data->nb_philos;
// 	fork2 = (philo->id + 1) % philo->data->nb_philos;
// 	first_fork = fork1;
// 	second_fork = fork2;

// 	if (fork2 < fork1) //assign the lower-numbered fork to the first_fork var
// 	{
// 		first_fork = fork2;
// 		second_fork = fork1;
// 	}
// 	if (pthread_mutex_lock(&philo->data->forks_mutex[first_fork]) != 0) //try to pick up the first fork
// 		return (0);
// 	pthread_mutex_lock(&philo->data->print_mutex);
// 	print_message(philo, 1);
// 	pthread_mutex_unlock(&philo->data->print_mutex);
// 	if (pthread_mutex_lock(&philo->data->forks_mutex[second_fork]) != 0) //try to pick up the second fork
// 	{
// 		pthread_mutex_unlock(&philo->data->forks_mutex[first_fork]);
// 		return (0);
// 	}
// 	pthread_mutex_lock(&philo->data->print_mutex);
// 	print_message(philo, 1);
// 	pthread_mutex_unlock(&philo->data->print_mutex);
// 	pthread_mutex_lock(&philo->data->print_mutex);
// 	print_message(philo, 2);//printf(" - Philo %d is eating\n", philo->id);
// 	pthread_mutex_unlock(&philo->data->print_mutex);
// 	philo->last_meal = time_stamp() - philo->data->start_time;
// 	if (philo->data->time_to_die < philo->data->time_to_eat)
// 		ms_sleep(philo->data->time_to_die);
// 	else
// 		ms_sleep(philo->data->time_to_eat);
// 	pthread_mutex_unlock(&(philo->data->forks_mutex[first_fork]));
// 	pthread_mutex_unlock(&(philo->data->forks_mutex[second_fork]));
// 	return (1);
// }