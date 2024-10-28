/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:16:26 by victor            #+#    #+#             */
/*   Updated: 2024/10/15 12:07:57 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

static double	atod_collect_fraction(char *n, double d)
{
	bool	period;
	uint	period_positions;

	period_positions = 0;
	period = false;
	while (*n)
	{
		if (*n == '.' && period == false)
		{
			period = true;
			n++;
			continue ;
		}
		else if ((*n == '.' && period == true) \
				|| (!ft_isdigit(*n) && *n != '.'))
			return (0);
		if (period)
			period_positions++;
		d *= 10;
		d += (*n - '0');
		n++;
	}
	return (d / pow(10, period_positions));
}

double	ft_atod(char *n)
{
	double	d;
	int8_t	sign;

	if (!n)
		return (0);
	sign = 1;
	if (*n == '-')
	{
		sign = -1;
		n++;
	}
	else if (*n == '+')
		n++;
	d = 0;
	return (atod_collect_fraction(n, d) * sign);
}
