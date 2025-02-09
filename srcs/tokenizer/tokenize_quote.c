/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:32:15 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/08 17:53:31 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_count_with_quote
{
	size_t	count;
	int		ends_with_quote;
}	t_count_with_quote;

// Count how big the string is between the quotes
static t_count_with_quote	count_until_quote(char *input,\
		size_t index, char quote)
{
	size_t						old_index;
	struct s_count_with_quote	return_value;

	return_value.ends_with_quote = 0;
	old_index = index;
	while (input[index] && input[index] != quote)
		index++;
	if (input[index] == quote)
		return_value.ends_with_quote = 1;
	return_value.count = index - old_index;
	return (return_value);
}

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
			break;
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
	// "11111=2222"
	// str_len = 10
	// cut_len = 6
	// alloc_len = 4 + 1
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
			continue;
		}
		if (ft_strncmp(key, minishell->mini_env[index], key_len))
		{
			value = cut_front(minishell, minishell->mini_env[index],\
					key_len + 1);
			return (value);
		}
		index++;
	}
	return (NULL);
}

char	*replace_test(t_minishell *minishell, char *word)
{
	size_t	index;

	index = 0;
	while (word[index])
	{
		// If we find a dollar sign, this means that there must be a variable
		if (word[index] == '$')
		{
			// Count the length of the word somehow
		}
		index++;
	}
}

// Return the tokenized quote
t_token	tokenize_quote(t_minishell *minishell, size_t index, char quote)
{
	t_token				token;
	t_count_with_quote	count_with_quote;
	char				*replaced;

	count_with_quote = count_until_quote(minishell->input, ++index, quote);
	token.parsed_length = count_with_quote.count;
	token.read_length = count_with_quote.count + 1;
	if (count_with_quote.ends_with_quote)
		token.read_length++;
	token.type = TOKEN_WORD;
	token.string = ft_substr(minishell->input,\
			index, count_with_quote.count);
	if (token.string == NULL)
		gc_exit(minishell, EXIT_FAILURE);
	if (quote == '\'')
	{
		replaced = replace_test(minishell, token.string);
		if (replaced == NULL)
			gc_exit(minishell, EXIT_FAILURE);
		gc_free(token.string, minishell);
		token.string = replaced;
	}
	gc_add_to_allocs(token.string, minishell);
	return (token);
}
