/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:12:05 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/14 17:55:29 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_cmd_grp	*init_cmd_grp_node(t_minishell *minishell)
{
	t_cmd_grp	*new_node;

	new_node = (t_cmd_grp *)gc_malloc(sizeof(t_cmd_grp), minishell);
	new_node->cmd_args = NULL;
	new_node->cmd_name = NULL;
	new_node->in_fd = STDIN_FILENO;
	new_node->out_fd = STDOUT_FILENO;
	new_node->previous = NULL;
	new_node->next = NULL;
	return (new_node);
}

static void	add_back_cmd_group(t_minishell *minishell, t_cmd_grp *cmd_grp_node)
{
	t_cmd_grp	*last;

	if (!minishell)
		return ;
	if (!(minishell->cmd_grp_strt))
		last = NULL;
	else
	{
		last = minishell->cmd_grp_strt;
		while (last->next)
			last = last->next;
	}
	if (!last)
		minishell->cmd_grp_strt = cmd_grp_node;
	else
	{
		cmd_grp_node->previous = last;
		last->next = cmd_grp_node;
	}
}

int	is_redir_opr(char *token)
{
	if (!ft_strncmp(token, "<", 2)
		|| !ft_strncmp(token, "<<", 3)
		|| !ft_strncmp(token, ">", 2)
		|| !ft_strncmp(token, ">>", 3))
	{
		return (1);
	}
	return (0);
}

static int	init_update_cmd_grp(t_minishell *minishell, int i,
t_cmd_grp *cmd_grp_node, char **tokens)
{
	while (tokens[i] && g_signal_received != SIGINT)
	{
		cmd_grp_node = init_cmd_grp_node(minishell);
		while (tokens[i] && g_signal_received != SIGINT
			&& ft_strncmp(minishell->tokenized[i], "|", 2))
		{
			if (is_redir_opr(tokens[i]))
			{
				if (update_cmd_grp_fds(minishell, cmd_grp_node,
						tokens[i], tokens[i + 1]) == -1)
					return (add_back_cmd_group(minishell, cmd_grp_node), -1);
				i += 2;
			}
			else
			{
				if (update_cmd_grp_cmds(minishell, &i, cmd_grp_node) == -1
					|| find_full_cmd_path(minishell, cmd_grp_node) == -1)
					return (add_back_cmd_group(minishell, cmd_grp_node), -1);
			}
		}
		add_back_cmd_group(minishell, cmd_grp_node);
		if (tokens[i])
			i++;
	}
	return (0);
}

int	parser(t_minishell *minishell)
{
	int			i;
	t_cmd_grp	*cmd_grp_node;
	char		**tokens;

	if (!minishell)
		return (-1);
	minishell->cmd_grp_strt = NULL;
	if (!(minishell->tokenized))
		return (-1);
	if (last_token_is_pipe(minishell->tokenized))
		return (-1);
	tokens = minishell->tokenized;
	cmd_grp_node = NULL;
	i = 0;
	return (init_update_cmd_grp(minishell, i, cmd_grp_node, tokens));
}
