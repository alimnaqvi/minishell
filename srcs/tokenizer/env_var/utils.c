/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:20:09 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/16 02:34:29 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// Counts the length of the environment variable key
// Does not count the first $ that the variable begins with
// The beginning of the `str` passed is the first char after the dollar sign
// Should contain alphanuberical characters or underscores ('_')
size_t	count_key_len(char *str)
{
	size_t	length;

	length = 0;
	while (is_env_char(str[length]))
	{
		length++;
	}
	return (length);
}

// Returns the variable that is found at the potition in the string
// Finds the key and finds the variable value for that key
// And also returns the length of the variable in teh `t_replaced_var`
t_replaced_var	cut_variable(t_minishell *minishell, char *str)
{
	char			*key_str;
	t_replaced_var	replaced_variable;

	replaced_variable.value = NULL;
	if (*str == '?')
	{
		replaced_variable.key_len = 1;
		replaced_variable.value = ft_itoa(minishell->last_exit_status);
		gc_add_to_allocs(replaced_variable.value, minishell);
		return (replaced_variable);
	}
	replaced_variable.key_len = count_key_len(str);
	key_str = ft_substr(str, 0, replaced_variable.key_len);
	if (key_str == NULL)
		gc_exit(minishell, EXIT_FAILURE);
	gc_add_to_allocs(key_str, minishell);
	if (replaced_variable.key_len == 0)
	{
		replaced_variable.value = gc_malloc(sizeof(char) * 2, minishell);
		replaced_variable.value[0] = '$';
		replaced_variable.value[1] = '\0';
		gc_free(key_str, minishell);
		return (replaced_variable);
	}
	replaced_variable.value = get_env_var_value(key_str, minishell);
	if (replaced_variable.value == NULL)
	{
		replaced_variable.value = gc_malloc(sizeof(char), minishell);
		replaced_variable.value[0] = 0;
	}
	gc_free(key_str, minishell);
	return (replaced_variable);
}

char	*str_add_char(t_minishell *minishell, char *str, char c)
{
	char	*new_str;
	size_t	str_len;

	str_len = ft_strlen(str);
	new_str = gc_realloc(sizeof(char) * (str_len + 2), str, minishell);
	new_str[str_len] = c;
	new_str[str_len + 1] = 0;
	return (new_str);
}
