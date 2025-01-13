/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:24:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/13 18:10:22 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_count_cmd_grps(t_cmd_grp *cmd_grp_strt)
{
	int	count;

	count = 0;
	while (cmd_grp_strt)
	{
		count++;
		cmd_grp_strt = cmd_grp_strt->next;
	}
	return (count);
}

static int	update_fds(t_cmd_grp *cur_node, t_minishell *minishell)
{
	if (!cur_node)
		return (-1);
	if (cur_node->in_fd != STDIN_FILENO)
	{
		if (dup2(cur_node->in_fd, STDIN_FILENO) == -1)
			return (perror("dup2 failed"), -1);
		gc_close(cur_node->in_fd, minishell);
	}
	if (cur_node->out_fd != STDOUT_FILENO)
	{
		if (dup2(cur_node->out_fd, STDOUT_FILENO) == -1)
			return (perror("dup2 failed"), -1);
		gc_close(cur_node->out_fd, minishell);
	}
	return (0);
}

static int	reset_signal_behavior()
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (-1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (-1);
	}
	return (0);
}

static void execute_ith_cmd_grp(int i, t_minishell *minishell)
{
	int			cur_index;
	t_cmd_grp	*cur_node;

	cur_index = 0;
	cur_node = minishell->cmd_grp_strt;
	while (cur_node)
	{
		if (cur_index == i)
		{
			if (reset_signal_behavior() == -1)
				return (gc_exit(minishell, EXIT_FAILURE));
			else if (update_fds(cur_node, minishell) == -1)
				return (gc_exit(minishell, EXIT_FAILURE));
			else if (execve(cur_node->cmd_name, cur_node->cmd_args,
				minishell->mini_env) == -1)
			{
				perror(cur_node->cmd_name);
				return (gc_exit(minishell, EXIT_FAILURE));
			}
		}
		cur_node = cur_node->next;
		cur_index++;
	}
}

void	execution(t_minishell *minishell)
{
	pid_t	*pids;
	int		count_cmd_grps;
	int		i;

	count_cmd_grps = get_count_cmd_grps(minishell->cmd_grp_strt);
	pids = gc_malloc(sizeof(pid_t) * count_cmd_grps, minishell);
	i = 0;
	while (i < count_cmd_grps)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork failed");
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
}