/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:21:25 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/09 21:09:52 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_fds(t_cmd_grp *cur_node, t_minishell *minishell)
{
	if (!cur_node)
		return (-1);
	if (cur_node->in_fd != STDIN_FILENO)
	{
		if (cur_node->in_fd == -1)
			return (-1);
		if (dup2(cur_node->in_fd, STDIN_FILENO) == -1)
			return (shell_error("dup2 failed"), -1);
		gc_close(cur_node->in_fd, minishell);
	}
	if (cur_node->out_fd != STDOUT_FILENO)
	{
		if (cur_node->out_fd == -1)
			return (-1);
		if (dup2(cur_node->out_fd, STDOUT_FILENO) == -1)
			return (shell_error("dup2 failed"), -1);
		gc_close(cur_node->out_fd, minishell);
	}
	gc_close_all_open_fds(minishell);
	return (0);
}

static int	execve_exit_code(char *cmd_name)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		return (127);
	}
	else if (errno == EACCES)
	{
		shell_error(cmd_name);
		return (126);
	}
	shell_error(cmd_name);
	return (EXIT_FAILURE);
}

static void	execute_builtin(t_cmd_grp *cur_node, t_minishell *minishell)
{
	if (!ft_strncmp(cur_node->cmd_name, "echo", 5))
		ft_echo(cur_node->cmd_args, minishell);
	if (!ft_strncmp(cur_node->cmd_name, "cd", 3))
		ft_cd_child(cur_node->cmd_args, minishell);
	if (!ft_strncmp(cur_node->cmd_name, "pwd", 4))
		ft_pwd(minishell);
	// if (!ft_strncmp(cur_node->cmd_name, "export", 7))
	// 	ft_export(cur_node->cmd_args, minishell);
	// if (!ft_strncmp(cur_node->cmd_name, "unset", 6))
	// 	ft_unset(cur_node->cmd_args, minishell);
	// if (!ft_strncmp(cur_node->cmd_name, "env", 4))
	// 	ft_env(minishell);
	if (!ft_strncmp(cur_node->cmd_name, "exit", 5))
		ft_exit_child(cur_node->cmd_args, minishell);
	if (!ft_strncmp(cur_node->cmd_name, "exit", 5)) // delete
		gc_exit(minishell, EXIT_SUCCESS);
}

void	execute_ith_cmd_grp(int i, t_minishell *minishell)
{
	int			cur_index;
	t_cmd_grp	*cur_node;

	cur_index = 0;
	cur_node = minishell->cmd_grp_strt;
	while (cur_node)
	{
		if (cur_index == i)
		{
			if (set_signal_handler(CHILD) == -1)
				return (gc_exit(minishell, EXIT_FAILURE));
			if (update_fds(cur_node, minishell) == -1)
				return (gc_exit(minishell, EXIT_FAILURE));
			if (cur_node->cmd_type == BUILTIN)
				return (execute_builtin(cur_node, minishell));
			else if (execve(cur_node->cmd_name, cur_node->cmd_args,
					minishell->mini_env) == -1)
				return (gc_exit(minishell,
						execve_exit_code(cur_node->cmd_name)));
		}
		cur_node = cur_node->next;
		cur_index++;
	}
	gc_exit(minishell, EXIT_FAILURE);
}
