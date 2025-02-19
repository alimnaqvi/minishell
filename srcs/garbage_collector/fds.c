/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 20:26:16 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/16 16:12:23 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(void *ptr)
{
	if (ptr)
		close((int)(intptr_t)(ptr));
}

int	gc_open(char *file, int flags, mode_t mode, t_minishell *minishell)
{
	int		fd;
	t_list	*fd_node;

	fd = open(file, flags, mode);
	if (fd == -1)
	{
		perror(file);
		gc_exit(minishell, EXIT_FAILURE);
	}
	fd_node = ft_lstnew((void *)(intptr_t)fd);
	if (!fd_node)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		close(fd);
		gc_exit(minishell, EXIT_FAILURE);
	}
	ft_lstadd_front(&(minishell->garbage.open_fds), fd_node);
	return (fd);
}

void	gc_add_to_open_fds(int fd, t_minishell *minishell)
{
	t_list	*fd_node;

	if (fd == -1 || !minishell)
		return ;
	fd_node = ft_lstnew((void *)(intptr_t)fd);
	if (!fd_node)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		close(fd);
		gc_exit(minishell, EXIT_FAILURE);
	}
	ft_lstadd_front(&(minishell->garbage.open_fds), fd_node);
}

void	gc_close(int fd, t_minishell *minishell)
{
	t_list	*current;
	t_list	*prev;

	if (fd == -1 || !minishell)
		return ;
	current = minishell->garbage.open_fds;
	prev = NULL;
	while (current)
	{
		if ((int)(intptr_t)(current->content) == fd)
		{
			if (prev)
				prev->next = current->next;
			else
				minishell->garbage.open_fds = current->next;
			ft_lstdelone(current, ft_close);
			if (prev)
				current = prev->next;
			else
				current = minishell->garbage.open_fds;
			continue ;
		}
		prev = current;
		current = current->next;
	}
}

void	gc_close_all_open_fds(t_minishell *minishell)
{
	if (!minishell || !(minishell->garbage.open_fds))
		return ;
	ft_lstclear(&(minishell->garbage.open_fds), ft_close);
}
