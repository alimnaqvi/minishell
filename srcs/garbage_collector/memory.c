/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:15:15 by anaqvi            #+#    #+#             */
/*   Updated: 2024/12/24 23:13:54 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	free_check_null(void *ptr)
{
	if (ptr)
		free(ptr);
}

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

void	gc_free(void *ptr, t_minishell *minishell)
{
	t_list	*current;
	t_list	*prev;

	if (!ptr || !minishell)
		return ;
	current = minishell->garbage.allocs;
	prev = NULL;
	while (current)
	{
		if (current->content == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				minishell->garbage.allocs = current->next;
			ft_lstdelone(current, free_check_null);
			if (prev)
				current = prev->next;
			else
				current = minishell->garbage.allocs;
			continue ;
		}
		prev = current;
		current = current->next;
	}
}
