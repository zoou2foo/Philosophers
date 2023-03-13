/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:06:00 by vjean             #+#    #+#             */
/*   Updated: 2023/03/13 15:48:32 by vjean            ###   ########.fr       */
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
	SLEEPING
}	state_t;
typedef struct s_philo{ //struct for each philo; need to add id?
	state_t				state; //to keep track of their state; see above
	int					id; //philo's id
	int					right_fork;
	int					left_fork;
	int					last_meal;
	struct s_data		*data; //checker l'allocation
	pthread_t			philos; //comme c'est une struct pour chaque philo; c'est un thread
}						t_philo;
//threads philos[200];

typedef struct s_data{ //parameters needed for simulation (rules)
	int						nb_philos;
	int						time_to_death;
	int						time_to_eat;
	int						time_to_sleep;
	int						nb_to_eat;
	long					elapsed_time_ms;
	pthread_mutex_t			forks[200]; //each philo will need its own. Le philo a besoin de prendre la fourchette de son voisin. Pas plus de 200 fourchettes, car pas plus que 200 philos. Le philo ne peut pas avoir acces aux 200, donc, ne doit pas etre dans la struc du philo
	struct s_philo			philo[200]; //chaque philo est une STRUCT pour avoir acces aux info(forks, son id, etc..)
}							t_data;

/*		PARSING 		*/
void	parsing(char **str, t_data *data);

/*		UTILS		*/
int		ft_strlen(char *str);
int		ft_atoi(char *str);
void	time_stamp(t_data *data);

/*		EXECUTE		*/
void	execute(t_data *data);

#endif