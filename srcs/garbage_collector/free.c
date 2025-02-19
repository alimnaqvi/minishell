/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:57:47 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/18 20:58:00 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	gc_free_2d_char_arr(char **arr, t_minishell *minishell)
{
	int	i;

	if (!arr || !minishell)
		return ;
	i = 0;
	while (arr[i])
	{
		gc_free(arr[i], minishell);
		i++;
	}
	gc_free(arr, minishell);
}

void	gc_free_cmd_grps(t_minishell *minishell)
{
	t_cmd_grp	*current;
	t_cmd_grp	*to_free_node;

	if (!minishell || !(minishell->cmd_grp_strt))
		return ;
	current = minishell->cmd_grp_strt;
	while (current)
	{
		gc_free(current->cmd_name, minishell);
		gc_free_2d_char_arr(current->cmd_args, minishell);
		gc_close(current->in_fd, minishell);
		gc_close(current->out_fd, minishell);
		to_free_node = current;
		current = current->next;
		gc_free(to_free_node, minishell);
	}
	minishell->cmd_grp_strt = NULL;
}

void	free_check_null(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	gc_exit(t_minishell *minishell, int exit_status)
{
	restore_terminal_settings(minishell);
	ft_lstclear(&(minishell->garbage.allocs), free_check_null);
	ft_lstclear(&(minishell->garbage.open_fds), ft_close);
	exit(exit_status);
}
