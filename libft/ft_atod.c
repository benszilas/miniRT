/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 22:25:35 by victor            #+#    #+#             */
/*   Updated: 2024/09/17 12:09:40 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	atod_collect_fraction(char *n, double d)
{
	int		period;
	int		mult;

	mult = 1;
	while (*n)
	{
		if (*n == '.' && period == 0)
		{
			period = 1;
			n++;
			continue ;
		}
		else if ((*n == '.' && period == 1) \
				|| (!ft_isdigit(*n) && *n != '.'))
			return (ft_putendl_fd("Invalid double format: Too many periods", \
						2), exit(1), 0);
		if (period)
		{
			mult *= 10;
		}
		d *= 10;
		d += (*n - '0');
		n++;
	}
	return (d / mult);
}

double	ft_atod(char *n)
{
	double	d;
	int		sign;

	if (!n)
		exit(1);
	sign = 1;
	if (*n == '-')
	{
		sign = -1;
		n++;
	}
	d = 0;
	return (atod_collect_fraction(n, d) * sign);
}
