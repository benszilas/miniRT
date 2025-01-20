/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:00:49 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/16 16:19:58 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

bool	check_position(char **tmp)
{
	bool	period;

	period = false;
	while (**tmp && !ft_isspace(**tmp))
	{
		if (!ft_isdigit(**tmp) && **tmp != '-')
			if (!(**tmp == '.' && !period))
				return (false);
		if (**tmp == '.')
			period = true;
		(*tmp)++;
	}
	return (true);
}

bool	check_normal_vector(t_vector *vec)
{
	if (vec->x < -1 || vec->x > 1)
		return (false);
	if (vec->y < -1 || vec->y > 1)
		return (false);
	if (vec->z < -1 || vec->z > 1)
		return (false);
	if (vec->x == 0 && vec->y == 0 && vec->z == 0)
		return (false);
	return (true);
}
