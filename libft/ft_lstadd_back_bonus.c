/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:22:43 by ecakdemi          #+#    #+#             */
/*   Updated: 2024/10/28 15:31:00 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst)
		lst = &new;
	else if (!new)
		return ;
	else if (!*lst)
		*lst = new;
	else
	{
		ft_lstlast(*lst)->next = new;
	}
}
