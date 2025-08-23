/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:21:04 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:25:32 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	*memory_exit(void)
{
	const char	*error;
	int			i;

	error = "memory: Out of memory\n";
	i = -1;
	while (error[++i])
		write(STDERR_FILENO, &error[i], 1);
	memory_free();
	exit(EXIT_FAILURE);
	return (NULL);
}

t_memory_block	*memory_add_new_block(void *data, size_t size)
{
	static t_memory_block	*head;
	t_memory_block			*new;
	t_memory_block			*node;

	new = malloc(sizeof(t_memory_block));
	if (!new)
		return (memory_exit());
	new->data = data;
	new->size = size;
	new->head = &head;
	new->next = NULL;
	if (!head)
		head = new;
	else
	{
		node = head;
		while (node->next)
			node = node->next;
		node->next = new;
	}
	return (new);
}

void	memory_clear_block(t_memory_block **head)
{
	t_memory_block	*temp;
	t_memory_block	*node;

	if (!(*head))
		return ;
	node = *head;
	while (node)
	{
		temp = node->next;
		if (node->data)
		{
			free(node->data);
			node->data = NULL;
		}
		free(node);
		node = temp;
	}
	*head = NULL;
}

size_t	mem_find_size(void *data)
{
	t_memory_block	*node;
	size_t			res;

	node = *(memory_add_new_block(NULL, 0)->head);
	while (1)
	{
		if (node->data == data)
			res = node->size;
		if (node->next)
			node = node->next;
		else
			break ;
	}
	return (res);
}

void	ft_exit(int status)
{
	memory_free();
	exit(status);
}
