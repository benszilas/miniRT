/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:14:20 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/09 23:55:49 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count(char const *s, char const c)
{
	int	n;

	n = 0;
	if (*s != c && *s)
		n = 1;
	while (*s)
	{
		if (*s == c && *(s + 1) != c && *(s + 1))
			n++;
		s++;
	}
	return (n);
}

static int	count_sub(char const *s, char const c)
{
	int	i;

	i = 0;
	while (*s != c && *s)
	{
		i++;
		s++;
	}
	return (i);
}

static char	**free_all(char **back)
{
	char	**tmp;

	tmp = back;
	while (*back)
	{
		free(*back);
		back++;
	}
	free(tmp);
	return (NULL);
}

char	**ft_split(char const *s, char const c)
{
	char	**tmp;
	char	**back;

	if (!s)
		return (NULL);
	tmp = (char **)ft_calloc(sizeof(*tmp), count(s, c) + 1);
	if (!tmp)
		return (NULL);
	back = tmp;
	while (*s && tmp)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			*tmp = ft_substr(s, 0, count_sub(s, c));
			if (!*tmp)
				return (free_all(back));
		}
		tmp++;
		while (*s != c && *s)
			s++;
	}
	return (back);
}
