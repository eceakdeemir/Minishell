/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:21:12 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 02:44:42 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	*memory_malloc(size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
		return (memory_exit());
	memory_add_new_block(res, size);
	return (res);
}

void	*memory_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;
	void			*result;
	size_t			i;

	result = malloc(size * count);
	if (!result)
		return (memory_exit());
	ptr = result;
	i = -1;
	while (++i < size * count)
		ptr[i] = '\0';
	memory_add_new_block(result, size * count);
	return (result);
}

void	memory_free(void)
{
	memory_clear_block(memory_add_new_block(NULL, 0)->head);
}

void	*memory_absorb(void *address)
{
	memory_add_new_block(address, 1);
	return (address);
}
