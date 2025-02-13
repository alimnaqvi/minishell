/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:26:19 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/13 20:44:52 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **args, t_minishell *minishell)
{
	int newline;

	if (!args)
		return(gc_exit(minishell, EXIT_FAILURE));
	args++;
	if (!(*args))
		return (gc_exit(minishell, EXIT_SUCCESS));
	if (!ft_strncmp(*args, "-n", 3))
	{
		newline = 0;
		args++;
	}
	else
		newline = 1;
	while (*args)
	{
		ft_putstr_fd(*args, STDOUT_FILENO);
		if (*(args + 1))
			ft_putstr_fd(" ", STDOUT_FILENO);
		args++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	gc_exit(minishell, EXIT_SUCCESS);
}
