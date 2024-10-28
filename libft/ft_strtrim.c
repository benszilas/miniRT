/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:30:35 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/15 13:07:55 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char const *s)
{
	while (*s)
		if (c == *s++)
			return (1);
	return (0);
}

static int	find_end(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (*s1)
		s1++;
	while (is_set(*--s1, set))
		i++;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	while (is_set(*s1, set) && *s1)
		s1++;
	if ((int)ft_strlen(s1) > find_end(s1, set))
		return (ft_substr(s1, 0, ft_strlen(s1) - find_end(s1, set)));
	else
		return (ft_calloc(1, 1));
}
