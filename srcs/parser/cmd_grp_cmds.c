/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_grp_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:42:44 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/09 15:22:48 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe(t_minishell *minishell, t_cmd_grp *cmd_grp_node)
{
	int			pipefd[2];
	t_cmd_grp	*last;

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe failed"), -1);
	gc_add_to_open_fds(pipefd[0], minishell);
	gc_add_to_open_fds(pipefd[1], minishell);
	if (!cmd_grp_node)
		return (-1);
	cmd_grp_node->in_fd = pipefd[0];
	if (!minishell)
		return (-1);
	if (!(minishell->cmd_grp_strt))
		return (ft_putendl_fd("minishell: syntax error", STDERR_FILENO), -1);
	else
	{
		last = minishell->cmd_grp_strt;
		while (last->next)
			last = last->next;
	}
	last->out_fd = pipefd[1];
	return (0);
}

static int	count_args(t_minishell *minishell, int *i)
{
	int	arg_count;

	arg_count = 0;
	while (minishell->tokenized[*i]
		&& ft_strncmp(minishell->tokenized[*i], "|", 2)
		&& !is_redir_opr(minishell->tokenized[*i]))
	{
		if (!ft_strncmp(minishell->tokenized[*i], "||", 3)
			|| !ft_strncmp(minishell->tokenized[*i], "&&", 3))
		{
			ft_putendl_fd("&& and || are not supported by minishell :(", 2);
			return (-1);
		}
		arg_count++;
		(*i)++;
	}
	return (arg_count);
}

static int	is_external_cmd(char *cmd_name)
{
	if (!ft_strncmp(cmd_name, "echo", 5)
		|| !ft_strncmp(cmd_name, "cd", 3)
		|| !ft_strncmp(cmd_name, "pwd", 4)
		|| !ft_strncmp(cmd_name, "export", 7)
		|| !ft_strncmp(cmd_name, "unset", 6)
		|| !ft_strncmp(cmd_name, "env", 4)
		|| !ft_strncmp(cmd_name, "exit", 5))
	{
		return (0);
	}
	return (1);
}

static char	**copy_cmd_args(t_minishell *minishell, int arg_count,
int cmd_start)
{
	char	**cmd_args;
	int		j;

	cmd_args = gc_malloc(sizeof(char *) * (arg_count + 1), minishell);
	j = 0;
	while (j < arg_count)
	{
		cmd_args[j] = ft_strdup(minishell->tokenized[cmd_start + j]);
		if (!cmd_args[j])
			return (gc_exit(minishell, EXIT_FAILURE), NULL);
		gc_add_to_allocs(cmd_args[j], minishell);
		j++;
	}
	cmd_args[j] = NULL;
	return (cmd_args);
}

int	update_cmd_grp_cmds(t_minishell *minishell, int *i, t_cmd_grp *cmd_grp_node)
{
	int		arg_count;
	int		cmd_start;

	if (!i)
		return (-1);
	cmd_start = *i;
	if (*i != 0 && !ft_strncmp(minishell->tokenized[(*i) - 1], "|", 2))
	{
		if (create_pipe(minishell, cmd_grp_node) == -1)
			return (-1);
	}
	arg_count = count_args(minishell, i);
	if (arg_count == -1 || arg_count == 0)
		return (-1);
	if (is_external_cmd(minishell->tokenized[cmd_start]))
		cmd_grp_node->cmd_type = EXTERNAL;
	else
		cmd_grp_node->cmd_type = BUILTIN;
	cmd_grp_node->cmd_name = ft_strdup(minishell->tokenized[cmd_start]);
	cmd_grp_node->cmd_args = copy_cmd_args(minishell, arg_count, cmd_start);
	if (!(cmd_grp_node->cmd_name) || !(cmd_grp_node->cmd_args))
		return (-1);
	gc_add_to_allocs(cmd_grp_node->cmd_name, minishell);
	return (0);
}
