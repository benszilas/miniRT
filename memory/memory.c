/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:21:13 by victor            #+#    #+#             */
/*   Updated: 2024/09/12 16:16:33 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

void	lst_memory(void *mem, void (*del)(void *c), int mode)
{
	static t_clean	*list;
	t_clean			*new;

	if (!del)
		del = free;
	if (mode == FAIL)
		return (lst_list_clean(&list), exit(EXIT_FAILURE));
	if (mode == END)
		return (lst_list_clean(&list));
	if (mode == FREE)
		return (lst_node_del_clean(&list, mem));
	if (!mem)
		return (lst_list_clean(&list), perror("malloc"), exit(EXIT_FAILURE));
	new = lst_node_new(mem, del);
	if (!new)
		return (del(mem), lst_list_clean(&list), \
				perror("malloc"), exit(EXIT_FAILURE));
	lst_add_back(&list, new);
}
