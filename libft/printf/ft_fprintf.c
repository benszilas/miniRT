/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:04:30 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/17 12:09:29 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list	args;
	int		count;

	if (!format || fd < 0)
		return (-1);
	va_start(args, format);
	count = 0;
	while (1)
	{
		while (*format != '%' && *format)
			ft_putchar(*format++, &count, fd);
		if (!*format)
			break ;
		else
			format++;
		if (!*format || !handle_arg(args, *format, &count, fd))
			return (-1);
		format++;
	}
	va_end(args);
	return (count);
}
