/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:41:18 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/11 20:32:06 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_from_env(char *var_name, t_minishell *minishell)
{
	char	*var_name_eqsign;
	int		var_eq_len;
	int		i;

	if (!var_name || !(minishell->mini_env))
		return ;
	var_name_eqsign = gc_ft_strjoin(var_name, "=", minishell);
	var_eq_len = ft_strlen(var_name_eqsign);
	i = 0;
	while (minishell->mini_env[i])
	{
		if (!ft_strncmp(minishell->mini_env[i], var_name_eqsign, var_eq_len))
		{
			gc_free(minishell->mini_env[i], minishell);
			while (minishell->mini_env[i])
			{
				minishell->mini_env[i] = minishell->mini_env[i + 1];
				i++;
			}
			break ;
		}
		i++;
	}
	gc_free(var_name_eqsign, minishell);
}

void	ft_unset_child(char **args, t_minishell *minishell)
{
	int		i;

	if (!args || !(*args) || !(minishell->mini_env))
		return(gc_exit(minishell, EXIT_FAILURE));
	i = 1;
	if (!(args[i]))
		return (gc_exit(minishell, EXIT_SUCCESS));
	while (args[i])
	{
		remove_from_env(args[i], minishell);
		i++;
	}
	gc_exit(minishell, EXIT_SUCCESS);
}

void	ft_unset_parent(char **args, t_minishell *minishell)
{
	int		i;

	if (!args || !(*args) || !(minishell->mini_env))
		return(minishell->last_exit_status = 1, (void)0);
	i = 1;
	if (!(args[i]))
		return ;
	while (args[i])
	{
		remove_from_env(args[i], minishell);
		i++;
	}
}
