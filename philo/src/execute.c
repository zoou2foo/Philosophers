/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/31 13:52:05 by vjean            ###   ########.fr       */
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
//infinite loop and then, if condition to check if anyone dies or if they have
//all eaten enough to break
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(15000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->full_mutex);
		if (is_dead(philo) == false
			&& (philo->data->nb_full_philos != philo->data->nb_philos))
		{
			pthread_mutex_unlock(&philo->data->full_mutex);
			take_first_fork(philo);
			take_second_fork(philo);
			eat(philo);
			time_to_sleep(philo);
			print_message(philo, "is thinking");
		}
		else
		{
			pthread_mutex_unlock(&philo->data->full_mutex);
			break ;
		}
	}
	return (NULL);
}

//function to join back threads
void	wait_for_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
			return ;
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
}

//starting the simulation
void	execute(t_data *data)
{
	int		i;

	init_singles_mutex(data);
	init_philo_mutex(data);
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
		i++;
	}
	wait_for_threads(data);
}
