/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:16:48 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/11 14:48:59 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_len(int n)
{
	int		i;

	if (n < 0)
		n = -n;
	i = 0;
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

static char	*is_negative(int n)
{
	int		i;
	char	*num;

	i = get_len(n);
	num = malloc(sizeof(*num) * i + 2);
	if (!num)
		return (NULL);
	num[i + 1] = 0;
	while (n)
	{
		num[i--] = -(n % 10) + 48;
		n /= 10;
	}
	num[i] = 0x2d;
	return (num);
}

static char	*is_positive(int n)
{
	int		i;
	char	*num;

	i = get_len(n);
	num = malloc(sizeof(*num) * i + 1);
	if (!num)
		return (NULL);
	num[i--] = 0;
	while (n)
	{
		num[i--] = (n % 10) + 48;
		n /= 10;
	}
	return (num);
}

char	*ft_itoa(int n)
{
	if (n == 0)
		return (ft_strdup("0"));
	else if (n < 0)
		return (is_negative(n));
	else
		return (is_positive(n));
}
