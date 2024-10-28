/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putfloat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:50:45 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/17 12:07:47 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbrf(double f, int *count, int precision, int fd)
{
	int				number;
	double			fraction;
	unsigned int	mult;

	mult = 1;
	while (precision-- > 0)
	{
		mult *= 10;
	}
	number = (int)f;
	fraction = f - (double)number;
	if (fraction < 0)
		fraction = -fraction;
	ft_putnbr(number, count, fd);
	ft_putchar('.', count, fd);
	number = fraction * mult;
	ft_putnbr(number, count, fd);
}
