/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:15:15 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/09 15:15:27 by anaqvi           ###   ########.fr       */
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
