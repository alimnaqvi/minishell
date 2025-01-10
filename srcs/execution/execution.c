/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:24:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/10 18:19:12 by anaqvi           ###   ########.fr       */
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
		}
		i++;
	}
	// wait for children and update last_exit_status
}