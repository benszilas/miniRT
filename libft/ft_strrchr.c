/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:20:50 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/09 20:54:22 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(char const *s, int c)
{
	char	*n;
	int		i;

	i = 0;
	n = NULL;
	while (s[i] != 0)
	{
		if (s[i] == (char)c)
			n = (char *)&s[i];
		i++;
	}
	if ((char)c == 0)
		return ((char *)&s[i]);
	if (n)
		return (n);
	return (NULL);
}
