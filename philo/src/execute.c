/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/15 15:48:02 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//If one philo needs to be execute separately as it will have only one fork
void	print_message(t_philo *philo, int flag)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (flag == 1)
		printf("%ld - Philo %d has taken a fork\n", time_stamp() - philo->data->start_time, philo->id); //get time(current time - start time) APRES mutex
	else if (flag == 2)
		printf("%ld - Philo %d is eating\n", time_stamp() - philo->data->start_time, philo->id);
	else if (flag == 3)
		printf("%ld - Philo %d is sleeping\n", time_stamp() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	//mettre if impair dort (temps de manger) si pair run; dernier dort 2 fois si impair(temps de manger)
	while (1)
	{
		pthread_mutex_lock(&(philo->data->forks_mutex[philo->id - 1]));
		print_message(philo, 1);
		pthread_mutex_lock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
		print_message(philo, 1);//printf("- Philo %d has taken a fork\n", philo->id);
		print_message(philo, 2);//printf(" - Philo %d is eating\n", philo->id);
		ms_sleep(philo->data->time_to_eat); //put in a function to modify the state too
		pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
		pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
		print_message(philo, 3);//printf(" - Philo %d is sleeping\n", philo->id); //get time(current time - start time)
		ms_sleep(philo->data->time_to_sleep);
		if (philo->data->nb_to_eat)
			philo->data->nb_to_eat++;
		//	another if to check: // Check if a philo died et si les philos ont déjà mangé au moins nb_to_eat; temps actuel - l'heure qui a mange(la derniere fois) = res a comparer au time_to_eat
		// 	break ;
	}
	return (NULL);
}

void	init_philo(t_data *data, int i)
{
	data->philo_struct[i].id = i + 1;
	data->philo_struct[i].data = data;
	pthread_mutex_init(&data->forks_mutex[i], NULL); // i - 1, pour partir a la fourchette zero
}


void	execute(t_data *data)
{
	int		i;

	i = 0;
	data->start_time = time_stamp();
	pthread_mutex_init(&data->print_mutex, NULL);
	while (i < data->nb_philos)
	{
		init_philo(data, i);
		i++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(&(data->philo_struct[i].philo_th), NULL, routine, &(data->philo_struct[i])) != 0)
		{
			printf("%s\n", ERR_THREAD);
			return ;
		}
		usleep(100); //pour donner le temps au premier philo de prendre les fourchettes
		i++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
			return ; //function to return
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)