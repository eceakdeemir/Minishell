/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:20:47 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 19:20:48 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef MEM_UTILS_H
# define MEM_UTILS_H

# include "../libmem.h"

t_mem_block	*mem_add_new_block(void *data, size_t size);
void		mem_clear_block(t_mem_block **head);
size_t		mem_find_size(void *data);
void		*mem_exit(void);
void	    ft_exit(int status);

#endif // MEM_UTILS_H
