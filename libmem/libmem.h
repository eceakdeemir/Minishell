/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libmem.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:21:17 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 19:21:18 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef LIBMEM_H
# define LIBMEM_H

# include <stddef.h>

typedef struct s_mem_block
{
	void				*data;
	size_t				size;
	struct s_mem_block	**head;
	struct s_mem_block	*next;

}	t_mem_block;

void	*mem_malloc(size_t size);
void	*mem_calloc(size_t type, size_t size);
void	*mem_realloc(void *old_data, size_t new_size);
void	mem_free(void);
void	*mem_absorb(void *addr);

#endif // LIBMEM_H
