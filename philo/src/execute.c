/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/04/03 15:22:53 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FOUR FUNCTIONS			*/

//function to print_messages
void	print_message(t_philo *philo, char *str)
{
	if (is_dead(philo) == false)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld - Philo %d %s\n", time_stamp()
			- philo->data->start_time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
}

//thread function
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(15000);
	pthread_mutex_lock(&philo->data->status_mutex);
	while (philo->data->status == 1) //status on
	{
		pthread_mutex_unlock(&philo->data->status_mutex);
		take_first_fork(philo);
		take_second_fork(philo);
		eat(philo);
		time_to_sleep(philo);
		print_message(philo, "is thinking");

	}
	pthread_mutex_unlock(&philo->data->status_mutex);
	return (NULL);
}

//function to join back threads
void	wait_for_threads(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->someone_is_dead_mutex); //does not work to fix datarace
	while (data->someone_is_dead != 1)//variable a creer dans data et creer un mutex //FIXME data race
	{
		pthread_mutex_unlock(&data->someone_is_dead_mutex); //does not work to fix datarace
		while(i < data->nb_philos)
		{
			pthread_mutex_lock(&data->state_mutex);
			if (data->philo_struct[i].state == DEAD)  //si un meurt; mutex lock print; mutex lock fork //FIXED data race
			{
				pthread_mutex_unlock(&data->state_mutex);
				pthread_mutex_lock(&data->status_mutex);
				data->status = 0;
				pthread_mutex_unlock(&data->status_mutex);
				pthread_mutex_lock(&data->print_mutex);
			}
			pthread_mutex_unlock(&data->state_mutex);
			i++;
		}
		usleep(1000); //might need to adjust
		i = 0;
		//pthread_mutex_unlock(&data->dead_body);
	}
	//pthread_mutex_unlock(&data->someone_is_dead_mutex); //does not work to fix datarace
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld - Philo %d is dead\n", time_stamp()
		- data->start_time, data->philo_struct[i].id);
	pthread_mutex_unlock(&data->print_mutex);
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->someone_is_dead_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->status_mutex);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->full_mutex);
}

//starting the simulation
void	execute(char **av, t_data *data)
{
	int	i;

	init_singles_mutex(data);
	init_philo_mutex(av, data);
	i = 0;
	data->start_time = time_stamp();
	while (i < data->nb_philos)
	{
		if (pthread_create(&(data->philo_struct[i].philo_th), NULL,
				&routine, &(data->philo_struct[i])) != 0)
		{
			printf("%s\n", ERR_THREAD);
			return ;
		}
		pthread_detach(data->philo_struct[i].philo_th);
		i++;
	}
	wait_for_threads(data);
}


// while (i < data->nb_philos)
	// {
	// 	if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
	// 		return ;
	// 	pthread_mutex_destroy(&data->forks_mutex[i]);
	// 	i++;
	// }

// void routine()
// pthread_mutex_lock(&philo->data->full_mutex);
// 		if (is_dead(philo) == false
// 			&& (philo->data->nb_full_philos != philo->data->nb_philos))
// 		{
// 			pthread_mutex_unlock(&philo->data->full_mutex);
// 		}
// 		else
// 		{
// 			pthread_mutex_unlock(&philo->data->full_mutex);
// 			break ;
// 		}