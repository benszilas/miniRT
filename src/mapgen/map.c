/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 19:07:56 by vvobis            #+#    #+#             */
/*   Updated: 2024/11/27 23:08:25 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	print_help(void)
{
	ft_fprintf(STDERR_FILENO, \
		"Usage: ./map_gen [OPTIONS] [INFILE] <OUTFILE>\n\t"
		"Options:\n\t\t-o\tCreate New Map with Random Values\n"
		"\t\t-i\tCreate New map From INFILE and Write to OUTFILE\n");
}

bool	open_file(const char *argv, int *fd, int options)
{
	if (*(argv) && *fd == 0)
	{
		*fd = open(argv, options, 0644);
		if (*fd == -1)
		{
			perror("open");
			return (false);
		}
	}
	else
		print_help();
	return (true);
}

void	check_input(int argc)
{
	if (argc < 2 || argc > 5)
	{
		ft_fprintf(STDERR_FILENO, \
				"Usage: ./mapgen <INFILE>.ppm <OUTFILE>.bump");
		exit (1);
	}
}

int	main(int argc, const char **argv)
{
	int	infile;
	int	outfile;
	int	i;

	check_input(argc);
	infile = 0;
	outfile = 0;
	i = 0;
	if (!open_file(argv[1], &infile, O_RDONLY))
		return (1);
	if (!open_file(argv[2], &outfile, O_CREAT | O_WRONLY | O_TRUNC))
		return (1);
	if (infile && outfile)
		map_create_from_ppm(infile, outfile);
	else
		print_help();
}
