/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:17:01 by valeriejean       #+#    #+#             */
/*   Updated: 2023/04/04 14:48:09 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*		THREE FUNCTIONS		*/

int	check_args(char **str)
{
	int	i;
	int	j;

	i = 1;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (!(str[i][j] >= 48 && str[i][j] <= 57))
			{
				printf("%s\n", ERR_TYPE_ARGS);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	check_len(char **str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (ft_strlen(str[i]) > 6)
		{
			printf("%s\n", ERR_INT);
			return (1);
		}
		i++;
	}
	return (0);
}

//time to check args and do atoi with args
int	parsing(char **str, t_data *data)
{
	if (check_args(str) == 0 && check_len(str) == 0)
	{
		setup_datastruct(data, str);
		if (data->nb_philos < 1 || data->nb_philos > 200)
		{
			printf("%s\n", ERR_PHILO);
			return (1);
		}
	}
	return (0);
}
