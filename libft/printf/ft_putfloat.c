/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putfloat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:50:45 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 14:15:28 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbrf(double f, int *count, int precision, int fd)
{
	int				number;
	double			fraction;
	unsigned int	mult;
	char			sign;

	mult = 1;
	sign = 0;
	if (f < 0.0 && f > -1.0)
		sign = 1;
	while (precision-- > 0)
	{
		mult *= 10;
	}
	number = (int)f;
	fraction = f - (double)number;
	if (sign)
		ft_putchar('-', count, fd);
	ft_putnbr(number, count, fd);
	ft_putchar('.', count, fd);
	number = fraction * mult;
	if (number < 0)
		number = -number;
	ft_putnbr(number, count, fd);
}
