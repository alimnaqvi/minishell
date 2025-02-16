/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_grp_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:42:44 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/16 16:53:23 by anaqvi           ###   ########.fr       */
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
	if (last->out_fd == STDOUT_FILENO)
		last->out_fd = pipefd[1];
	return (0);
}

static int	count_args(t_minishell *minishell, int *i, t_cmd_grp *cmd_grp_node)
{
	int		arg_count;
	char	*op_err;

	arg_count = 0;
	while (minishell->tokenized[*i]
		&& ft_strncmp(minishell->tokenized[*i], "|", 2))
	{
		if (is_redir_opr(minishell->tokenized[*i])
			&& minishell->tokenized[*i + 1])
		{
			if (update_cmd_grp_fds(minishell, cmd_grp_node,
					minishell->tokenized[*i],
					minishell->tokenized[*i + 1]) == -1)
				return (-1);
			(*i) += 2;
			continue ;
		}
		op_err = token_is_unsupp_op(minishell->tokenized[*i]);
		if (op_err)
			return (put_specific_error(op_err, CTRL_OP_ERR),
				minishell->last_exit_status = 2, -1);
		arg_count++;
		(*i)++;
	}
	return (arg_count);
}

static char	**copy_cmd_args(t_minishell *minishell, int arg_count,
int cmd_start)
{
	char	**cmd_args;
	int		i;
	int		j;

	cmd_args = gc_malloc(sizeof(char *) * (arg_count + 1), minishell);
	i = cmd_start;
	j = 0;
	while (minishell->tokenized[i]
		&& ft_strncmp(minishell->tokenized[i], "|", 2))
	{
		if (is_redir_opr(minishell->tokenized[i])
			&& minishell->tokenized[i + 1])
		{
			i += 2;
			continue ;
		}
		cmd_args[j] = gc_ft_strdup(minishell->tokenized[i], minishell);
		i++;
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
	if (minishell->cmd_grp_strt)
	{
		if (create_pipe(minishell, cmd_grp_node) == -1)
			return (-1);
	}
	arg_count = count_args(minishell, i, cmd_grp_node);
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
