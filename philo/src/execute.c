/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 14:02:50 by vjean             #+#    #+#             */
/*   Updated: 2023/03/21 11:21:29 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//If one philo needs to be execute separately as it will have only one fork
void	print_message(t_philo *philo, int flag) //fait trop de choses separes pour function qui s'occupe du state, une autre pour print
{
	if (philo->data->someone_is_dead == 1)
		return ;
	if (flag == 1)
		printf("%ld - Philo %d has taken a fork\n", time_stamp() - philo->data->start_time, philo->id); //get time(current time - start time) APRES mutex
	else if (flag == 2)
	{
		philo->state = EATING;
		printf("%ld - Philo %d is eating\n", time_stamp() - philo->data->start_time, philo->id);
	}
	else if (flag == 3)
	{
		philo->state = SLEEPING;
		printf("%ld - Philo %d is sleeping\n", time_stamp() - philo->data->start_time, philo->id);
	}
	else if (flag == 4)
	{
		philo->state = THINKING;
		printf("%ld - Philo %d is thinking\n", time_stamp() - philo->data->start_time, philo->id);
	}
	else if (flag == 5)
	{
		philo->state = DEAD;
		printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
		philo->data->someone_is_dead = 1;
	}
}

bool	check_if_philo_dead(t_philo *philo) //unlock mutex when I break
{
	if (((time_stamp() - philo->data->start_time) - philo->last_meal) >= philo->data->time_to_die)
	{
		return (false); //philo died
	}
	else if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(&(philo->data->forks_mutex[philo->id - 1]));
		pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 1); //printf("- Philo %d has taken a fork\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		ms_sleep(philo->data->time_to_die);
		return (false); //One philo will die after time_to_die as it doesn't have another fork
	}
	return (true); //not dead
}



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
		pthread_mutex_lock(&(philo->data->forks_mutex[philo->id - 1]));
		pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 1); //printf("- Philo %d has taken a fork\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_lock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
		pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 1);//printf("- Philo %d has taken a fork\n", philo->id celle de son voisin);
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 2);//printf(" - Philo %d is eating\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		philo->last_meal = time_stamp() - philo->data->start_time;
		philo->nb_meals_enjoyed++;
		// if (philo->nb_meals_enjoyed == philo->data->nb_to_eat)
		// 	return (NULL); //terminate thread
		if (philo->data->time_to_die < philo->data->time_to_eat)
			ms_sleep(philo->data->time_to_die);
		else
			ms_sleep(philo->data->time_to_eat); //put in a function to modify the state too
		pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
		pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
		pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 3);//printf(" - Philo %d is sleeping\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		if (philo->data->time_to_die < philo->data->time_to_eat + philo->data->time_to_sleep) //->sleep time to die
			ms_sleep(philo->data->time_to_die - philo->data->time_to_eat);
		else
		{
			ms_sleep(philo->data->time_to_sleep);
			pthread_mutex_lock(&philo->data->print_mutex);
			print_message(philo, 4); //think message
			pthread_mutex_unlock(&philo->data->print_mutex);
		}
	}
	return (NULL);
}

void	init_philo(t_data *data, int i)
{
	data->philo_struct[i].id = i + 1;
	data->philo_struct[i].data = data;
	data->philo_struct[i].last_meal = 0;
	pthread_mutex_init(&data->forks_mutex[i], NULL);
}

void	execute(t_data *data)
{
	int		i;

	i = 0;
	data->start_time = time_stamp();//get the start time of the simulation
	pthread_mutex_init(&(data->print_mutex), NULL); //init my mutex to print_mess
	//pthread_mutex_init(&(data->death_mutex), NULL); //init my mutex to die
	while (i < data->nb_philos) //loop to initialize mutex for the forks
	{
		init_philo(data, i);  //initialise les mutex AVANT de faire la boucle pour les threads
		i++;
	}
	i = 0; //reset index
	while (i < data->nb_philos) //loop to create the threads --  TRYING to fix dead mess: && (time_stamp() - data->start_time < data->time_to_death)
	{
		if (pthread_create(&(data->philo_struct[i].philo_th), NULL, &routine, &(data->philo_struct[i])) != 0) //sending the thread, NULL, thread function and the struct for each philo
		{
			printf("%s\n", ERR_THREAD);
			return ;
		}
		usleep(100); //to give time for each philo to take a fork
		i++;
	}
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
