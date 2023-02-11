/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriejean <valeriejean@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:31:45 by valeriejean       #+#    #+#             */
/*   Updated: 2023/02/11 16:38:54 by valeriejean      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int ft_strlen(char *str) //useful for malloc
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int ft_atoi(char *str) //change args in int
{
    int i;
    int sign;
    int result;
    
    i = 0;
    sign = 1;
    result = 0;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
        {
            sign *= -1;
            i++;
        }
    }
    while (str[i] >= 48 && str[i] <= 57)
    {
        result = result * 10 + str[i] - 48;
        i++;
    }
    return (result * sign);
}