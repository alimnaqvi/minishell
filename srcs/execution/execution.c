/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:24:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/14 18:18:26 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	get_count_cmd_grps(t_cmd_grp *cmd_grp_strt)
// {
// 	int	count;

// 	count = 0;
// 	while (cmd_grp_strt)
// 	{
// 		count++;
// 		cmd_grp_strt = cmd_grp_strt->next;
// 	}
// 	return (count);
// }

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
	return (0);
}

static int	execve_exit_code(char *cmd_name)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		return (127);
	}
	else if (errno == EACCES)
	{
		shell_error(cmd_name);
		return (126);
	}
	perror(cmd_name);
	return(EXIT_FAILURE);
}

static void execute_builtin(t_cmd_grp *cur_node, t_minishell *minishell)
{
	// if (!ft_strncmp(cur_node->cmd_name, "echo", 5))
	// 	ft_echo(cur_node->cmd_args[1]);
	// if (!ft_strncmp(cur_node->cmd_name, "cd", 3))
	// 	ft_cd(cur_node->cmd_args[1]);
	// if (!ft_strncmp(cur_node->cmd_name, "pwd", 4))
	// 	ft_pwd(cur_node->cmd_args[1]);
	// if (!ft_strncmp(cur_node->cmd_name, "export", 7))
	// 	ft_export(cur_node->cmd_args[1]);
	// if (!ft_strncmp(cur_node->cmd_name, "unset", 6))
	// 	ft_unset(cur_node->cmd_args[1]);
	// if (!ft_strncmp(cur_node->cmd_name, "env", 4))
	// 	ft_env(cur_node->cmd_args[1]);
	if (!ft_strncmp(cur_node->cmd_name, "exit", 5))
		gc_exit(minishell, EXIT_SUCCESS);
}

static void execute_ith_cmd_grp(int i, t_minishell *minishell)
{
	int			cur_index;
	t_cmd_grp	*cur_node;
	int			exit_code;

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
			if (execve(cur_node->cmd_name, cur_node->cmd_args,
				minishell->mini_env) == -1)
			{
				exit_code = execve_exit_code(cur_node->cmd_name);
				return (gc_exit(minishell, exit_code));
			}
		}
		cur_node = cur_node->next;
		cur_index++;
	}
}

static int	validate_count_cmd_grps(t_minishell *minishell)
{
	t_cmd_grp	*cur_node;
	int			count_cmd_grps;

	if (!minishell || !(minishell->cmd_grp_strt))
		return (-1);
	count_cmd_grps = 0;
	cur_node = minishell->cmd_grp_strt;
	while (cur_node)
	{
		if (!(cur_node->cmd_name))
			return (-1);
		if (!(cur_node->cmd_args)
				|| !(minishell->cmd_grp_strt->cmd_args[0]))
			return (-1);
		count_cmd_grps++;
		cur_node = cur_node->next;
	}
	if (!ft_strncmp("exit", minishell->cmd_grp_strt->cmd_name, 5)
			&& count_cmd_grps == 1)
		gc_exit(minishell, EXIT_SUCCESS);
	return (count_cmd_grps);
}

void	execution(t_minishell *minishell)
{
	pid_t	*pids;
	int		count_cmd_grps;
	int		i;

	count_cmd_grps = validate_count_cmd_grps(minishell);
	if (count_cmd_grps == -1)
		return ;
	pids = gc_malloc(sizeof(pid_t) * count_cmd_grps, minishell);
	i = 0;
	while (i < count_cmd_grps)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			shell_error("fork failed");
			return ;
		}
		if (pids[i] == 0)
		{
			// execute ith cmd_grp and exit the process
			execute_ith_cmd_grp(i, minishell);
		}
		i++;
	}
	// wait for children and update last_exit_status
	int		*statuses;
	int		last_status;
	int		signal_num;
	int		last_valid_index;

	statuses = gc_malloc(sizeof(int) * count_cmd_grps, minishell);
	i = 0;
	last_valid_index = -1;
	while (i < count_cmd_grps)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &statuses[i], 0);
			last_valid_index = i;
		}
		i++;
	}
	if (last_valid_index == -1)
	{
		minishell->last_exit_status = 1;
		return ;
	}
	last_status = statuses[last_valid_index];
	if (WIFEXITED(last_status))
		minishell->last_exit_status = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
	{
		signal_num = WTERMSIG(last_status);
		minishell->last_exit_status = 128 + signal_num;
		if (signal_num == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (signal_num == SIGQUIT)
			write(STDERR_FILENO, "Quit\n", 5);
	}
	else
		minishell->last_exit_status = 1;
	printf("last_exit_status: %i\n", minishell->last_exit_status); // for testing
}