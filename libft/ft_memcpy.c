/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:13:18 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/09 19:21:29 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, void const *src, size_t n)
{
	char	*d;
	char	*s;

	if (n == 0 || (dest == NULL && src == NULL))
		return (dest);
	if (dest == NULL || src == NULL)
	{
		*(char *)dest = 1;
		*(char *)src = 1;
	}
	d = (char *) dest;
	s = (char *) src;
	while (n--)
		*d++ = *s++;
	return (dest);
}
