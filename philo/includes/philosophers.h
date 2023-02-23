/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:06:00 by vjean             #+#    #+#             */
/*   Updated: 2023/02/23 12:07:35 by vjean            ###   ########.fr       */
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

typedef struct s_philo{
	int		who_ate;
	int		right_fork;
	int		left_fork;
	int		last_meal;
}			t_philo;


typedef struct s_data{
	int				nb_philos;
	int				time_to_death;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_to_eat;
	pthread_mutex_t	machin;
}					t_data;

/*		PARSING 		*/
void	parsing(char **str);

/*		section 2		*/

/*		section 3		*/

#endif