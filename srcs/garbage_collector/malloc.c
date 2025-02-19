/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:15:15 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/17 04:02:56 by rreimann         ###   ########.fr       */
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

static int	is_already_in_allocs(void *ptr, t_minishell *minishell)
{
	t_list	*current;

	current = minishell->garbage.allocs;
	while (current)
	{
		if (ptr == current->content)
			return (1);
		current = current->next;
	}
	return (0);
}

void	gc_add_to_allocs(void *ptr, t_minishell *minishell)
{
	t_list	*alloc_node;

	if (!ptr || !minishell)
		return ;
	if (is_already_in_allocs(ptr, minishell))
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

char	*gc_malloc_str(t_minishell *minishell, const char *str)
{
	size_t	str_len;
	char	*allocated_str;
	size_t	index;

	str_len = ft_strlen(str);
	allocated_str = gc_malloc(sizeof(char) * (str_len + 1), minishell);
	index = 0;
	while (index < str_len)
	{
		allocated_str[index] = str[index];
		index++;
	}
	allocated_str[index] = 0;
	return (allocated_str);
}
