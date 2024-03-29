/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:06:00 by vjean             #+#    #+#             */
/*   Updated: 2023/04/15 15:08:43 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>

# define ERR_NB_ARGS	"Incorrect number of arguments. It has to be 5 or 6\n"
# define ERR_ARGS		"Incorrect type of arguments. Only positive numbers\n"
# define ERR_PHILO		"Incorrect number of philosophers at the table\n"
# define ERR_TYPE_ARGS	"Incorrect type of args. Positive numbers needed\n"
# define ERR_IN_TIME	"Error: no negative time"
# define ERR_INT		"Error: int too long"
# define ERR_THREAD		"Error to create thread"

typedef enum e_state{
	ALIVE,
	DEAD
}	t_state;
typedef struct s_philo{
	t_state				state;
	int					id;
	time_t				last_meal;
	int					nb_meals_enjoyed;
	int					nb_to_eat;
	struct s_data		*data;
	pthread_t			philo_th;
}						t_philo;

typedef struct s_data{
	int						status;
	int						nb_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						nb_full_philos;
	time_t					start_time;
	pthread_mutex_t			full_mutex;
	pthread_mutex_t			count_full;
	pthread_mutex_t			last_meal_mutex;
	pthread_mutex_t			state_mutex;
	pthread_mutex_t			print_mutex;
	pthread_mutex_t			status_mutex;
	pthread_mutex_t			forks_mutex[200];
	struct s_philo			philo_struct[200];
}							t_data;

/*		PARSING 		*/
int		parsing(char **str, t_data *data);

/*		UTILS			*/
int		ft_strlen(char *str);
int		ft_atoi(char *str);
time_t	time_stamp(void);
void	ms_sleep(int ms);

/*		EXECUTE			*/
void	execute(char **av, t_data *data);
void	print_message(t_philo *philo, char *str);
void	lock_n_change(t_data *data, int i);

/*		ACTIONS			*/
void	take_first_fork(t_philo *philo);
void	take_second_fork(t_philo *philo);
void	eat(t_philo *philo);
void	time_to_sleep(t_philo *philo);

/*		INIT_STUFF		*/
void	init_philo(char **av, t_data *data, int i);
void	setup_datastruct(t_data *data, char **str);
void	init_singles_mutex(t_data *data);
void	init_philo_mutex(char **av, t_data *data);
void	kill_mutex(t_data *data);

/*		CHECK_UP		*/
int		is_dead(t_philo *philo);
void	end_when_dead(t_data *dat, int i);
int		is_full(t_philo *philo);
void	check_health(t_data *data);

#endif