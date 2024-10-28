/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:50:35 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/17 12:13:18 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H

# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_fprintf(int fd, const char *format, ...);
int		ft_printf(const char *format, ...);

int		handle_arg(va_list args, char format, int *count, int fd);

void	ft_puthex_lower(unsigned long nbr, int *count, int fd);
void	ft_puthex_upper(unsigned long nbr, int *count, int fd);
void	ft_putchar(int c, int *count, int fd);
void	ft_putnbr(long n, int *count, int fd);
void	ft_putnbrf(double f, int *count, int precision, int fd);
void	ft_putstr(const char *str, int *count, int fd);
void	ft_putptr(void *ptr, int *count, int fd);
size_t	ft_strlen(const char *s);

#endif
