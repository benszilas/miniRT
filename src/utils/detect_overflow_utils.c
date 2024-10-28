/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_overflow_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:16:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/09/16 13:06:02 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

char	temporary_terminate_string(char *end)
{
	char	tmp;

	tmp = *end;
	*end = 0;
	return (tmp);
}

void	restore_string(char *end, char tmp)
{
	*end = tmp;
}

int64_t	get_one_tenth(char *nbr)
{
	char	*ptr;
	int64_t	tenth;
	char	last;

	if (!nbr || !*nbr)
		return (0);
	ptr = nbr + ft_strlen(nbr) - 1;
	last = temporary_terminate_string(ptr);
	tenth = ft_atol(nbr);
	restore_string(ptr, last);
	return (tenth);
}

char	get_last_char(char *str)
{
	if (!str || !*str)
		return (0);
	return (*(str + ft_strlen(str) - 1));
}

size_t	count_digits(int64_t n, int base)
{
	size_t	len;

	len = 1;
	while (n % base != n)
	{
		n = n / base;
		len++;
	}
	return (len);
}
