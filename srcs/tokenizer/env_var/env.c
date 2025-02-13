/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:18:14 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 15:40:36 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
If we want to replace environment variables:
1. Determine the variable we are replacing
2. Find and get the corresponding string with that key
3.

Let's create an empty terminated string and just start adding characters to it
Everytime we come across a character, let's add it to the string
And if we come across an environment variable, let's determine what
it must be replaced with
Then let's just get the
*/

// Essentially counts the characters until the `=` sign
size_t	get_env_key_len(char *env)
{
	size_t	len;

	len = 0;
	while (env[len] != 0)
	{
		if (env[len] == '=')
			break ;
		len++;
	}
	return (len);
}

// Construct a substring starting from the index until the very end
// It's called `cut front` because it essentially cuts the number of
// ...characters off from the very front of the string passed
char	*cut_front(t_minishell *minishell, char *str, size_t cut_len)
{
	size_t	str_len;
	size_t	index;
	char	*new_str;

	str_len = ft_strlen(str);
	new_str = gc_malloc(sizeof(char) * (str_len - cut_len) + 1, minishell);
	index = 0;
	while (str[cut_len + index] != 0)
	{
		new_str[index] = str[cut_len + index];
		index++;
	}
	new_str[index] = 0;
	return (new_str);
}

char	*get_env(t_minishell *minishell, char *key)
{
	size_t	key_len;
	size_t	index;
	char	*value;

	key_len = ft_strlen(key);
	index = 0;
	while (minishell->mini_env[index] != NULL)
	{
		if (key_len != get_env_key_len(minishell->mini_env[index]))
		{
			index++;
			continue ;
		}
		if (ft_strncmp(key, minishell->mini_env[index], key_len))
		{
			value = cut_front(minishell, minishell->mini_env[index], key_len
					+ 1);
			return (value);
		}
		index++;
	}
	return (NULL);
}

char	*replace_env(t_minishell *minishell, char *word)
{
	size_t				index;
	char				*output_str;
	char				*tmp;
	t_replaced_variable	replacement;

	output_str = gc_malloc(sizeof(char), minishell);
	output_str[0] = 0;
	index = 0;
	while (word[index])
	{
		if (word[index] == '$')
		{
			replacement = cut_variable(minishell, &(word[++index]));
			index += replacement.key_len;
			tmp = output_str;
			output_str = ft_strjoin(output_str, replacement.value);
			if (output_str == NULL)
				gc_exit(minishell, EXIT_FAILURE);
			gc_add_to_allocs(output_str, minishell);
			gc_free(tmp, minishell);
			continue ;
		}
		output_str = str_add_char(minishell, output_str, word[index++]);
	}
	return (output_str);
}
