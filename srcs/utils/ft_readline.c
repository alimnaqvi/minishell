/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:18:03 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/18 19:16:49 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*readline, check NULL, gc_add_to_allocs, add_history*/

// Read the input from the command line
void	ft_readline(t_minishell *minishell)
{
	minishell->input = readline("minishell$ ");
	if (minishell->input == NULL)
		return (ft_exit_parent((char *[]){"exit", NULL}, minishell));
	add_history(minishell->input);
	gc_add_to_allocs(minishell->input, minishell);
}
