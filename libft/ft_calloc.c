/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:44:00 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/10 16:39:56 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t s)
{
	char				*tmp;
	unsigned long		i;

	i = 0;
	if (n == 0)
		return (malloc(0));
	if (SIZE_MAX / n < s)
		return (NULL);
	tmp = malloc(n * s);
	if (tmp)
		while (i < n * s)
			tmp[i++] = 0;
	return ((void *)tmp);
}
