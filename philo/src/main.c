/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:05:38 by vjean             #+#    #+#             */
/*   Updated: 2023/04/04 14:49:08 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
	{
		printf("%s\n", ERR_NB_ARGS);
		return (1);
	}
	else
	{
		if (parsing(av, &data) == 1)
			return (1);
		else
			execute(av, &data);
	}
	return (0);
}
