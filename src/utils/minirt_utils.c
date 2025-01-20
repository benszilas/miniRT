/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:50:35 by victor            #+#    #+#             */
/*   Updated: 2024/12/10 16:16:15 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

int	ft_read(int fd, char *character, unsigned int size_to_read)
{
	int	bytes_read;

	bytes_read = read(fd, character, size_to_read);
	if (bytes_read == -1)
	{
		ft_fprintf(STDERR_FILENO, "read: %s", strerror(errno));
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
		ft_fprintf(STDERR_FILENO, "close: %s", strerror(errno));
		lst_memory(NULL, NULL, FAIL);
	}
	return (1);
}

/* flag corresponds to O_RDONLY, O_CREAT etc.; 
 * When opening a file, permissions doesnt can be set to 0*/
void	ft_open(int *fd, const char *path, int flag, int permissons)
{
	if (permissons > 0)
		*fd = open(path, flag, permissons);
	else
		*fd = open(path, flag);
	if (*fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "open: %s: %s", path, strerror(errno));
		lst_memory(NULL, NULL, FAIL);
	}
}

void	minirt_fd_close(void *ptr)
{
	close(*(int *)ptr);
}

void	swap_2_ints(uint *a, uint *b)
{
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}
