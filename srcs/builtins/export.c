/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:40:06 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/11 19:16:37 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	display_export(t_minishell *minishell)
{
	int		i;
	char	*var_name;
	char	*var_val;

	i = 0;
	while (minishell->mini_env[i])
	{
		var_name = substr_before_char(minishell->mini_env[i], '=', minishell);
		var_val = substr_after_char(minishell->mini_env[i], '=', minishell);
		printf("declare -x %s=\"%s\"\n", var_name, var_val);
		i++;
	}
}

static int	is_invalid_id(char *arg)
{
	if (!ft_isalpha(*arg) && *arg != '_')
		return (1);
	while (*arg && *arg != '=')
	{
		if (!ft_isalnum(*arg) && *arg != '_')
			return (1);
		arg++;
	}
	return (0);
}

void ft_export_child(char **args, t_minishell *minishell)
{
	int		i;
	char	*var_name;
	char	*var_val;

	if (!args || !(*args) || !(minishell->mini_env))
		return(gc_exit(minishell, EXIT_FAILURE));
	i = 1;
	if (!(args[i]))
		return (display_export(minishell), gc_exit(minishell, EXIT_SUCCESS));
	while (args[i])
	{
		if (is_invalid_id(args[i]))
			put_builtin_error("export", args[i], "not a valid identifier");
		else if (!ft_strchr(args[i], '='))
			put_builtin_error("export", args[i], MARK_EXP_ERR);
		else
		{
			var_name = substr_before_char(args[i], '=', minishell);
			var_val = substr_after_char(args[i], '=', minishell);
			update_env_var(var_name, var_val, minishell);
		}
		i++;
	}
	gc_exit(minishell, EXIT_SUCCESS);
}

void ft_export_parent(char **args, t_minishell *minishell)
{
	int		i;
	char	*var_name;
	char	*var_val;

	if (!args || !(*args) || !(minishell->mini_env))
		return(minishell->last_exit_status = 1, (void)0);
	i = 1;
	if (!(args[i]))
		return (display_export(minishell));
	while (args[i])
	{
		if (is_invalid_id(args[i]))
			put_builtin_error("export", args[i], "not a valid identifier");
		else if (!ft_strchr(args[i], '='))
			put_builtin_error("export", args[i], MARK_EXP_ERR);
		else
		{
			var_name = substr_before_char(args[i], '=', minishell);
			var_val = substr_after_char(args[i], '=', minishell);
			update_env_var(var_name, var_val, minishell);
		}
		i++;
	}
}
