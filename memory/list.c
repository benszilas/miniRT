/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:47:55 by victor            #+#    #+#             */
/*   Updated: 2024/09/13 10:16:10 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

INTERN t_clean	*lst_node_new(void *content, void (*del)(void *))
{
	t_clean	*new;

	if (!content || !del)
		return (NULL);
	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->content = content;
	new->clean = del;
	new->next = NULL;
	return (new);
}

INTERN void	lst_node_del(t_clean **lst)
{
	(*lst)->clean((*lst)->content);
	free(*lst);
	*lst = 0;
}

INTERN void	lst_node_del_clean(t_clean **lst, void *mem)
{
	t_clean	*tmp;
	t_clean	*head;

	if (!lst || !*lst)
		return ;
	head = *lst;
	tmp = *lst;
	while ((*lst) && (*lst)->next->content != mem)
		(*lst) = (*lst)->next;
	if (!*lst)
		return ;
	tmp = *lst;
	if ((*lst)->next && (*lst)->next->next)
	{
		tmp = (*lst)->next->next;
		lst_node_del(&(*lst)->next);
		(*lst)->next = tmp;
	}
	else if ((*lst)->next)
		lst_node_del(&(*lst)->next);
	else
		lst_node_del(lst);
	*lst = head;
}

INTERN void	lst_list_clean(t_clean **head)
{
	t_clean	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		lst_node_del(head);
		*head = tmp;
	}
	free(*head);
}

INTERN void	lst_add_back(t_clean **node, t_clean *new)
{
	t_clean	*tmp;

	if (!new)
	{
		perror("malloc");
	}
	if (*node)
	{
		tmp = *node;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*node = new;
}
