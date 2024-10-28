/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_overflow.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:11:52 by bszilas           #+#    #+#             */
/*   Updated: 2024/10/06 18:18:08 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	positive_overflow(int64_t max, char *str)
{
	size_t	len;
	int64_t	tenth;
	int		last;

	len = count_digits(max, 10);
	if (*str == '-')
		return (false);
	if (*str == '+')
		str++;
	if (ft_strlen(str) > len)
		return (true);
	if (ft_strlen(str) == len)
	{
		tenth = get_one_tenth(str);
		last = get_last_char(str) - '0';
		if (tenth > max / 10 || (tenth == max / 10 && last > max % 10))
			return (true);
	}
	return (false);
}

bool	negative_overflow(int64_t min, char *str)
{
	size_t	len;
	int64_t	tenth;
	int		last;

	if (min == 0 && *str == '-')
		return (true);
	else if (min == 0)
		return (false);
	len = count_digits(min, 10) + 1;
	if (*str == '+')
		return (false);
	if (ft_strlen(str) > len)
		return (true);
	if (ft_strlen(str) == len)
	{
		tenth = get_one_tenth(str);
		last = - (get_last_char(str) - '0');
		if (tenth < min / 10 || (tenth == min / 10 && last < (min % 10)))
			return (true);
	}
	return (false);
}

bool	is_a_number(char *str)
{
	if (!str)
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

char	*skip_whitespace(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			break ;
		i++;
	}
	return (str + i);
}

int64_t	set_signed_int(const char *str, int64_t min, int64_t max, int *error)
{
	char	*nbr;

	if (!str)
	{
		*error = true;
		return (0);
	}
	nbr = skip_whitespace((char *)str);
	if (!is_a_number(nbr))
		*error = true;
	if (negative_overflow(min, nbr) || positive_overflow(max, nbr))
		*error = true;
	return (ft_atol(str));
}
