/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:45:31 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/09 13:31:51 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	count_args(char **args) // possibly not needed
// {
// 	int count;

// 	count = 0;
// 	while (*args)
// 	{
// 		count++;
// 		args++;
// 	}
// 	return (count);
// }

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
		printf("%s", *args);
		if (*(args + 1))
			printf(" ");
		args++;
	}
	if (newline)
		printf("\n");
	gc_exit(minishell, EXIT_SUCCESS);
}
