/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:06:00 by vjean             #+#    #+#             */
/*   Updated: 2023/03/24 14:28:29 by vjean            ###   ########.fr       */
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
# define ERR_TYPE_ARGS	"Incorrect type of arguments. Need to be positive numbers and higher numbers\n"
# define ERR_IN_TIME	"Error: no negative time"
# define ERR_INT		"Error: int too long"
# define ERR_THREAD		"Error to create thread"

typedef enum {
	THINKING,
	EATING,
	SLEEPING,
	FULL,
	DEAD
}	state_t;
typedef struct s_philo{ //struct for each philo; need to add id?
	state_t				state; //to keep track of their state; see above
	int					id; //philo's id
	time_t				last_meal; //to know the last time they ate
	int					nb_meals_enjoyed; //to keep track of number of times that they ate
	struct s_data		*data; //give access to info in data struct
	pthread_t			philo_th; //comme c'est une struct pour chaque philo; c'est un thread
}						t_philo;

typedef struct s_data{ //parameters needed for simulation (rules)
	int						nb_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						nb_to_eat;
	int						someone_is_dead;
	int						nb_full_philos;
	time_t					start_time;
	pthread_mutex_t			full_mutex;
	pthread_mutex_t			last_meal_mutex;
	pthread_mutex_t			dead_body;
	pthread_mutex_t			print_mutex;
	pthread_mutex_t			forks_mutex[200]; //each philo will need its own. Le philo a besoin de prendre la fourchette de son voisin. Pas plus de 200 fourchettes, car pas plus que 200 philos. Le philo ne peut pas avoir acces aux 200, donc, ne doit pas etre dans la struc du philo
	struct s_philo			philo_struct[200]; //chaque philo est une STRUCT pour avoir acces aux info(forks, son id, etc..)
}							t_data;

/*		PARSING 		*/
int		parsing(char **str, t_data *data);

/*		UTILS			*/
int		ft_strlen(char *str);
int		ft_atoi(char *str);
time_t	time_stamp(void);
void	ms_sleep(int ms);

/*		EXECUTE			*/
void	execute(t_data *data);
void	print_message(t_philo *philo, char *str);

/*		ACTIONS			*/
void	take_first_fork(t_philo *philo);
void	take_second_fork(t_philo *philo);
void	eat(t_philo *philo);
void	time_to_sleep(t_philo *philo);
void	think(t_philo *philo);

/*		INIT_STUFF		*/
void	init_philo(t_data *data, int i);
void	setup_datastruct(t_data *data, char **str);
void	init_singles_mutex(t_data *data);
void	init_philo_mutex(t_data *data);

/*		CHECK_UP		*/
bool	is_dead(t_philo *philo);
void	stop_simulation(t_philo *philo);
void	exit_simulation(t_data *data);

#endif