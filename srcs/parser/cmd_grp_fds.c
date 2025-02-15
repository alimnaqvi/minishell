/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_grp_fds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:31:11 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/15 13:30:22 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_infile(char *infile, t_minishell *minishell)
{
	int	fd;

	if (!infile)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		return (-1);
	}
	if (access(infile, F_OK | R_OK) == -1)
		shell_error(infile);
	fd = open(infile, O_RDONLY);
	if (fd != -1)
		gc_add_to_open_fds(fd, minishell);
	return (fd);
}

static int	handle_outfile(char *outfile, int flag, t_minishell *minishell)
{
	int	fd;

	if (!outfile)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		return (-1);
	}
	if (access(outfile, F_OK) != -1)
	{
		if (access(outfile, W_OK) == -1)
		{
			shell_error(outfile);
			return (-1);
		}
	}
	fd = open(outfile, O_WRONLY | O_CREAT | flag, 0644);
	if (fd == -1)
	{
		shell_error(outfile);
		return (-1);
	}
	gc_add_to_open_fds(fd, minishell);
	return (fd);
}

int	update_cmd_grp_fds(t_minishell *minishell, t_cmd_grp *cmd_grp_node,
char *s1, char *s2)
{
	if (!minishell || !cmd_grp_node || !s1)
		return (-1);
	if (!ft_strncmp(s1, "<", 2) && cmd_grp_node->in_fd != -1
		&& cmd_grp_node->out_fd != -1)
		cmd_grp_node->in_fd = handle_infile(s2, minishell);
	else if (!ft_strncmp(s1, "<<", 3))
		cmd_grp_node->in_fd = handle_heredoc(s2, minishell);
	else if (!ft_strncmp(s1, ">", 2) && cmd_grp_node->in_fd != -1
		&& cmd_grp_node->out_fd != -1)
		cmd_grp_node->out_fd = handle_outfile(s2, O_TRUNC, minishell);
	else if (!ft_strncmp(s1, ">>", 3) && cmd_grp_node->in_fd != -1
		&& cmd_grp_node->out_fd != -1)
		cmd_grp_node->out_fd = handle_outfile(s2, O_APPEND, minishell);
	if (g_signal_received == SIGINT)
		return (-1);
	return (0);
}
