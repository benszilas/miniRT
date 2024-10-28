/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glyph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:29:23 by vvobis            #+#    #+#             */
/*   Updated: 2024/10/13 15:44:35 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

static void	glyphs_create(char glyph[30][7][7], char const *path)
{
	size_t	i;
	int		fd;
	size_t	j;
	char	c;

	i = 0;
	ft_open(&fd, path, O_RDONLY, 0644);
	while (i < GLYPH_COUNT)
	{
		j = 0;
		while (j < GLYPH_ROW)
		{
			ft_read(fd, glyph[i][j], GLYPH_COL + 1);
			glyph[i][j][GLYPH_COL] = 0;
			j++;
		}
		ft_read(fd, &c, 1);
		i++;
	}
	ft_close(fd);
}

static void	glyph_draw(	t_pixel *pixel, \
						uint x, \
						uint y, \
						char glyph[GLYPH_ROW][GLYPH_COL + 1])
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i / 2 < GLYPH_ROW)
	{
		j = 0;
		while (j / 2 < GLYPH_COL)
		{
			if (glyph[i / 2][j / 2] == '1')
			{
				*(int *)pixel[(y + i) * WI + (x + j)].color = 0xffffff;
				*(int *)pixel[(y + i + 1) * WI + (x + j)].color = 0xffffff;
				*(int *)pixel[(y + i) * WI + (x + j + 1)].color = 0xffffff;
				*(int *)pixel[(y + i + 1) * WI + (x + j + 1)].color = 0xffffff;
			}
			j += 2;
		}
		i += 2;
	}
}

static int	glyph_which(uint *begin_x, uint *begin_y, char const c)
{
	if (ft_isalpha(c))
		return (*begin_x += 12, c - 97);
	if (c == ':')
		return (*begin_x += 12, 26);
	if (c == '+')
		return (*begin_x += 12, 27);
	if (c == '-')
		return (*begin_x += 12, 28);
	if (c == '=')
		return (*begin_x += 12, 29);
	if (c == '\n')
		return (*begin_y += 16, -1);
	return (*begin_x += 12, -2);
}

void	glyph_print(	uint begin_x, \
						uint begin_y, \
						char const *text, \
						t_pixel *pixel)
{
	size_t		i;
	int			x;
	int			index;
	static char	glyph[30][7][7] = {0};
	static bool	created = 0;

	if (!created)
	{
		glyphs_create(glyph, "./assets/alpha_bit_bonus");
		created = true;
	}
	x = begin_x;
	i = 0;
	while (text[i])
	{
		index = glyph_which(&begin_x, &begin_y, text[i]);
		if (index >= 0)
		{
			glyph_draw(pixel, begin_x, begin_y, glyph[index]);
		}
		else if (index == -1)
			begin_x = x;
		i++;
	}
}
