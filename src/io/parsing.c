/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:12:18 by victor            #+#    #+#             */
/*   Updated: 2024/11/04 15:29:52 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	collect_param_comma(char **entry_position, char *params[], uint count)
{
	char	*tmp;
	uint	i;

	i = 0;
	tmp = *entry_position;
	params[i] = tmp;
	while (i < count)
	{
		tmp = ft_strchr(tmp, ',');
		if (!tmp)
			return (false);
		*tmp++ = 0;
		i++;
		params[i] = tmp;
	}
	while (*tmp && !ft_isspace((*tmp)))
		tmp++;
	if (*tmp)
		*tmp = 0;
	*entry_position = tmp + 1;
	return (true);
}

bool	collect_param_single(char **entry_position, char *params[])
{
	char	*tmp;

	tmp = *entry_position;
	while (*tmp && ft_isspace(*tmp))
		tmp++;
	if (!*tmp)
		return (false);
	while (*tmp && !ft_isspace(*tmp))
		tmp++;
	*tmp = 0;
	params[0] = *entry_position;
	*entry_position = tmp + 1;
	return (true);
}

uint	id_set(uint group, int id)
{
	return ((group << 24) | (id & 0x00ffffff));
}

uint	id_group_get(int id)
{
	if (id < 0)
		return (-1);
	return ((id >> 24) & 0xff);
}

uint	parse_body_color(char *params[], int *error)
{
	return (set_color(set_signed_int(params[0], 0, UCHAR_MAX, error), \
				set_signed_int(params[1], 0, UCHAR_MAX, error), \
				set_signed_int(params[2], 0, UCHAR_MAX, error)));
}
