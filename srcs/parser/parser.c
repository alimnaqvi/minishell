/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:12:05 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/05 18:50:09 by anaqvi           ###   ########.fr       */
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

static int	handle_infile(char *infile, t_minishell *minishell)
{
	int	fd;

	if (!infile)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		return (-1);
	}
	if (access(infile, F_OK | R_OK) == -1)
	{
		perror(infile);
		infile = "/dev/null";
	}
	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		perror(infile);
		return (-1);
	}
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
			perror(outfile);
			return (-1);
		}
	}
	fd = open(outfile, O_WRONLY | O_CREAT | flag, 0644);
	if (fd == -1)
	{
		perror(outfile);
		return (-1);
	}
	gc_add_to_open_fds(fd, minishell);
	return (fd);
}

static int update_cmd_grp_fds(t_minishell *minishell, t_cmd_grp *cmd_grp_node, char *s1, char *s2)
{
	if (!minishell || !cmd_grp_node || !s1)
		return (-1);
	if (ft_strncmp(s1, "<", 2))
		cmd_grp_node->in_fd = handle_infile(s2, minishell);
	else if (ft_strncmp(s1, "<<", 3))
	{
		// handle heredoc
	}
	else if (ft_strncmp(s1, ">", 2))
		cmd_grp_node->out_fd = handle_outfile(s2, O_TRUNC, minishell);
	else if (ft_strncmp(s1, ">>", 3))
		cmd_grp_node->out_fd = handle_outfile(s2, O_APPEND, minishell);
	return (cmd_grp_node->in_fd);
}

int	parser(t_minishell *minishell)
{
	int			i;
	// int			in_cmd_grp;
	t_cmd_grp	*cmd_grp_node;

	if (!minishell)
		return (-1);
	minishell->cmd_grp_strt = NULL;
	if (!(minishell->tokenized))
		return (-1);
	i = 0;
	while (minishell->tokenized[i])
	{
		cmd_grp_node = init_cmd_grp_node(minishell);
		while (minishell->tokenized[i] && !ft_strncmp(minishell->tokenized[i], "|", 2))
		{
			if (is_redir_opr(minishell->tokenized[i]))
			{
				if (update_cmd_grp_fds(minishell, cmd_grp_node,
					minishell->tokenized[i], minishell->tokenized[i + 1]) == -1)
					return (free(cmd_grp_node), -1);
				i += 2;
			}
			else
				if (update_cmd_grp_cmds(minishell, &i) == -1)
					return (free(cmd_grp_node), -1);
		}
		add_back_cmd_group(minishell, cmd_grp_node);
		i++;
	}
}
