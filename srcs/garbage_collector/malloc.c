/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:15:15 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/12 16:28:27 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*gc_malloc(size_t size, t_minishell *minishell)
{
	void	*ptr;
	t_list	*alloc_node;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		gc_exit(minishell, EXIT_FAILURE);
	}
	alloc_node = ft_lstnew(ptr);
	if (!alloc_node)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		free(ptr);
		gc_exit(minishell, EXIT_FAILURE);
	}
	ft_lstadd_front(&(minishell->garbage.allocs), alloc_node);
	return (ptr);
}

void	gc_add_to_allocs(void *ptr, t_minishell *minishell)
{
	t_list	*alloc_node;

	if (!ptr || !minishell)
		return ;
	alloc_node = ft_lstnew(ptr);
	if (!alloc_node)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		gc_exit(minishell, EXIT_FAILURE);
	}
	ft_lstadd_front(&(minishell->garbage.allocs), alloc_node);
}

// 1. Allocate a new memory of given size
// 2. Copy the old stuff over to the new allocated memory
// 3. Free the old allocated memory
// 4. Return the new memory
void	*gc_realloc(size_t size, void *old_alloc, t_minishell *minishell)
{
	void	*new_alloc;

	new_alloc = gc_malloc(size, minishell);
	ft_memcpy(new_alloc, old_alloc, size);
	gc_free(old_alloc, minishell);
	return (new_alloc);
}
