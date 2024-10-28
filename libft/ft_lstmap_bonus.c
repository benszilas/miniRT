/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 16:18:03 by vvobis            #+#    #+#             */
/*   Updated: 2024/04/09 23:40:57 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*free_list(t_list **head, void (*del)(void *), void *fcontent)
{
	if (fcontent)
		del(fcontent);
	ft_lstclear(head, del);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*tmp;
	t_list	*head;
	void	*fcontent;

	if (!lst || !f || !del)
		return (NULL);
	head = NULL;
	while (lst)
	{
		fcontent = f(lst->content);
		if (!fcontent)
			return (free_list(&head, del, fcontent));
		tmp = ft_lstnew(fcontent);
		if (!tmp)
			return (free_list(&head, del, fcontent));
		ft_lstadd_back(&head, tmp);
		lst = lst->next;
	}
	return (head);
}
