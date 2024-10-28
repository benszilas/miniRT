/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 17:50:35 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/17 14:11:57 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_arg(va_list args, char format, int *count, int fd)
{
	if (format == 'd' || format == 'i')
		ft_putnbr(va_arg(args, int), count, fd);
	else if (format == 'u')
		ft_putnbr(va_arg(args, unsigned int), count, fd);
	else if (format == 's')
		ft_putstr(va_arg(args, char *), count, fd);
	else if (format == 'X')
		ft_puthex_upper(va_arg(args, unsigned int), count, fd);
	else if (format == 'x')
		ft_puthex_lower(va_arg(args, unsigned int), count, fd);
	else if (format == 'p')
		ft_putptr(va_arg(args, void *), count, fd);
	else if (format == 'c')
		ft_putchar(va_arg(args, int), count, fd);
	else if (format == '%')
		ft_putchar('%', count, fd);
	else if (format == 'f')
		ft_putnbrf(va_arg(args, double), count, 5, fd);
	else
		return (0);
	return (1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	if (!format)
		return (-1);
	va_start(args, format);
	count = 0;
	while (1)
	{
		while (*format != '%' && *format)
			ft_putchar(*format++, &count, STDOUT_FILENO);
		if (!*format)
			break ;
		else
			format++;
		if (!*format || !handle_arg(args, *format, &count, STDOUT_FILENO))
			return (-1);
		format++;
	}
	va_end(args);
	return (count);
}
