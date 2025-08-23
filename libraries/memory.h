/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:21:17 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:25:23 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <stddef.h>

typedef struct s_memory_block
{
	void					*data;
	size_t					size;
	struct s_memory_block	**head;
	struct s_memory_block	*next;

}							t_memory_block;

void						*memory_malloc(size_t size);
void						*memory_calloc(size_t type, size_t size);
void						memory_free(void);
void						*memory_absorb(void *address);
t_memory_block				*memory_add_new_block(void *data, size_t size);
void						memory_clear_block(t_memory_block **head);
void						*memory_exit(void);
void						ft_exit(int status);
#endif // LIBMEM_H
