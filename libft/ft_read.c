/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:49:30 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/18 12:50:46 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_read(int fd, char *character, unsigned int size_to_read)
{
	int	bytes_read;

	bytes_read = read(fd, character, size_to_read);
	if (bytes_read == -1)
	{
		ft_fprintf(STDERR_FILENO, "Read failed\n");
		exit (1);
	}
	return (bytes_read);
}
