/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:49:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/13 22:19:12 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_space(char const c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(char const *s)
{
	int			nb;
	char const	*tmp;

	nb = 0;
	while (is_space(*s))
		s++;
	tmp = s;
	if (*tmp == '+' || *tmp == '-')
		tmp++;
	while (*tmp >= '0' && *tmp <= '9')
	{
		nb *= 10;
		nb += (*tmp - '0');
		tmp++;
	}
	if (*s == '-')
		nb = -nb;
	return (nb);
}

long	ft_atol(const char *nptr)
{
	int64_t	n;
	int		sign;

	n = 0;
	sign = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
		sign = -1;
	if (sign == -1 || *nptr == '+')
		nptr++;
	while (*nptr >= '0' && *nptr <= '9')
		n = n * 10 + (*nptr++ - '0');
	return (n * sign);
}
