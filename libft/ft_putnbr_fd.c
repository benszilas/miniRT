/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 18:02:14 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/11 11:23:10 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
		ft_putchar_fd(0x2d, fd);
	if (n <= -10)
		ft_putnbr_fd(n / -10, fd);
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	if (n >= 0)
		ft_putchar_fd(n % 10 + 0x30, fd);
	if (n < 0)
		ft_putchar_fd(-(n % -10) + 0x30, fd);
}
