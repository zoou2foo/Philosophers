/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/04/15 15:18:42 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		FIVE FUNCTIONS			*/

//function to print_messages
void	print_message(t_philo *philo, char *str)
{
	if (is_dead(philo) != 1)
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
	usleep(15000);
	if (philo->id % 2 == 0)
		usleep(15000);
	while (is_dead(philo) != 1 && is_full(philo) != 1)
	{
		take_first_fork(philo);
		take_second_fork(philo);
		eat(philo);
		time_to_sleep(philo);
		print_message(philo, "is thinking");
	}
	return (NULL);
}

//in chech_health, if one philo is dead, change right away the status of
//the simulation
void	lock_n_change(t_data *data, int i)
{
	(void)i;
	pthread_mutex_lock(&data->state_mutex);
	data->status = DEAD;
	pthread_mutex_unlock(&data->state_mutex);
}

//joining threads when they are done their job
void	wait_thread(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
			return ;
		i++;
	}
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
		i++;
	}
	check_health(data);
	wait_thread(data);
	kill_mutex(data);
}
