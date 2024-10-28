/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:01:03 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/14 09:24:02 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "limits.h"

size_t	ft_strlen(char const *str)
{
	size_t	i;

	i = 0;
	while (*str++)
	{
		i++;
		if (i == ULONG_MAX)
			break ;
	}
	return (i);
}
