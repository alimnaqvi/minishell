/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 19:26:56 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/09 20:44:26 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit_parent(char **args, t_minishell *minishell)
{
	int		argc;
	int		exit_status_arg;

	if (!args || !(*args))
		return(gc_exit(minishell, EXIT_FAILURE));
	argc = get_array_size(args);
	if (argc > 2)
	{
		minishell->last_exit_status = EXIT_FAILURE;
		return (ft_putendl_fd("exit: too many arguments", STDERR_FILENO));
	}
	if (argc == 1)
	{
		printf("exit\n");
		gc_exit(minishell, minishell->last_exit_status);
	}
	if (ft_atoi_error(args[1], &exit_status_arg) == -1)
		return (ft_putendl_fd("exit: numeric argument required", 2));
	printf("exit\n");
	gc_exit(minishell, exit_status_arg);
}

void ft_exit_child(char **args, t_minishell *minishell)
{
	int		argc;
	int		exit_status_arg;

	if (!args || !(*args))
		return(gc_exit(minishell, EXIT_FAILURE));
	argc = get_array_size(args);
	if (argc > 2)
	{
		minishell->last_exit_status = EXIT_FAILURE;
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		return (gc_exit(minishell, EXIT_FAILURE));
	}
	if (argc == 1)
	{
		printf("exit\n");
		gc_exit(minishell, minishell->last_exit_status);
	}
	if (ft_atoi_error(args[1], &exit_status_arg) == -1)
	{
		ft_putendl_fd("exit: numeric argument required", 2);
		return (gc_exit(minishell, EXIT_FAILURE));
	}
	printf("exit\n");
	gc_exit(minishell, exit_status_arg);
}
