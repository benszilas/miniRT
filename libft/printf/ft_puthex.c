/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 17:52:38 by vvobis            #+#    #+#             */
/*   Updated: 2024/09/17 12:09:29 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_puthex_upper(unsigned long nbr, int *count, int fd)
{
	char	*base_str;

	base_str = "0123456789ABCDEF";
	if (nbr >= 16)
		ft_puthex_upper(nbr / 16, count, fd);
	ft_putchar(base_str[(nbr % 16)], count, fd);
}

void	ft_puthex_lower(unsigned long nbr, int *count, int fd)
{
	char	*base_str;

	base_str = "0123456789abcdef";
	if (nbr >= 16)
		ft_puthex_lower(nbr / 16, count, fd);
	ft_putchar(base_str[(nbr % 16)], count, fd);
}
