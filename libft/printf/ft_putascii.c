/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putascii.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 17:54:53 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/17 16:35:10 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar(int c, int *count, int fd)
{
	write(fd, &c, 1);
	*count += 1;
}

void	ft_putnbr(long n, int *count, int fd)
{
	if (n < 0)
		ft_putchar(0x2d, count, fd);
	if (n <= -10)
		ft_putnbr(n / -10, count, fd);
	if (n >= 10)
		ft_putnbr(n / 10, count, fd);
	if (n >= 0)
		ft_putchar(n % 10 + 0x30, count, fd);
	if (n < 0)
		ft_putchar(-(n % -10) + 0x30, count, fd);
}

void	ft_putstr(const char *str, int *count, int fd)
{
	if (!str)
	{
		*count += ft_fprintf(fd, "(null)");
		return ;
	}
	*count += ft_fprintf(fd, str);
}
