/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_grp_fds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:31:11 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/13 18:30:34 by anaqvi           ###   ########.fr       */
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

static void	read_from_heredoc(int fd, char *delimiter,
size_t len_delmtr, t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		gc_add_to_allocs(line, minishell);
		if (!line || !ft_strncmp(delimiter, line, len_delmtr + 1)
			|| g_signal_received == SIGINT)
		{
			write(fd, "\n", 1);
			if (!line)
				printf("minishell: warning: here-document delimited "
					"by end-of-file (wanted `%s')\n", delimiter);
			gc_free(line, minishell);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		gc_free(line, minishell);
	}
}

static int	fork_for_heredoc(char *delimiter, t_minishell *minishell)
{
	int		fd;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return(perror("fork failed"), -1);
	else if (pid == 0)
	{
		fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("/tmp/minishell_heredoc");
			return (gc_exit(minishell, EXIT_FAILURE), -1);
		}
		read_from_heredoc(fd, delimiter, ft_strlen(delimiter), minishell);
		close(fd);
		gc_exit(minishell, EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			minishell->last_exit_status = WEXITSTATUS(status);
			return (0);
		}
		else if (WIFSIGNALED(status))
		{
			minishell->last_exit_status = 128 + WTERMSIG(status);
		}
		else
			minishell->last_exit_status = 1;
	}
	return (0);
}

static int	handle_heredoc(char *delimiter, t_minishell *minishell)
{
	int	fd;

	if (!delimiter)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		return (-1);
	}
	fork_for_heredoc(delimiter, minishell);
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (fd == -1)
	{
		perror("/tmp/minishell_heredoc");
		return (-1);
	}
	unlink("/tmp/minishell_heredoc");
	gc_add_to_open_fds(fd, minishell);
	return (fd);
}

int	update_cmd_grp_fds(t_minishell *minishell, t_cmd_grp *cmd_grp_node,
char *s1, char *s2)
{
	if (!minishell || !cmd_grp_node || !s1)
		return (-1);
	if (!ft_strncmp(s1, "<", 2))
		cmd_grp_node->in_fd = handle_infile(s2, minishell);
	else if (!ft_strncmp(s1, "<<", 3))
		cmd_grp_node->in_fd = handle_heredoc(s2, minishell);
	else if (!ft_strncmp(s1, ">", 2))
		cmd_grp_node->out_fd = handle_outfile(s2, O_TRUNC, minishell);
	else if (!ft_strncmp(s1, ">>", 3))
		cmd_grp_node->out_fd = handle_outfile(s2, O_APPEND, minishell);
	if (cmd_grp_node->in_fd == -1 || cmd_grp_node->out_fd == -1 || g_signal_received == SIGINT)
		return (-1);
	return (0);
}
