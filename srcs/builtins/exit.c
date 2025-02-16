/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 19:26:56 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/16 16:07:59 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit_parent(char **args, t_minishell *minishell)
{
	int		argc;
	int		exit_status_arg;

	if (!args || !(*args))
		return (gc_exit(minishell, EXIT_FAILURE));
	argc = get_array_size(args);
	if (argc == 1)
	{
		printf("exit\n");
		gc_exit(minishell, minishell->last_exit_status);
	}
	if (ft_atoi_error(args[1], &exit_status_arg) == -1)
	{
		put_builtin_error("exit", args[1], "numeric argument required");
		printf("exit\n");
		gc_exit(minishell, 2);
	}
	if (argc > 2)
	{
		minishell->last_exit_status = EXIT_FAILURE;
		return (put_builtin_error("exit", "", "too many arguments"));
	}
	printf("exit\n");
	gc_exit(minishell, exit_status_arg);
}

void	ft_exit_child(char **args, t_minishell *minishell)
{
	int		argc;
	int		exit_status_arg;

	if (!args || !(*args))
		return (gc_exit(minishell, EXIT_FAILURE));
	argc = get_array_size(args);
	if (argc == 1)
		gc_exit(minishell, minishell->last_exit_status);
	if (ft_atoi_error(args[1], &exit_status_arg) == -1)
	{
		put_builtin_error("exit", args[1], "numeric argument required");
		return (gc_exit(minishell, EXIT_FAILURE));
	}
	if (argc > 2)
	{
		minishell->last_exit_status = EXIT_FAILURE;
		put_builtin_error("exit", "", "too many arguments");
		return (gc_exit(minishell, EXIT_FAILURE));
	}
	gc_exit(minishell, exit_status_arg);
}
