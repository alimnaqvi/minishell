/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:18:03 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/19 11:49:54 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	*replace_with_expanded_env(int i, t_minishell *minishell)
// {
// 	size_t	old_len;
// 	size_t	new_len;
// 	char	*var_name;
// 	char	*var_value;
// 	size_t	var_name_len;
// 	size_t	var_val_len;
// 	char	*new_str;

// 	old_len = ft_strlen(minishell->input);
// 	var_name = get_var_name();
// 	var_name_len = ft_strlen(var_name);
// 	var_value = get_env_var_value(var_name, minishell);
// 	new_str = gc_malloc(sizeof(char) * (old_len - var_name_len - 1 + var_val_len + 1), minishell);
// 	//
// }

static int	check_unclosed_quote(t_minishell *minishell)
{
	int		in_sing_quot;
	int		in_doub_quot;
	int		i;

	in_sing_quot = 0;
	in_doub_quot = 0;
	i = 0;
	while (minishell->input[i])
	{
		if (minishell->input[i] == '"' && !in_sing_quot)
			in_doub_quot = !in_doub_quot;
		else if (minishell->input[i] == '\'' && !in_doub_quot)
			in_sing_quot = !in_sing_quot;
		// if (!in_sing_quot && minishell->input[i] == '$')
		// 	minishell->input = replace_with_expanded_env(i, minishell);
		i++;
	}
	if (in_doub_quot || in_sing_quot)
		return (ft_putendl_fd(QUOTE_ERR, STDERR_FILENO), -1);
	return (0);
}

/*readline, check NULL, gc_add_to_allocs, add_history*/
// Read the input from the command line
int	ft_readline(t_minishell *minishell)
{
	minishell->input = readline("minishell$ ");
	set_signal_handler(NON_INTERACTIVE);
	if (minishell->input == NULL)
		return (ft_exit_parent((char *[]){"exit", NULL}, minishell), -1);
	add_history(minishell->input);
	gc_add_to_allocs(minishell->input, minishell);
	if (check_unclosed_quote(minishell) < 0)
		return (-1);
	return (0);
}
