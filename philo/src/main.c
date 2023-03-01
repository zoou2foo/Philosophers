/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjean <vjean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:05:38 by vjean             #+#    #+#             */
/*   Updated: 2023/03/01 11:05:25 by vjean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 5 && ac != 6)
	{
		printf("%s\n", ERR_NB_ARGS);//need to add error message of not enough args
		exit(EXIT_FAILURE);
	}
	else
		parsing(av); //probably better to send it in a while loop?
	return (0);
}