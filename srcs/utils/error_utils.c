/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:11:13 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/11 17:14:17 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_error(const char *msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	perror(msg);
}

void	put_specific_error(char *problem, char *msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	ft_putstr_fd(problem, STDERR_FILENO);
	write(STDERR_FILENO, ": ", 2);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	put_builtin_error(char *builtin_name, char *problem, char *msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	ft_putstr_fd(builtin_name, STDERR_FILENO);
	write(STDERR_FILENO, ": ", 2);
	if (*problem)
	{
		ft_putstr_fd(problem, STDERR_FILENO);
		write(STDERR_FILENO, ": ", 2);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}
