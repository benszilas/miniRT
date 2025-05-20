/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:12:18 by victor            #+#    #+#             */
/*   Updated: 2024/12/11 18:07:57 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	collect_param_comma(char **entry_position, char *params[], \
		uint count, int mode)
{
	char	*tmp;
	uint	i;

	i = 0;
	tmp = *entry_position;
	params[i] = tmp;
	while (i < count)
	{
		while (*tmp && *tmp != ',')
			if (ft_isspace(*tmp++))
				return (false);
		if (!*tmp)
			return (false);
		*tmp++ = 0;
		i++;
		params[i] = tmp;
	}
	if (!check_position(&tmp))
		return (false);
	if (mode == COLLECT_MODE_END)
		if (!check_end(tmp))
			return (false);
	if (*tmp)
		*tmp = 0;
	return (*entry_position = tmp + 1, true);
}

bool	collect_param_single(char **entry_position, char *params[], int mode)
{
	char	*tmp;

	tmp = *entry_position;
	while (*tmp && ft_isspace(*tmp))
		tmp++;
	if (!check_position(&tmp))
		return (false);
	if (mode == COLLECT_MODE_END)
		if (!check_end(tmp))
			return (false);
	*tmp = 0;
	if (params)
		params[0] = *entry_position;
	*entry_position = tmp + 1;
	return (true);
}

bool	check_end(char *entry)
{
	if (!entry)
		return (true);
	while (*entry && ft_isspace(*entry))
		entry++;
	if (*entry != 0)
		return (false);
	return (true);
}

void	err(char *body_type, uint line)
{
	ft_fprintf(STDERR_FILENO, "Error\n"\
	"Invalid %s configuration occured at line: %d\n", body_type, line);
}

bool	check_line(char *tmp)
{
	while (tmp && *tmp)
	{
		if (!ft_isspace(*tmp) && !ft_isdigit(*tmp) \
			&& *tmp != '.' && *tmp != ',' \
			&& *tmp != '-' && *tmp != '+')
			return (false);
		tmp++;
	}
	return (true);
}
