/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glyph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:29:23 by vvobis            #+#    #+#             */
/*   Updated: 2024/12/05 14:45:55 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

static void	glyphs_create(char ****glyph, char const *path)
{
	size_t	i;
	int		fd;
	size_t	j;
	char	c;

	i = 0;
	*glyph = ft_calloc(GLYPH_COUNT, sizeof(*glyph));
	lst_memory(*glyph, free, ADD);
	ft_open(&fd, path, O_RDONLY, 0644);
	while (i < GLYPH_COUNT)
	{
		(*glyph)[i] = ft_calloc(GLYPH_ROW, sizeof(**glyph));
		lst_memory((*glyph)[i], free, ADD);
		j = -1;
		while (++j < GLYPH_ROW)
		{
			(*glyph)[i][j] = ft_calloc(GLYPH_COL, sizeof(***glyph));
			lst_memory((*glyph)[i][j], free, ADD);
			ft_read(fd, (*glyph)[i][j], GLYPH_COL + 1);
			(*glyph)[i][j][GLYPH_COL] = 0;
		}
		ft_read(fd, &c, 1);
		i++;
	}
	ft_close(fd);
}

static void	glyph_draw(	t_pixel *pixel, \
						uint x, \
						uint y, \
						char **glyph)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (x + 1 < WI)
	{
		while (i / 2 < GLYPH_ROW)
		{
			j = 0;
			while (j / 2 < GLYPH_COL)
			{
				if (x + j + 1 < WI && y + i + 1 < HI \
						&& glyph[i / 2][j / 2] == '1')
				{
					*pixel[(y + i) * WI + (x + j)].color = 0xffffff;
					*pixel[(y + i + 1) * WI + (x + j)].color = 0xffffff;
					*pixel[(y + i) * WI + (x + j + 1)].color = 0xffffff;
					*pixel[(y + i + 1) * WI + (x + j + 1)].color = 0xffffff;
				}
				j += 2;
			}
			i += 2;
		}
	}
}

static int	glyph_which(uint *begin_x, uint *begin_y, char const c)
{
	if (ft_isalpha(c))
	{
		*begin_x += 12;
		if (c >= 'A' && c <= 'Z')
			return (c - 65 + 26);
		return (c - 97 + 52);
	}
	else if (ft_isdigit(c))
	{
		return (*begin_x += 12, c - 48 + 16);
	}
	else if (c >= ' ' && c <= '/')
		return (*begin_x += 12, c - 32);
	else if (c == '_')
		return (*begin_x += 12, 78);
	else if (c == ':')
		return (*begin_x += 12, 79);
	else if (c == '=')
		return (*begin_x += 12, 80);
	else if (c == '\n')
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
	static char	***glyph = {0};

	if (!glyph)
		glyphs_create(&glyph, "./assets/alpha_bit_bonus");
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
