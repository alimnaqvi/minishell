/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:32:15 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/18 05:07:40 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Count how big the string is between the quotes
static t_count_with_quote	count_until_quote(char *input, \
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
	token.string = ft_substr(minishell->input, \
			index, count_with_quote.count);
	if (token.string == NULL)
		gc_exit(minishell, EXIT_FAILURE);
	gc_add_to_allocs(token.string, minishell);
	if (quote == '"')
	{
		replaced = replace_env(minishell, token.string);
		if (replaced == NULL)
			replaced = gc_malloc_str(minishell, "");
		gc_free(token.string, minishell);
		token.string = replaced;
	}
	return (token);
}
