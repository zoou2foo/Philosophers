/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:06:00 by vjean             #+#    #+#             */
/*   Updated: 2023/03/03 09:14:54 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H


# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <time.h>

# define ERR_NB_ARGS	"Incorrect number of arguments. It has to be 5 or 6\n"
# define ERR_ARGS		"Incorrect type of arguments. Only positive numbers\n"
# define ERR_PHILO		"Incorrect number of philosophers at the table\n"
# define ERR_TYPE_ARGS	"Incorrect type of arguments. Need to be positive numbers"
# define ERR_IN_TIME	"Error: no negative time"
# define ERR_INT		"Error: int too long"

typedef struct s_philo{ //struct for each philo; need to add id;
	int		who_eat;
	int		who_think;
	int		who_sleep;
	int		who_die;
	int		right_fork;
	int		left_fork;
	int		last_meal;
}			t_philo;
//threads philos[200];

typedef struct s_data{ //parameters needed for simulation
	int				nb_philos;
	int				time_to_death;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_to_eat;
	pthread_mutex_t	machin;
}					t_data;

/*		PARSING 		*/
void	parsing(char **str, t_data *data);

/*		UTILS		*/
int	ft_strlen(char *str);
int	ft_atoi(char *str);

/*		EXECUTE		*/
void	execute(t_data *data);

#endif