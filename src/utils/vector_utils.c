/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:56:37 by bszilas           #+#    #+#             */
/*   Updated: 2024/11/05 12:58:20 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

t_vector	rot_x(t_vector vec, int dir)
{
	return ((t_vector){vec.x, vec.y * COS_10 - vec.z * SIN_10 * dir, \
	vec.y * SIN_10 * dir + vec.z * COS_10});
}

t_vector	rot_y(t_vector vec, int dir)
{
	return ((t_vector){vec.x * COS_10 + vec.z * SIN_10 * dir, vec.y, \
	vec.x * dir * -SIN_10 + vec.z * COS_10});
}

t_vector	rot_z(t_vector vec, int dir)
{
	return ((t_vector){vec.x * COS_10 - vec.y * SIN_10 * dir, \
	vec.x * SIN_10 * dir + vec.y * COS_10, vec.z});
}

t_vector	vec_by_matrix(t_vector vec, t_matrix4 m)
{
	return ((t_vector){.x \
	= m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0], \
	.y = m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1], \
	.z = m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2]});
}

void	vector_print(t_vector vector, int fd)
{
	ft_putnbrf_fd(vector.x, fd, 3);
	ft_putchar_fd(',', fd);
	ft_putnbrf_fd(vector.y, fd, 3);
	ft_putchar_fd(',', fd);
	ft_putnbrf_fd(vector.z, fd, 3);
}
