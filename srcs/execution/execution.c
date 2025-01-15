/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:24:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/15 17:09:03 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	get_last_exit_status(int last_status)
{
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (1);
}

/*Wait for child processes and update `minishell.last_exit_status`*/
static void	wait_children(pid_t *pids, int count_cmd_grps,
t_minishell *minishell)
{
	int		*statuses;
	int		last_status;
	int		last_valid_index;
	int		i;

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
	minishell->last_exit_status = get_last_exit_status(last_status);
	printf("last_exit_status: %i\n", minishell->last_exit_status); // for testing
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
			execute_ith_cmd_grp(i, minishell);
		}
		i++;
	}
	wait_children(pids, count_cmd_grps, minishell);
}
