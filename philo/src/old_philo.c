
#include "philosophers.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (check_dead(philo->data) == 0)
	{
		if ((check_dead(philo->data) == 0) && (check_if_philo_dead(philo) == false))
		{
			philo->state = DEAD;
			pthread_mutex_lock(&philo->data->print_mutex);
			printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
			pthread_mutex_unlock(&philo->data->print_mutex);
			pthread_mutex_lock(&philo->data->dead_body);
			philo->data->someone_is_dead = 1;
			pthread_mutex_unlock(&philo->data->dead_body);
			return (NULL); //to terminate thread
		}
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
	while (check_dead(data) != 1)
	{
		pthread_mutex_lock(&data->full_mutex);
		if (data->nb_full_philos == data->nb_philos)
		{
			pthread_mutex_unlock(&data->full_mutex);
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld - All philosophers have eaten enough\n", time_stamp() - data->start_time);
			pthread_mutex_unlock(&data->print_mutex);
			pthread_mutex_lock(&data->dead_body);
			data->someone_is_dead = 1;
			pthread_mutex_unlock(&data->dead_body);
			break ;
		}
		pthread_mutex_unlock(&data->full_mutex);
	}
}

/*			ACTIONS				*/
/* FOUR functions... */

void	print_eating(t_philo *philo)
{
	philo->state = EATING;
	lock_n_print(philo, "is eating");
	//printf("%ld - Philo %d is eating\n", time_stamp() - philo->data->start_time, philo->id);
	philo->nb_meals_enjoyed++;
	if (philo->nb_meals_enjoyed == philo->data->nb_to_eat)
	{
		philo->state = FULL;
		pthread_mutex_lock(&philo->data->full_mutex);
		philo->data->nb_full_philos++; // increment the number of full philosophers. DATA RACE
		pthread_mutex_unlock(&philo->data->full_mutex);
	}
}

void	lock_n_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld - Philo %d %s\n", time_stamp() - philo->data->start_time, philo->id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	print_message(t_philo *philo, int flag) //fait trop de choses separes pour function qui s'occupe du state, une autre pour print
{

	if (check_dead(philo->data) == 1) //data race
		return (1);
	if (flag == 1 && check_dead(philo->data) != 1)
		lock_n_print(philo, "has taken a fork");
		//printf("%ld - Philo %d has taken a fork\n", time_stamp() - philo->data->start_time, philo->id); //get time(current time - start time) APRES mutex
	else if (flag == 2 && check_dead(philo->data) != 1)
		print_eating(philo);
	else if (flag == 3 && check_dead(philo->data) != 1)
	{
		philo->state = SLEEPING;
		lock_n_print(philo, "is sleeping");
		//printf("%ld - Philo %d is sleeping\n", time_stamp() - philo->data->start_time, philo->id);
	}
	else if (flag == 4 && check_dead(philo->data) != 1)
	{
		philo->state = THINKING;
		lock_n_print(philo, "is thinking");
		//printf("%ld - Philo %d is thinking\n", time_stamp() - philo->data->start_time, philo->id);
	}
	// else if (flag == 5 && check_dead(philo->data) != 1)
	// {
	// 	philo->state = DEAD;
	// 	lock_n_print(philo, "is dead");
	// 	//printf("%ld - Philo %d is dead\n", time_stamp() - philo->data->start_time, philo->id);
	// 	pthread_mutex_lock(&philo->data->dead_body);
	// 	philo->data->someone_is_dead = 1;
	// 	pthread_mutex_unlock(&philo->data->dead_body);
	// }
	else
		return (1);
	return (0);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->forks_mutex[philo->id - 1]));
	//pthread_mutex_lock(&philo->data->print_mutex);
	if (print_message(philo, 1) == 1) //printf("- Philo %d has taken a fork\n", philo->id);
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]);
		//pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	//pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_lock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
	//pthread_mutex_lock(&philo->data->print_mutex);
	if (print_message(philo, 1) == 1)//printf("- Philo %d has taken a fork\n", philo->id celle de son voisin);
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]);
		pthread_mutex_unlock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
		//pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	//pthread_mutex_unlock(&philo->data->print_mutex);
	//pthread_mutex_lock(&philo->data->print_mutex);
	if (print_message(philo, 2) == 1)//printf(" - Philo %d is eating\n", philo->id);
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->id - 1]);
		pthread_mutex_unlock(&philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]);
		//pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	//pthread_mutex_unlock(&philo->data->print_mutex);
	philo->last_meal = time_stamp() - philo->data->start_time;
	if (philo->data->time_to_die < philo->data->time_to_eat)
		ms_sleep(philo->data->time_to_die);
	else
		ms_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&(philo->data->forks_mutex[philo->id - 1]));
	pthread_mutex_unlock(&(philo->data->forks_mutex[(philo->id) % philo->data->nb_philos]));
}

void	go_to_sleep(t_philo *philo)
{
	//pthread_mutex_lock(&philo->data->print_mutex);
	print_message(philo, 3);//printf(" - Philo %d is sleeping\n", philo->id);
	//pthread_mutex_unlock(&philo->data->print_mutex);
	if (philo->data->time_to_die < philo->data->time_to_eat + philo->data->time_to_sleep) //->sleep time to die
		ms_sleep(philo->data->time_to_die - philo->data->time_to_eat);
	else
	{
		ms_sleep(philo->data->time_to_sleep);
		//pthread_mutex_lock(&philo->data->print_mutex);
		print_message(philo, 4); //think message
		//pthread_mutex_unlock(&philo->data->print_mutex);
	}
}
