/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/17 14:27:48 by vjean            ###   ########.fr       */
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
		printf("%ld - Philo %d is thinking\n", time_stamp() - philo->data->start_time, philo->id);
	// else if (flag == 5)
	// 	printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

//maybe no need int i
bool	check_if_philo_dead(t_data *data, int i) //ne pas oublier the unlock les mutex quand je break
{
	if (((time_stamp() - data->start_time) - data->philo_struct[i].last_meal) >= data->time_to_die)
	{
		data->philo_struct->state = DEAD;
		return (false); //philo died
	}
	return (true); //not dead
}


void	*routine(void *arg)
{
	t_philo	*philo;
	int		nb_time_ate;

	philo = (t_philo *)arg;
	nb_time_ate = 0;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1) //philo->state != DEAD; maybe put state in my data struct
	{
		// if (philo->state == DEAD)
		// 	break ;
		// if (((time_stamp() - philo->data->start_time) - philo->last_meal) >= philo->data->time_to_die)
		// {
		// 	philo->state = DEAD;
		// 	print_message(philo, 5);
		// 	return (NULL);
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
		print_message(philo, 4); //think message
		// if (philo->data->nb_to_eat) //need to check where to put it
		// 	nb_time_ate++;
		// if (nb_time_ate == philo->data->nb_to_eat) //need to where to put it
		// 	break ;
		// if time_to_die < current_time -> philo DIED
		//	print_message -> philo id died. -> break
		//	another if to check: // Check if a philo died et si les philos ont déjà mangé au moins nb_to_eat; temps actuel - l'heure qui a mange(la derniere fois) = res a comparer au time_to_eat
		// 	break ;
	}
	return (NULL);
}

void	init_philo(t_data *data, int i)
{
	data->philo_struct[i].id = i + 1;
	data->philo_struct[i].data = data;
	pthread_mutex_init(&data->forks_mutex[i], NULL);
}

int	watching_for_dead(t_data *data) //Not sure that it's working
{
	int	i;

	i = 0;
	while(1)
	{
		if (check_if_philo_dead(data, i) == false)
		{
			//data.philo_struct->state = DEAD; //maybe not at the right place
			//print_mess_dead(&data);
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld - Philo %d is dead\n", time_stamp() - data->start_time, data->philo_struct[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		i += 1 % data->nb_philos;
	}
	return (0);
}

void	execute(t_data *data)
{
	int		i;

	i = 0;
	data->start_time = time_stamp();//get the start time of the simulation
	pthread_mutex_init(&data->print_mutex, NULL); //init my mutex to print_mess
	while (i < data->nb_philos) //loop to initialize mutex for the forks
	{
		init_philo(data, i);  //initialise les mutex AVANT de faire la boucle pour les threads
		i++;
	}
	i = 0; //reset index
	while (i < data->nb_philos) //loop to create the threads --  TRYING to fix dead mess: && (time_stamp() - data->start_time < data->time_to_death)
	{
		if (pthread_create(&(data->philo_struct[i].philo_th), NULL, routine, &(data->philo_struct[i])) != 0) //sending the thread, NULL, thread function and the struct for each philo
		{
			printf("%s\n", ERR_THREAD);
			return ;
		}
		usleep(100); //to give time for each philo to take a fork
		i++;
	}
	if (watching_for_dead(data) == 1)//put the fucking function to check if dead
		return ; //I don't think I'm calling it at the right time
	i = 0;
	while (i < data->nb_philos)//loop to join(wait) each thread  -- TRYING to figure out the dead mess: data->philo_struct[i].state != DEAD
	{
		if (pthread_join(data->philo_struct[i].philo_th, NULL) != 0)
			return ; //function to return

		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
}

//compiler avec fsanitize pour voir data race =thread. ou =address (a verifier)