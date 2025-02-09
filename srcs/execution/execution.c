/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:24:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/09 21:06:01 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_unsupported_operator(char *cmd_name)
{
	if (!cmd_name)
		return (1);
	if (!ft_strncmp(cmd_name, "&&", 3) || !ft_strncmp(cmd_name, "||", 3)
		|| !ft_strncmp(cmd_name, "\n", 2))
		return (printf("returning at 21\n"), 1);
	if (ft_strnstr(cmd_name, "&&", 2) || ft_strnstr(cmd_name, "||", 2)
		|| ft_strnstr(cmd_name, "\n", 1))
		return (1);
	return (0);
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
		if (!(cur_node->cmd_name) || !(cur_node->cmd_args)
			|| !(minishell->cmd_grp_strt->cmd_args[0]))
			return (ft_putendl_fd("minishell: syntax error", 2),
				minishell->last_exit_status = 2, -1);
		if (has_unsupported_operator(cur_node->cmd_name))
			return (ft_putendl_fd("minishell: &&, ||, and newlines between "
					"commands are not supported by minishell :(", 2),
				minishell->last_exit_status = 2, -1);
		count_cmd_grps++;
		cur_node = cur_node->next;
	}
	if (!ft_strncmp("exit", minishell->cmd_grp_strt->cmd_name, 5)
		&& count_cmd_grps == 1)
		return (ft_exit_parent(minishell->cmd_grp_strt->cmd_args, minishell), -1);
	if (!ft_strncmp("cd", minishell->cmd_grp_strt->cmd_name, 3)
		&& count_cmd_grps == 1)
		return (ft_cd_parent(minishell->cmd_grp_strt->cmd_args, minishell), -1);
	return (count_cmd_grps);
}

/*Wait for child processes and update `minishell.last_exit_status`.*/
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
			printf("Execution child %i exited with status %i\n", pids[i], get_exit_status(statuses[i])); // for testing
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
	minishell->last_exit_status = get_exit_status(last_status);
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
	ft_lstclear(&(minishell->garbage.open_fds), ft_close);
	wait_children(pids, count_cmd_grps, minishell);
}
