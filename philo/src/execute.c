/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/16 12:54:20 by vjean            ###   ########.fr       */
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
	{
		printf("%ld - Philo %d is eating\n", time_stamp() - philo->data->start_time, philo->id);
		philo->last_meal = time_stamp() - philo->data->start_time;
	}
	else if (flag == 3)
		printf("%ld - Philo %d is sleeping\n", time_stamp() - philo->data->start_time, philo->id);
	else if (flag == 4)
		printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

// bool	check_if_philo_dead(t_philo *philo)
// {
// 	if (time_stamp() - philo->last_meal > philo->data->time_to_eat)
// 		return (false); //philo died
// 	return (true); //not dead
// }

void	*routine(void *arg)
{
	t_philo	*philo;
	int		nb_time_ate;
	int		is_dead;

	philo = (t_philo *)arg;
	nb_time_ate = 0;
	is_dead = 0;
	//mettre if impair dort (temps de manger) si pair run; dernier dort 2 fois si impair(temps de manger)
	while (!is_dead) //ou mettre un flag tant que no death => !is_dead (setup at 0)
	{
		// if (check_if_philo_dead(philo) == false)  //does not work
		// {
		// 	print_message(philo, 4);
		// 	break ;
		// }
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
			nb_time_ate++;
		if (nb_time_ate == philo->data->nb_to_eat)
			break ;
		// if time_to_death < current_time -> philo DIED
		//	print_message -> philo id died. -> break
		//	another if to check: // Check if a philo died et si les philos ont déjà mangé au moins nb_to_eat; temps actuel - l'heure qui a mange(la derniere fois) = res a comparer au time_to_eat
		// 	break ;
	} // when out of the loop, set is_dead = 1
	is_dead = 1;
	print_message(philo, 4);
	return (NULL);
}

void	init_philo(t_data *data, int i)
{
	data->philo_struct[i].id = i + 1;
	data->philo_struct[i].data = data;
	pthread_mutex_init(&data->forks_mutex[i], NULL);
}


void	execute(t_data *data)
{
	int		i;

	i = 0;
	data->start_time = time_stamp();
	pthread_mutex_init(&data->print_mutex, NULL);
	while (i < data->nb_philos)
	{
		init_philo(data, i);  //initialise les mutex AVANT de faire la boucle pour les threads
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