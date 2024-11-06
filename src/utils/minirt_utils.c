/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:50:35 by victor            #+#    #+#             */
/*   Updated: 2024/11/05 14:23:40 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

int	ft_read(int fd, char *character, unsigned int size_to_read)
{
	int	bytes_read;

	bytes_read = read(fd, character, size_to_read);
	if (bytes_read == -1)
	{
		perror("read");
		lst_memory(NULL, NULL, FAIL);
	}
	return (bytes_read);
}

int	ft_close(int fd)
{
	if (fd == -1)
		return (0);
	if (close(fd) == -1)
	{
		perror("close");
		lst_memory(NULL, NULL, FAIL);
	}
	return (1);
}

void	ft_open(int *fd, const char *path, int flag, int permissons)
{
	if (permissons > 0)
		*fd = open(path, flag, permissons);
	else
		*fd = open(path, flag);
	if (*fd == -1)
	{
		perror("open");
		lst_memory(NULL, NULL, FAIL);
	}
}

void	ft_putnbrf_fd(double f, int fd, int precision)
{
	int		number;
	double	fraction;

	number = (int)f;
	fraction = f - (double)number;
	if (fraction < 0)
		fraction = -fraction;
	ft_putnbr_fd(number, fd);
	ft_putchar_fd('.', fd);
	number = fraction * pow(10, precision);
	ft_putnbr_fd(number, fd);
}
