/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:39:04 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/15 17:57:29 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (shell_error("fork failed"), -1);
	else if (pid == 0)
	{
		set_signal_handler(HEREDOC);
		fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			shell_error("/tmp/minishell_heredoc");
			return (gc_exit(minishell, EXIT_FAILURE), -1);
		}
		read_from_heredoc(fd, delimiter, ft_strlen(delimiter), minishell);
		close(fd);
		gc_exit(minishell, EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	minishell->last_exit_status = get_exit_status(status);
	return (0);
}

int	handle_heredoc(char *delimiter, t_minishell *minishell)
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
		shell_error("/tmp/minishell_heredoc");
		return (-1);
	}
	unlink("/tmp/minishell_heredoc");
	gc_add_to_open_fds(fd, minishell);
	return (fd);
}
