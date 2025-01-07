/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:12:05 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/07 15:05:02 by anaqvi           ###   ########.fr       */
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

static void	read_from_heredoc(int fd, char *delimiter,
size_t len_delmtr, t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		gc_add_to_allocs(line, minishell);
		if (!line || !ft_strncmp(delimiter, line, len_delmtr + 1))
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

static int	handle_heredoc(char *delimiter, t_minishell *minishell)
{
	int	fd;

	if (!delimiter)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		return (-1);
	}
	fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("/tmp/minishell_heredoc");
		return (-1);
	}
	read_from_heredoc(fd, delimiter, ft_strlen(delimiter), minishell);
	close(fd);
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

static int update_cmd_grp_fds(t_minishell *minishell, t_cmd_grp *cmd_grp_node, char *s1, char *s2)
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
	return (cmd_grp_node->in_fd);
}

static int is_redir_opr(char *token)
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

static int	create_pipe(t_minishell *minishell, t_cmd_grp *cmd_grp_node)
{
	int	*pipefd;
	t_cmd_grp	*last;

	pipefd = gc_malloc(sizeof(int) * 2, minishell);
	if (pipe(pipefd) == -1)
		return(perror("minishell: pipe failed"), -1);
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

static int	count_args(t_minishell *minishell, int *i, t_cmd_grp *cmd_grp_node)
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
	return (0);
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
int cmd_start, t_cmd_grp *cmd_grp_node)
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

static int	update_cmd_grp_cmds(t_minishell *minishell, int *i, t_cmd_grp *cmd_grp_node)
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
	arg_count = count_args(minishell, i, cmd_grp_node);
	if (arg_count == -1 || arg_count == 0)
		return (-1);
	if (is_external_cmd(minishell->tokenized[cmd_start]))
		cmd_grp_node->cmd_type = EXTERNAL;
	else
		cmd_grp_node->cmd_type = BUILTIN;
	cmd_grp_node->cmd_name = ft_strdup(minishell->tokenized[cmd_start]);
	cmd_grp_node->cmd_args = copy_cmd_args(minishell, arg_count, cmd_start,
		cmd_grp_node);
	if (!(cmd_grp_node->cmd_name) || !(cmd_grp_node->cmd_args))
		return (-1);
	gc_add_to_allocs(cmd_grp_node->cmd_name, minishell);
	return (0);
}

static char	**get_paths(char **mini_env, t_minishell *minishell)
{
	char	**paths;
	int		i;

	if (!mini_env)
		return (NULL);
	while (*mini_env)
	{
		if (!ft_strncmp("PATH=", *mini_env, 5))
			break ;
		mini_env++;
	}
	if (!(*mini_env))
		return (NULL);
	paths = ft_split((*mini_env) + 5, ':');
	if (!paths)
		return (gc_exit(minishell, EXIT_FAILURE), NULL);
	gc_add_to_allocs(paths, minishell);
	i = 0;
	while (paths[i])
	{
		gc_add_to_allocs(paths[i], mini_env);
		i++;
	}
	return (paths);
}

static void	find_executable(t_cmd_grp *cmd_grp_node, char **paths, t_minishell *minishell)
{
	char	*current_path;
	size_t	len;

	while (*paths)
	{
		len = sizeof(char) * (ft_strlen(*paths) + ft_strlen(cmd_grp_node->cmd_name) + 2);
		current_path = gc_malloc(len, minishell);
		ft_strlcpy(current_path, *paths, len);
		ft_strlcat(current_path, "/", len);
		ft_strlcat(current_path, cmd_grp_node->cmd_name, len);
		if (access(current_path, X_OK) == 0)
			break ;
		gc_free(current_path, minishell);
		current_path = NULL;
		paths++;
	}
	if (!current_path)
		return ;
	gc_free(cmd_grp_node->cmd_name, minishell);
	cmd_grp_node->cmd_name = current_path;
	return ;
}

static int	find_full_cmd_path(t_minishell *minishell, t_cmd_grp *cmd_grp_node)
{
	char	**paths;

	if (cmd_grp_node->cmd_type == EXTERNAL)
	{
		if (!access(cmd_grp_node->cmd_name, X_OK))
			return (0);
		paths = get_paths(minishell->mini_env, minishell);
		find_executable(cmd_grp_node, paths, minishell);
	}
	return (0);
}

int	parser(t_minishell *minishell)
{
	int			i;
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
		while (minishell->tokenized[i] && ft_strncmp(minishell->tokenized[i], "|", 2))
		{
			if (is_redir_opr(minishell->tokenized[i]))
			{
				if (update_cmd_grp_fds(minishell, cmd_grp_node,
					minishell->tokenized[i], minishell->tokenized[i + 1]) == -1)
					return (gc_free(cmd_grp_node, minishell), -1);
				i += 2;
			}
			else
			{
				if (update_cmd_grp_cmds(minishell, &i, cmd_grp_node) == -1
					|| find_full_cmd_path(minishell, cmd_grp_node) == -1)
					return (gc_free(cmd_grp_node, minishell), -1);
			}
		}
		add_back_cmd_group(minishell, cmd_grp_node);
		i++;
	}
}
