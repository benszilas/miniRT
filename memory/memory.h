/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:18:13 by victor            #+#    #+#             */
/*   Updated: 2024/09/08 18:03:07 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <stdlib.h>
# include <stdio.h>

# define INTERN

enum e_alloc
{
	ADD,
	FAIL,
	END,
	FREE,
};

typedef struct s_clean
{
	void			*content;
	void			(*clean)(void *del);
	struct s_clean	*next;
}					t_clean;

/* Main memory handling function:
 *
 * Pointers may be passed to this function and will be stored on a linked list.
 * DEL_FUNC is a function pointer that will be used to destroy the
 * created node with the specified funtion.
 * If DEL_FUNC is NULL, the pointer will be attempted to be freed using free() 
 *
 * MODEs are: 
 *		ADD: adds a node to the end of the list
 *		FREE: remove the specified node using the address pointed to by MEM
 *		END: cleans the list
 *		FAIL: cleans the list and exits with exit code (1)*/
void			lst_memory(void *mem, void (*del_func)(void *c), int mode);

INTERN t_clean	*lst_node_new(void *content, void (*del)(void *));
INTERN void		lst_node_del(t_clean **lst);
INTERN void		lst_node_del_clean(t_clean **lst, void *mem);
INTERN void		lst_list_clean(t_clean **head);
INTERN void		lst_add_back(t_clean **node, t_clean *new_node);

#endif
