/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:14:13 by vvobis            #+#    #+#             */
/*   Updated: 2024/05/20 11:38:03 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	g_strlen(char const *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (*str)
	{
		i++;
		str++;
	}
	return (i);
}

size_t	find_newline(char *buf)
{
	size_t	i;

	if (!buf)
		return (0);
	i = 0;
	while (*buf)
	{
		if (*buf == '\n')
			return (i + 1);
		i++;
		buf++;
	}
	return (0);
}

char	*g_strjoin(char const *s1, char const *s2)
{
	char			*tmp;
	unsigned int	i;
	unsigned int	j;

	if (!s2)
		return (NULL);
	if (!s1)
		return (g_substr(s2, 0, g_strlen(s2)));
	tmp = g_calloc(g_strlen(s1) + g_strlen(s2) + 1, sizeof(*tmp));
	if (!tmp)
		return (NULL);
	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			tmp[i] = s1[i];
			i++;
		}
	}
	j = 0;
	if (s2)
		while (s2[j])
			tmp[i++] = s2[j++];
	return (tmp);
}

char	*g_substr(char const *s, unsigned int start, size_t len)
{
	char			*tmp;
	unsigned int	i;

	i = 0;
	if (!s || start >= g_strlen(s) || len <= 0)
	{
		tmp = malloc(1);
		if (!tmp)
			return (NULL);
		tmp[i] = 0;
		return (tmp);
	}
	if (len + start > g_strlen(s))
		len = g_strlen(s) - start;
	tmp = g_calloc(len + 1, sizeof(*tmp));
	if (!tmp)
		return (NULL);
	while (i < len && s[i])
	{
		tmp[i] = s[i + start];
		i++;
	}
	return (tmp);
}

void	*g_calloc(size_t n, size_t s)
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
