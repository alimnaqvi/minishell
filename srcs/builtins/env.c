/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:06:55 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/16 15:58:43 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_to_env(char *var_name_val, t_minishell *minishell)
{
	char	**new_mini_env;
	int		old_num_vars;
	int		i;

	if (!var_name_val || !(minishell->mini_env))
		return ;
	old_num_vars = get_array_size(minishell->mini_env);
	new_mini_env = gc_malloc(sizeof(char *) * (old_num_vars + 2), minishell);
	i = 0;
	while (minishell->mini_env[i])
	{
		new_mini_env[i] = gc_ft_strdup(minishell->mini_env[i], minishell);
		gc_free(minishell->mini_env[i], minishell);
		i++;
	}
	new_mini_env[i] = var_name_val;
	i++;
	new_mini_env[i] = NULL;
	gc_free(minishell->mini_env, minishell);
	minishell->mini_env = new_mini_env;
}

void	update_env_var(char *var_name, char *var_val, t_minishell *minishell)
{
	int		i;
	char	*var_name_eqsign;
	char	*var_name_val;
	int		var_eq_len;

	if (!var_name || !var_val || !minishell->mini_env)
		return ;
	i = 0;
	var_name_eqsign = gc_ft_strjoin(var_name, "=", minishell);
	var_name_val = gc_ft_strjoin(var_name_eqsign, var_val, minishell);
	var_eq_len = ft_strlen(var_name_eqsign);
	while (minishell->mini_env[i])
	{
		if (!ft_strncmp(minishell->mini_env[i], var_name_eqsign, var_eq_len))
		{
			gc_free(minishell->mini_env[i], minishell);
			minishell->mini_env[i] = var_name_val;
			return (gc_free(var_name_eqsign, minishell));
		}
		i++;
	}
	append_to_env(var_name_val, minishell);
	return (gc_free(var_name_eqsign, minishell));
}

char	*get_env_var_value(char *var_name, t_minishell *minishell)
{
	int		i;
	char	*result;
	char	*var_name_eqsign;
	int		var_eq_len;

	i = 0;
	if (!(minishell->mini_env))
		return (NULL);
	var_name_eqsign = gc_ft_strjoin(var_name, "=", minishell);
	var_eq_len = ft_strlen(var_name_eqsign);
	while (minishell->mini_env[i])
	{
		if (!ft_strncmp(minishell->mini_env[i], var_name_eqsign, var_eq_len))
			break ;
		i++;
	}
	gc_free(var_name_eqsign, minishell);
	if (!(minishell->mini_env[i]))
		return (NULL);
	result = gc_ft_strdup(minishell->mini_env[i] + var_eq_len, minishell);
	return (result);
}

void	ft_env(t_minishell *minishell)
{
	int	i;

	if (!(minishell->mini_env))
		return (gc_exit(minishell, EXIT_FAILURE));
	i = 0;
	while (minishell->mini_env[i])
	{
		printf("%s\n", minishell->mini_env[i]);
		i++;
	}
	gc_exit(minishell, EXIT_SUCCESS);
}
